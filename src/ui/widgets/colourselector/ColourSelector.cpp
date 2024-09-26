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
#include <QPushButton>
#include "utils/Utils.h"
#include <iostream>

const int SLIDER_WIDTH = 32;

struct SliderInfo {
public:
	QVariant m_SquareVal;
	int m_PrimaryVal;
	int m_AlphaVal;
};

// FIXME: When editing the hexcode of a colour, some colours are rounded to a different but nearby colour -
//        e.g. #ff000eff rounds to #ff0011ff when input. This should absolutely not happen. Might be to do with
//        HSV/HSL not being able to encode the colour - Which is another issue, that working with floats could
//        potentially solve - Except, QAbstractSlider operates on integers (which okay that can be worked around
//        with sliding over values x10). Add asserts until the problem is diagnosed for real
// NOTE: Okay noticed Aseprite also has this issue (except it's only visible when changing the slider values, and
//       it rounds more sensibly like #ff000eff to #ff000dff instead of #ff0011ff) - I don't think 0-255 gives
//       enough precision for a 1:1 mapping of RGB->HSV (let alone HSL) so this is always going to be an issue there.
//       But, since Aseprite has this issue, maybe it's fine...? Although, can we do better...

ColourSelector::ColourSelector(QColor colour, QWidget* parent) : QWidget(parent),
	m_Colour(colour),
	m_SelectionModel(ColourSelectionModel::Hsv),
	m_SquareImg(new QImage(255, 255, QImage::Format_ARGB32)),
	m_PrimarySliderImg(new QImage(SLIDER_WIDTH, 360, QImage::Format_ARGB32)),
	m_AlphaSliderImg(new QImage(255, SLIDER_WIDTH, QImage::Format_ARGB32)),
	m_HexEntry(new QLineEdit()) {

	// NOTE: Do I just copy (with adjustments of course) the colour selector from my Java Pixel Editor?
	//       I do want to support other colour models of course, but specifically the vague styling (I will need it to fit in with QPix's theme however) and interactivity of it
	//           Look at Java swing's powerful colour picker dialog (button in my Java Pixel Editor's colour picker) - If I can approach that level of power, I'll be doing good
	//           Now with supporting different colour models, do I want to support perception-based models such as CIELAB/CIELCH, etc?
	//               Probably do some research as to what that would require - would I need to take into account display colour calibration, etc?

	m_SquareSlider = new ColourBoxSlider(m_SquareImg);
	m_SquareSlider->setPreferredSize(QSize(255, 255));

	m_PrimarySlider = new ColourSlider(m_PrimarySliderImg, Qt::Orientation::Vertical);
	m_PrimarySlider->setPreferredLength(255);

	m_AlphaSlider = new ColourSlider(m_AlphaSliderImg, Qt::Orientation::Horizontal);
	m_AlphaSlider->setMinimum(0);
	m_AlphaSlider->setMaximum(255);
	m_AlphaSlider->setValue(colour.alpha());
	m_AlphaSlider->setPreferredLength(255);

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

	QPushButton* hsvButton = new QPushButton("HSV");

	QPushButton* hslButton = new QPushButton("HSL");

	QPushButton* rgbButton = new QPushButton("RGB");

	connect(m_SquareSlider, &ColourBoxSlider::valueChanged, this, [this](QVariant value) {
		// QPoint pt = value.toPoint();
		SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
		this->setColour(this->colourFromSliders(value, sliderInfo.m_PrimaryVal, sliderInfo.m_AlphaVal));
		// this->setColour(QColor::fromHsv(qMax(this->m_Colour.hue(), 0), pt.x(), 255 - pt.y(), this->m_Colour.alpha()));
	});
	connect(m_PrimarySlider, &ColourSlider::valueChanged, this, [this](int value) {
		SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
		this->setColour(this->colourFromSliders(sliderInfo.m_SquareVal, value, sliderInfo.m_AlphaVal));
		// this->setColour(QColor::fromHsv(value, this->m_Colour.hsvSaturation(), this->m_Colour.value(), this->m_Colour.alpha()));
	});
	connect(m_AlphaSlider, &ColourSlider::valueChanged, this, [this](int value) {
		// QColor newCol = QColor(m_Colour);
		// newCol.setAlpha(value);
		// this->setColour(newVal);
		SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
		this->setColour(this->colourFromSliders(sliderInfo.m_SquareVal, sliderInfo.m_PrimaryVal, value));
	});
	connect(m_HexEntry, &QLineEdit::editingFinished, this, [this]() {
		this->setColour(QColor::fromString(QString("#").append(m_HexEntry->text())));
	});

	connect(hsvButton, &QPushButton::clicked, this, [this](bool checked) {
		this->setColourSelectionModel(ColourSelectionModel::Hsv);
	});
	connect(hslButton, &QPushButton::clicked, this, [this](bool checked) {
		this->setColourSelectionModel(ColourSelectionModel::Hsl);
	});
	connect(rgbButton, &QPushButton::clicked, this, [this](bool checked) {
		this->setColourSelectionModel(ColourSelectionModel::Rgb);
	});

	this->updateImages(true, true, true);
	this->setColourSelectionModel(ColourSelectionModel::Rgb);

	QGridLayout* layout = new QGridLayout();
	layout->setSpacing(8);

	QHBoxLayout* modelSelBox = new QHBoxLayout();
	modelSelBox->addWidget(hsvButton);
	modelSelBox->addWidget(hslButton);
	modelSelBox->addWidget(rgbButton);

	layout->addLayout(modelSelBox, 0, 0, 1, 0);
	layout->addWidget(m_SquareSlider, 1, 0);
	layout->addWidget(m_PrimarySlider, 1, 1);
	layout->addWidget(m_AlphaSlider, 2, 0);

	QHBoxLayout* hexBox = new QHBoxLayout();
	hexBox->addWidget(hash);
	hexBox->addWidget(m_HexEntry);

	layout->addLayout(hexBox, 3, 0, 1, 0);

	this->setLayout(layout);
}

