#ifndef COLOURSELECTOR_H
#define COLOURSELECTOR_H

#include <QWidget>

enum class ColourSelectionModel {
	Hsv,
};

class QLineEdit;
class QSlider;

class ColourSelector : public QWidget {
	Q_OBJECT

public:
	explicit ColourSelector(QColor colour, QWidget* parent = nullptr);
	~ColourSelector() override;

public slots:
	void setColour(const QColor& colour);

signals:
	void colourChanged(const QColor& colour);

private:
	QColor m_Colour;

	QImage* m_SquareImg;
	QImage* m_HueSliderImg;
	QImage* m_AlphaSliderImg;

	QSlider* m_HueSlider;
	QSlider* m_AlphaSlider;
	QLineEdit* m_HexEntry;

	void genSquareImg();
	void genHueSliderImg();
	void genAlphaSliderImg();
};

#endif // COLOURSELECTOR_H