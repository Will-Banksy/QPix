#include "ColourSelector.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QImage>
#include "ColourSlider.h"
#include <QSlider>
#include "ColourBoxSlider.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLabel>

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

	this->updateImages(true, true, true);

	m_SquareSlider = new ColourBoxSlider(m_SquareImg);
	m_SquareSlider->setMinimum(QVariant(QPoint(0, 0)));
	m_SquareSlider->setMaximum(QVariant(QPoint(255, 255)));
	m_SquareSlider->setValue(QVariant(QPoint(m_Colour.hsvSaturation(), m_Colour.value())));

	m_HueSlider = new ColourSlider(m_HueSliderImg, Qt::Orientation::Vertical);
	m_HueSlider->setMinimum(0);
	m_HueSlider->setMaximum(359); // NOTE: Since this is maxing out at 359, not 360, does this affect anything else?
	m_HueSlider->setValue(colour.hue());

	m_AlphaSlider = new ColourSlider(m_AlphaSliderImg, Qt::Orientation::Horizontal);
	m_AlphaSlider->setMinimum(0);
	m_AlphaSlider->setMaximum(255);
	m_AlphaSlider->setValue(colour.alpha());

	QLabel* hash = new QLabel("#");
	hash->setFixedWidth(8);

	m_HexEntry->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed));
	m_HexEntry->setFont(QFont("Monospace"));
	m_HexEntry->setPlaceholderText("aarrggbb");
	m_HexEntry->setFrame(false);
	QRegularExpression re = QRegularExpression("[a-fA-F0-9]{8}");
	m_HexEntryValidator = new QRegularExpressionValidator(re);
	m_HexEntry->setValidator(m_HexEntryValidator);
	m_HexEntry->setText(colour.name(QColor::NameFormat::HexArgb).sliced(1).toLower());

	connect(m_SquareSlider, &ColourBoxSlider::valueChanged, this, [this](QVariant value) {
		QPoint pt = value.toPoint();
		this->setColour(QColor::fromHsv(qMax(this->m_Colour.hue(), 0), pt.x(), 255 - pt.y(), this->m_Colour.alpha()));
	});
	connect(m_HueSlider, &ColourSlider::valueChanged, this, [this](int value) {
		this->setColour(QColor::fromHsv(value, this->m_Colour.hsvSaturation(), this->m_Colour.value(), this->m_Colour.alpha()));
	});
	connect(m_AlphaSlider, &ColourSlider::valueChanged, this, [this](int value) {
		QColor newCol = QColor(m_Colour);
		newCol.setAlpha(value);
		this->setColour(newCol);
	});
	connect(m_HexEntry, &QLineEdit::editingFinished, this, [this]() {
		this->setColour(QColor::fromString(QString("#").append(m_HexEntry->text())));
	});

	QGridLayout* layout = new QGridLayout();
	layout->setSpacing(8);

	layout->addWidget(m_SquareSlider, 0, 0);
	layout->addWidget(m_HueSlider, 0, 1);
	layout->addWidget(m_AlphaSlider, 1, 0);

	QHBoxLayout* horBox = new QHBoxLayout();
	horBox->addWidget(hash);
	horBox->addWidget(m_HexEntry);

	layout->addLayout(horBox, 2, 0);

	this->setLayout(layout);
}

ColourSelector::~ColourSelector() {
	m_HexEntryValidator->deleteLater();
	delete m_SquareImg;
	delete m_AlphaSliderImg;
	delete m_HueSliderImg;
}

void ColourSelector::setColour(const QColor& colour) {
	bool hueDiff = colour.hue() != m_Colour.hue();
	bool rgbDiff = colour.rgb() != m_Colour.rgb();
	bool alphaDiff = colour.alpha() != m_Colour.alpha();
	bool satDiff = colour.hsvSaturation() != m_Colour.hsvSaturation();
	bool valDiff = colour.value() != m_Colour.value();
	emit colourChanged(colour);
	this->m_Colour = colour;

	updateImages(hueDiff, false, rgbDiff);
	this->updateUi(satDiff || valDiff, hueDiff, alphaDiff, rgbDiff || alphaDiff);
	this->update();
}

QSize ColourSelector::sizeHint() const {
	return QSize(280, 280);
}

void ColourSelector::updateImages(bool regenSquareSliderImg, bool regenPrimarySliderImg, bool regenAlphaSliderImg) {
	if(regenSquareSliderImg) {
		this->genSquareImg();
	}
	if(regenPrimarySliderImg) {
		this->genHueSliderImg();
	}
	if(regenAlphaSliderImg) {
		this->genAlphaSliderImg();
	}
}

void ColourSelector::updateUi(bool updateSquareSlider, bool updatePrimarySlider, bool updateAlphaSlider, bool updateHex) {
	if(updateSquareSlider) {
		m_SquareSlider->setValue(QVariant(QPoint(m_Colour.hsvSaturation(), 255 - m_Colour.value())));
	}
	if(updatePrimarySlider) {
		m_HueSlider->setValue(m_Colour.hue());
	}
	if(updateAlphaSlider) {
		m_AlphaSlider->setValue(m_Colour.alpha());
	}
	if(updateHex) {
		m_HexEntry->setText(m_Colour.name(QColor::NameFormat::HexArgb).sliced(1).toLower());
	}
}

// TODO: These image gen functions are very specific and not very optimised (mainly the hue slider image since it will not be making good use of prefetching/cache)

void ColourSelector::genSquareImg() {
	QRgb* pixels = (QRgb*)m_SquareImg->scanLine(0);

	for(int i = 0; i < m_SquareImg->width(); i++) {
		for(int j = 0; j < m_SquareImg->height(); j++) {
			// Qt returns -1 for achromatic colours, such as black - Not usually an issue but can be an issue on startup. This stops the square image from displaying only achromatic colours
			int hue = qMax(m_Colour.hue(), 0);
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