ColourSelector::~ColourSelector() {
	m_HexEntryValidator->deleteLater();
	delete m_SquareImg;
	delete m_AlphaSliderImg;
	delete m_PrimarySliderImg;
}

// TODO: Update to work with different models - there may always be a risk of infinite recursion here, as it relies on colours being 1:1 between different colour models
void ColourSelector::setColour(const QColor& colour) {
	// bool hueDiff = colour.hue() != m_Colour.hue();
	// bool rgbDiff = colour.rgb() != m_Colour.rgb();
	// bool alphaDiff = colour.alpha() != m_Colour.alpha();
	// bool satDiff = colour.hsvSaturation() != m_Colour.hsvSaturation();
	// bool valDiff = colour.value() != m_Colour.value();

	bool alphaDiff = colour.alpha() != m_Colour.alpha();
	bool rgbaDiff = (colour.rgba() != m_Colour.rgba()) || (colour.hue() != m_Colour.hue());

	std::cout << "colour: " << std::hex << colour.rgba() << ", m_Colour: " << std::hex << m_Colour.rgba() << std::endl;

	emit colourChanged(colour);
	this->m_Colour = colour;

	// updateImages(hueDiff, false, rgbDiff);
	// this->updateUi(satDiff || valDiff, hueDiff, alphaDiff, rgbDiff || alphaDiff);
	if(rgbaDiff) {
		updateImages(true, true, true);
		this->updateUi(true, true, true, true);
	}
	this->update();
}

void ColourSelector::setColourSelectionModel(ColourSelectionModel model) {
	emit colourSelectionModelChanged(model);
	m_SelectionModel = model;
	// TODO: Break the following into it's own function?
	switch(model) {
		case ColourSelectionModel::Hsv:
		case ColourSelectionModel::Hsl: {
			m_PrimarySlider->setMinimum(0);
			m_PrimarySlider->setMaximum(359); // NOTE: Since this is maxing out at 359, not 360, does this affect anything else?

			m_SquareSlider->setMinimum(QVariant(QPoint(0, 0)));
			m_SquareSlider->setMaximum(QVariant(QPoint(255, 255)));
			break;
		}
		case ColourSelectionModel::Rgb: {
			m_PrimarySlider->setMinimum(0);
			m_PrimarySlider->setMaximum(255);

			m_SquareSlider->setMinimum(QVariant(QPoint(0, 0)));
			m_SquareSlider->setMaximum(QVariant(QPoint(255, 255)));
		}
	}
	this->updateImages(true, true, false);
	this->updateUi(true, true, false, false);

	this->update();
}

// QSize ColourSelector::sizeHint() const {
// 	return QSize(400, 400);
// }

void ColourSelector::updateImages(bool regenSquareSliderImg, bool regenPrimarySliderImg, bool regenAlphaSliderImg) {
	if(regenSquareSliderImg) {
		this->genSquareImg();
	}
	if(regenPrimarySliderImg) {
		this->genPrimarySliderImg();
	}
	if(regenAlphaSliderImg) {
		this->genAlphaSliderImg();
	}
}

void ColourSelector::updateUi(bool updateSquareSlider, bool updatePrimarySlider, bool updateAlphaSlider, bool updateHex) {
	SliderInfo sliderInfo = slidersFromColour(m_Colour);

	if(updateSquareSlider) {
		m_SquareSlider->setValue(sliderInfo.m_SquareVal);
	}
	if(updatePrimarySlider) {
		m_PrimarySlider->setValue(sliderInfo.m_PrimaryVal);
	}
	if(updateAlphaSlider) {
		m_AlphaSlider->setValue(sliderInfo.m_AlphaVal);
	}
	if(updateHex) {
		m_HexEntry->setText(m_Colour.name(QColor::NameFormat::HexArgb).sliced(1).toLower());
	}

	QColor fromSliders = this->colourFromSliders(m_SquareSlider->value(), m_PrimarySlider->value(), m_AlphaSlider->value());
	assert(fromSliders.rgba() == m_Colour.rgba());
}

