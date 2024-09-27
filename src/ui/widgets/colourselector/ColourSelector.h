#ifndef COLOURSELECTOR_H
#define COLOURSELECTOR_H

#include <QWidget>

// NOTE: A little about using CIELCh and colour management in general - It'd be quite ideal I think, to be able to pick colours in LCh and have proper colour management
//
//       So my current understanding is that I would have a working colour space that would correspond to the image's colour space,
//       and I'd pick colours in CIELCh/CIELAB and those would immediately be translated using an ICC colour profile to the working
//       colour space - that is, I'd store the currently selected colour as RGB, in the working colour space - the same as the image.
//       Now where does the display's colour space come in to it? I'm not sure - I think that Qt might render taking this into account,
//       as long as we supply the right colour space information, but I'm not sure
//
//       For now, I'm going to stick with sRGB, which is what Qt uses by default
//
// Some links:
//     - Accurate (?) colour conversion: https://colorizer.org/
//     - Wikipedia article of CIELAB, including colour conversion: https://en.wikipedia.org/wiki/CIELAB_color_space
//     - Wikipedia article of sRGB, including colour conversion: https://en.wikipedia.org/wiki/SRGB

// TODO: The way that we work with colour is maybe a little messy... Each colour model is defined by various values and calculations spread haphazardly throughout
//       ColourSelector.cpp... Maybe we wrap a colour model neatly into a class instead?

class QLineEdit;
class ColourSlider;
class ColourBoxSlider;
class QValidator;
class QSpinBox;
class QAction;
class QActionGroup;

struct SliderInfo;
struct AbcaQuad;

/// The colour model used for selecting a colour - E.g. HSV, HSL, RGB
enum class ColourSelectionModel { // These are all the colour models I'd like to support picking colours from
	Hsv,
	Hsl,
	Rgb,
	Cmy,
	// specific versions of these?
	CieLch,
	CieLab,
	CieXyz,
	// Maybe other colour models like OkLch too
	// Or maybe I should be calling these colour spaces, and colour models are just Hsv, Rgb, Cmyk, Lch, Lab, Xyz, etc. and specifics are left to the colour space
};

/// The arrangement of values across the sliders, where A, B and C represent generic "values" that match the order as defined in
/// the ColourSelectionModel - i.e. for Hsv, A is hue, B is saturation, and C is value.
/// The order of A, B and C in this enum corresponds to their associated slider value, in order PXY, where P is the primary slider
/// value, and X and Y are the coordinates in the square slider.
/// Arrangements are chosen to work best for HSV/HSL, using the same arrangements as Java Swing's colour selection dialog
enum class SliderArrangement {
	Abc,
	Bac,
	Cab
};

class ColourSelector : public QWidget {
	Q_OBJECT

public:
	explicit ColourSelector(QColor colour, QWidget* parent = nullptr);
	~ColourSelector() override;

public slots:
	void setColour(const QColor& colour);
	void setColourSelectionModel(ColourSelectionModel model);
	void setSliderArrangement(SliderArrangement arrangement);

signals:
	void colourChanged(const QColor& colour);
	void colourSelectionModelChanged(ColourSelectionModel model);
	void sliderArrangementChanged(SliderArrangement arrangement);

protected:

private:
	QColor m_Colour;

	ColourSelectionModel m_SelectionModel;
	SliderArrangement m_Arrangement;

	QImage* m_SquareImg;
	QImage* m_PrimarySliderImg;
	QImage* m_AlphaSliderImg;

	ColourBoxSlider* m_SquareSlider;
	ColourSlider* m_PrimarySlider;
	ColourSlider* m_AlphaSlider;
	QLineEdit* m_HexEntry;
	QValidator* m_HexEntryValidator;

	QActionGroup* m_SelectionModelActions;
	QAction* m_HsvAction;
	QAction* m_HslAction;
	QAction* m_RgbAction;
	QAction* m_CmyAction;

	QActionGroup* m_ArrangementActions;
	QAction* m_AbcAction;
	QAction* m_BacAction;
	QAction* m_CabAction;

	QSpinBox* m_SpinA;
	QSpinBox* m_SpinB;
	QSpinBox* m_SpinC;
	QSpinBox* m_SpinAlpha;

	/// If true, then slider value change events will fire silently
	bool m_EventLock;

	void enableEventLock();
	void disableEventLock();

	/// Sets the minimum/maximum for all sliders, dependent on the current colour selection model and slider arrangement
	void updateUiBounds();
	void updateImages(bool regenSquareSliderImg, bool regenPrimarySliderImg, bool regenAlphaSliderImg);
	void updateUi(bool updateSquareSlider, bool updatePrimarySlider, bool updateAlphaSlider, bool updateHex);

	void genSquareImg();
	void genPrimarySliderImg();
	void genAlphaSliderImg();

	/// Calculates the colour represented by the sliders, dependent on the current selection model and slider arrangement
	QColor colourFromSliders(const QVariant& squareVal, int primaryVal, int alphaVal) const;
	/// Calculates the slider values represented by the colour, dependent on the current selection model and slider arrangement
	SliderInfo slidersFromColour(const QColor& col) const;

	/// Uses the current colour model to calculate the colour from the normalised (ABC-arranged) input
	QColor colourFromAbca(const AbcaQuad& abca) const;
	/// Uses the current colour model to calculate a normalised (ABC-arranged) AbcaQuad from the input colour
	AbcaQuad abcaFromColour(const QColor& col) const;

	/// Normalises the provided SliderInfo to use the ABC slider arrangement, from the current slider arrangement
	AbcaQuad abcaFromSliders(const SliderInfo& info) const;
	/// Rearranges the values from the normalised (ABC-arranged) input into a new SliderInfo, dependent on the current slider arrangement
	SliderInfo slidersFromAbca(const AbcaQuad& abca) const;
};

#endif // COLOURSELECTOR_H