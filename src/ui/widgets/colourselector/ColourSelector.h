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

class QLineEdit;
class ColourSlider;
class ColourBoxSlider;
class QValidator;
struct SliderInfo;

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

signals:
	void colourChanged(const QColor& colour);
	void colourSelectionModelChanged(ColourSelectionModel model);

protected:
	// QSize sizeHint() const override;

private:
	QColor m_Colour;

	ColourSelectionModel m_SelectionModel;

	QImage* m_SquareImg;
	QImage* m_PrimarySliderImg;
	QImage* m_AlphaSliderImg;

	ColourBoxSlider* m_SquareSlider;
	ColourSlider* m_PrimarySlider;
	ColourSlider* m_AlphaSlider;
	QLineEdit* m_HexEntry;
	QValidator* m_HexEntryValidator;

	void updateImages(bool regenSquareSliderImg, bool regenPrimarySliderImg, bool regenAlphaSliderImg);
	void updateUi(bool updateSquareSlider, bool updatePrimarySlider, bool updateAlphaSlider, bool updateHex);
	void genSquareImg();
	void genPrimarySliderImg();
	void genAlphaSliderImg();

	QColor colourFromSliders(const QVariant& squareVal, int primaryVal, int alphaVal) const;
	SliderInfo slidersFromColour(const QColor& col) const;
};

#endif // COLOURSELECTOR_H