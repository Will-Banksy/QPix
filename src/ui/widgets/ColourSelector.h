#ifndef COLOURSELECTOR_H
#define COLOURSELECTOR_H

#include <QWidget>

enum class ColourSelectionModel {
	Hsv,
};

class QLineEdit;
class ColourSlider;

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

	ColourSlider* m_HueSlider;
	ColourSlider* m_AlphaSlider;
	QLineEdit* m_HexEntry;

	void genSquareImg();
	void genHueSliderImg();
	void genAlphaSliderImg();
};

#endif // COLOURSELECTOR_H