void ColourSelector::genSquareImg() {
	QRgb* pixels = (QRgb*)m_SquareImg->scanLine(0);

	for(int i = 0; i < m_SquareImg->width(); i++) {
		for(int j = 0; j < m_SquareImg->height(); j++) {
			int a = utils::map(i, 0, m_SquareImg->width(), m_SquareSlider->minimum().toPoint().x(), m_SquareSlider->maximum().toPoint().x());
			int b = utils::map(j, 0, m_SquareImg->height(), m_SquareSlider->minimum().toPoint().y(), m_SquareSlider->maximum().toPoint().y());

			SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
			QColor col = this->colourFromSliders(QVariant(QPoint(a, b)), sliderInfo.m_PrimaryVal, 255);
			pixels[i + j * m_SquareImg->width()] = col.rgba();
		}
	}
}

void ColourSelector::genPrimarySliderImg() {
	QRgb* pixels = (QRgb*)m_PrimarySliderImg->scanLine(0);

	for(int j = 0; j < m_PrimarySliderImg->height(); j++) {
		int p = utils::map(j, 0, m_PrimarySliderImg->height(), m_PrimarySlider->minimum(), m_PrimarySlider->maximum());
		SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
		QColor col = this->colourFromSliders(sliderInfo.m_SquareVal, p, 255);
		for(int i = 0; i < m_PrimarySliderImg->width(); i++) {
			pixels[i + j * m_PrimarySliderImg->width()] = col.rgba();
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

QColor ColourSelector::colourFromSliders(const QVariant& squareVal, int primaryVal, int alphaVal) const { // TODO: Refactor this when we allow changing the primary slider
	QPoint squarePt = squareVal.toPoint();
	switch(m_SelectionModel) {
		case ColourSelectionModel::Hsv: {
			// Qt returns -1 for achromatic colours, such as black - Not usually an issue but can be an issue on startup. This corrects that flaw
			int hue = qMax(primaryVal, 0);
			int sat = squarePt.x();
			int val = squarePt.y();

			QColor col = QColor::fromHsv(hue, sat, val, alphaVal);

			assert(hue == col.hue());
			assert(sat == col.hsvSaturation());
			assert(val == col.value());

			return col;
		}
		case ColourSelectionModel::Hsl: {
			int hue = qMax(primaryVal, 0);
			int sat = squarePt.x();
			int lig = squarePt.y();

			QColor col = QColor::fromHsl(hue, sat, lig, alphaVal);

			assert(hue == qMax(col.hslHue(), 0));
			assert(sat == col.hslSaturation());
			assert(lig == col.lightness());

			return col;
		}
		case ColourSelectionModel::Rgb: {
			int red = primaryVal;
			int green = squarePt.x();
			int blue = squarePt.y();

			QColor col = QColor(red, green, blue, alphaVal);

			assert(red == col.red());
			assert(green == col.green());
			assert(blue == col.blue());

			return col;
		}
	}
	assert(false);
}

SliderInfo ColourSelector::slidersFromColour(const QColor& col) const {  // TODO: Refactor this when we allow changing the primary slider
	SliderInfo info;

	switch(m_SelectionModel) {
		case ColourSelectionModel::Hsv: {
			info = {
				.m_SquareVal = QVariant(QPoint(m_Colour.hsvSaturation(), m_Colour.value())),
				.m_PrimaryVal = qMax(m_Colour.hue(), 0),
				.m_AlphaVal = m_Colour.alpha()
			};
			break;
		}
		case ColourSelectionModel::Hsl: {
			info = {
				.m_SquareVal = QVariant(QPoint(m_Colour.hslSaturation(), m_Colour.lightness())),
				.m_PrimaryVal = qMax(m_Colour.hslHue(), 0),
				.m_AlphaVal = m_Colour.alpha()
			};
			break;
		}
		case ColourSelectionModel::Rgb: {
			info = {
				.m_SquareVal = QVariant(QPoint(m_Colour.green(), m_Colour.blue())),
				.m_PrimaryVal = qMax(m_Colour.red(), 0),
				.m_AlphaVal = m_Colour.alpha()
			};
			break;
		}
		default:
			assert(false);
	}

	assert(info.m_SquareVal.toPoint().x() >= m_SquareSlider->minimum().toPoint().x() && info.m_SquareVal.toPoint().y() >= m_SquareSlider->minimum().toPoint().y());
	assert(info.m_SquareVal.toPoint().x() <= m_SquareSlider->maximum().toPoint().x() && info.m_SquareVal.toPoint().y() <= m_SquareSlider->maximum().toPoint().y());
	assert(info.m_PrimaryVal >= m_PrimarySlider->minimum() && info.m_PrimaryVal <= m_PrimarySlider->maximum());
	assert(info.m_AlphaVal >= m_AlphaSlider->minimum() && info.m_AlphaVal <= m_AlphaSlider->maximum());

	return info;
}
