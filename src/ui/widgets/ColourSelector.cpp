#include "ColourSelector.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QImage>
#include "ColourSlider.h"
#include <QSlider>
#include "ColourBoxSlider.h"

const int SLIDER_WIDTH = 32;

ColourSelector::ColourSelector(QColor colour, QWidget* parent) : QWidget(parent),
	m_Colour(colour),
	m_SquareImg(new QImage(255, 255, QImage::Format_ARGB32)),
	m_HueSliderImg(new QImage(SLIDER_WIDTH, 360, QImage::Format_ARGB32)),
	m_AlphaSliderImg(new QImage(255, SLIDER_WIDTH, QImage::Format_ARGB32)),
	m_HexEntry(new QLineEdit()) {

	// NOTE: Do I just copy (with adjustments of course) the colour selector from my Java Pixel Editor?
	//       I do want to support other colour models of course, but specifically the vague styling (I will need it to fit in with QPix's theme however) and interactivity of it
	//           Look at Java swing's powerful colour picker dialog (button in my Java Pixel Editor's colour picker) - If I can approach that level of power, I'll be doing good
	//           Now with supporting different colour models, do I want to support perception-based models such as CIELAB/CIELCH, etc?
	//               Probably do some research as to what that would require - would I need to take into account display colour calibration, etc?

	genSquareImg();
	genHueSliderImg();
	genAlphaSliderImg();

	m_SquareSlider = new ColourBoxSlider(m_SquareImg);
	m_SquareSlider->setMinimum(QVariant(QPoint(0, 0)));
	m_SquareSlider->setMaximum(QVariant(QPoint(255, 255)));
	m_SquareSlider->setValue(QVariant(QPoint(m_Colour.hsvSaturation(), m_Colour.value())));

	m_HueSlider = new ColourSlider(m_HueSliderImg, Qt::Orientation::Vertical);
	m_HueSlider->setMinimum(0);
	m_HueSlider->setMaximum(360);
	m_HueSlider->setValue(colour.hue());
	// m_HueSlider->setInvertedAppearance(true);

	m_AlphaSlider = new ColourSlider(m_AlphaSliderImg, Qt::Orientation::Horizontal);
	m_AlphaSlider->setMinimum(0);
	m_AlphaSlider->setMaximum(255);
	m_AlphaSlider->setValue(colour.alpha());

	connect(m_SquareSlider, &ColourBoxSlider::valueChanged, this, [this](QVariant value) {
		QPoint pt = value.toPoint();
		this->m_Colour.setHsv(this->m_Colour.hsvHue(), pt.x(), 255 - pt.y(), this->m_Colour.alpha());
		emit colourChanged(this->m_Colour);
		this->genAlphaSliderImg();
		this->update();
	});
	connect(m_HueSlider, &ColourSlider::valueChanged, this, [this](int value) {
		this->m_Colour.setHsv(value, this->m_Colour.hsvSaturation(), this->m_Colour.value(), this->m_Colour.alpha());
		emit colourChanged(this->m_Colour);
		this->genAlphaSliderImg();
		this->genSquareImg();
		this->update();
	});
	connect(m_AlphaSlider, &ColourSlider::valueChanged, this, [this](int value) {
		this->m_Colour.setAlpha(value);
		emit colourChanged(this->m_Colour);
	});

	QGridLayout* layout = new QGridLayout();
	layout->setSpacing(8);

	layout->addWidget(m_SquareSlider, 0, 0);
	layout->addWidget(m_HueSlider, 0, 1);
	layout->addWidget(m_AlphaSlider, 1, 0);

	this->setLayout(layout);
}

ColourSelector::~ColourSelector() {
}

void ColourSelector::setColour(const QColor& colour) {
	m_HueSlider->setValue(colour.hue());
	m_AlphaSlider->setValue(colour.alpha());
}

QSize ColourSelector::sizeHint() const {
	return QSize(240, 240);
}

// TODO: These image gen functions are very specific and not very optimised (mainly the hue slider image since it will not be making good use of prefetching/cache)

void ColourSelector::genSquareImg() {
	QRgb* pixels = (QRgb*)m_SquareImg->scanLine(0);

	for(int i = 0; i < m_SquareImg->width(); i++) {
		for(int j = 0; j < m_SquareImg->height(); j++) {
			int hue = m_Colour.hue();
			int sat = i;
			int bri = 255 - j;
			QColor col = QColor::fromHsv(hue, sat, bri, 255);
			pixels[i + j * m_SquareImg->width()] = col.rgba();
		}
	}
}

void ColourSelector::genHueSliderImg() {
	QRgb* pixels = (QRgb*)m_HueSliderImg->scanLine(0);

	for(int j = 0; j < m_HueSliderImg->height(); j++) {
		QColor col = QColor::fromHsv(j, 255, 255, 255);
		for(int i = 0; i < m_HueSliderImg->width(); i++) {
			pixels[i + j * m_HueSliderImg->width()] = col.rgba();
		}
	}
}

void ColourSelector::genAlphaSliderImg() {
	QRgb* pixels = (QRgb*)m_AlphaSliderImg->scanLine(0);

	for(int i = 0; i < m_AlphaSliderImg->width(); i++) {
		QColor col = QColor(m_Colour);
		col.setAlpha(i);
		for(int j = 0; j < m_AlphaSliderImg->height(); j++) {
			pixels[i + j * m_AlphaSliderImg->width()] = col.rgba();
		}
	}
}
