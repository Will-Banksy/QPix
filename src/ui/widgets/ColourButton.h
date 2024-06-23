#ifndef COLOURBUTTON_H
#define COLOURBUTTON_H

#include <QToolButton>

class ColourButton : public QToolButton {
	Q_OBJECT

public:
	explicit ColourButton(const QColor& colour);
	~ColourButton() override;

public slots:
	void updateColour(const QColor& colour);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	static QPixmap* s_TransparentBackground;

	QColor m_Colour;
};

#endif // COLOURBUTTON_H