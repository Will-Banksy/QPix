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
// Some links:
//     - Accurate (?) colour conversion: https://colorizer.org/
//     - Wikipedia article of CIELAB, including colour conversion: https://en.wikipedia.org/wiki/CIELAB_color_space
//     - Wikipedia article of sRGB, including colour conversion: https://en.wikipedia.org/wiki/SRGB

enum class ColourSelectionModel { // These are all the colour models I'd like to support picking colours from - But first of course I need to figure out colour management
	Hsv,
	Rgb,
	Cmyk,
	// specific versions of these?
	CieLch,
	CieLab,
	CieXyz,
	// Maybe other colour models like OkLch too
	// Or maybe I should be calling these colour spaces, and colour models are just Hsv, Rgb, Cmyk, Lch, Lab, Xyz, etc. and specifics are left to the colour space
};

class QLineEdit;
class ColourSlider;
class ColourBoxSlider;
class QValidator;

class ColourSelector : public QWidget {
	Q_OBJECT

public:
	explicit ColourSelector(QColor colour, QWidget* parent = nullptr);
	~ColourSelector() override;

public slots:
	void setColour(const QColor& colour);

signals:
	void colourChanged(const QColor& colour);

protected:
	QSize sizeHint() const override;

private:
	QColor m_Colour;

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
};

#endif // COLOURSELECTOR_H