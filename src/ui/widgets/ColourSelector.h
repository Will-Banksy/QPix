#ifndef COLOURSELECTOR_H
#define COLOURSELECTOR_H

#include <QWidget>

enum class ColourSelectionModel {
	Hsv,
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
	QImage* m_HueSliderImg;
	QImage* m_AlphaSliderImg;

	ColourBoxSlider* m_SquareSlider;
	ColourSlider* m_HueSlider;
	ColourSlider* m_AlphaSlider;
	QLineEdit* m_HexEntry;
	QValidator* m_HexEntryValidator;

	void updateImages(bool regenSquareSliderImg, bool regenPrimarySliderImg, bool regenAlphaSliderImg);
	void updateUi(bool updateSquareSlider, bool updatePrimarySlider, bool updateAlphaSlider, bool updateHex);
	void genSquareImg();
	void genHueSliderImg();
	void genAlphaSliderImg();
};

#endif // COLOURSELECTOR_H