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
#include <QSpinBox>
#include <QAction>
#include <QActionGroup>
#include <QToolButton>

const int SLIDER_WIDTH = 32;

struct SliderInfo {
public:
	QVariant SquareVal;
	int PrimaryVal;
	int AlphaVal;
};

struct AbcaQuad {
public:
	int A;
	int B;
	int C;
	int Alpha;
};

// NOTE: Some colour models, using 0-255, cannot represent some RGB colours - e.g. HSL cannot represent pure red.
//       I noticed Aseprite also has this issue, but seemingly with better rounding - I don't think 0-255 gives
//       enough precision for a 1:1 mapping of RGB->HSV (let alone HSL) so this is always going to be an issue
//       most likely.
//       But, since Aseprite has this issue, maybe it's fine...? Although, can we do better...
//
// TODO: Investigate how my code rounds colours maybe

// TODO: Do we need different layouts to cater for using this widget in different contexts? I.e. in a dock widget

ColourSelector::ColourSelector(QColor colour, QWidget* parent) : QWidget(parent),
	m_Colour(colour),
	m_SelectionModel(ColourSelectionModel::Hsv),
	m_Arrangement(SliderArrangement::Abc),
	m_SquareImg(new QImage(255, 255, QImage::Format_ARGB32)),
	m_PrimarySliderImg(new QImage(SLIDER_WIDTH, 360, QImage::Format_ARGB32)),
	m_AlphaSliderImg(new QImage(255, SLIDER_WIDTH, QImage::Format_ARGB32)),
	m_HexEntry(new QLineEdit()) {

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

	connect(m_SquareSlider, &ColourBoxSlider::valueChanged, this, [this](QVariant value) {
		if(!this->m_EventLock) {
			SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
			this->setColour(this->colourFromSliders(value, sliderInfo.PrimaryVal, sliderInfo.AlphaVal));
		}
	});
	connect(m_PrimarySlider, &ColourSlider::valueChanged, this, [this](int value) {
		if(!this->m_EventLock) {
			SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
			this->setColour(this->colourFromSliders(sliderInfo.SquareVal, value, sliderInfo.AlphaVal));
		}
	});
	connect(m_AlphaSlider, &ColourSlider::valueChanged, this, [this](int value) {
		if(!this->m_EventLock) {
			SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
			this->setColour(this->colourFromSliders(sliderInfo.SquareVal, sliderInfo.PrimaryVal, value));
		}
	});
	connect(m_HexEntry, &QLineEdit::editingFinished, this, [this]() {
		this->enableEventLock();
		this->setColour(QColor::fromString(QString("#").append(m_HexEntry->text())));
		this->disableEventLock();
	});

	// Colour model switch buttons
	m_SelectionModelActions = new QActionGroup(nullptr);
	m_SelectionModelActions->setExclusive(true);

	m_HsvAction = new QAction("HSV", m_SelectionModelActions);
	m_HsvAction->setCheckable(true);
	QToolButton* hsvButton = new QToolButton();
	hsvButton->setDefaultAction(m_HsvAction);
	hsvButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

	m_HslAction = new QAction("HSL", m_SelectionModelActions);
	m_HslAction->setCheckable(true);
	QToolButton* hslButton = new QToolButton();
	hslButton->setDefaultAction(m_HslAction);
	hslButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

	m_RgbAction = new QAction("RGB", m_SelectionModelActions);
	m_RgbAction->setCheckable(true);
	QToolButton* rgbButton = new QToolButton();
	rgbButton->setDefaultAction(m_RgbAction);
	rgbButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

	m_CmyAction = new QAction("CMY", m_SelectionModelActions);
	m_CmyAction->setCheckable(true);
	QToolButton* cmyButton = new QToolButton();
	cmyButton->setDefaultAction(m_CmyAction);
	cmyButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));


	connect(m_HsvAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setColourSelectionModel(ColourSelectionModel::Hsv);
		}
	});
	connect(m_HslAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setColourSelectionModel(ColourSelectionModel::Hsl);
		}
	});
	connect(m_RgbAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setColourSelectionModel(ColourSelectionModel::Rgb);
		}
	});
	connect(m_CmyAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setColourSelectionModel(ColourSelectionModel::Cmy);
		}
	});

	// Slider arrangement switch actions/buttons (the text will not stay)
	m_ArrangementActions = new QActionGroup(nullptr);
	m_ArrangementActions->setExclusive(true);

	m_AbcAction = new QAction("ABC", m_ArrangementActions);
	m_AbcAction->setCheckable(true);
	QToolButton* abcButton = new QToolButton();
	abcButton->setDefaultAction(m_AbcAction);
	abcButton->setFixedWidth(80);

	m_BacAction = new QAction("BAC", m_ArrangementActions);
	m_BacAction->setCheckable(true);
	QToolButton* bacButton = new QToolButton();
	bacButton->setDefaultAction(m_BacAction);
	bacButton->setFixedWidth(80);

	m_CabAction = new QAction("CAB", m_ArrangementActions);
	m_CabAction->setCheckable(true);
	QToolButton* cabButton = new QToolButton();
	cabButton->setDefaultAction(m_CabAction);
	cabButton->setFixedWidth(80);

	connect(m_AbcAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setSliderArrangement(SliderArrangement::Abc);
		}
	});
	connect(m_BacAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setSliderArrangement(SliderArrangement::Bac);
		}
	});
	connect(m_CabAction, &QAction::toggled, this, [this](bool checked) {
		if(checked) {
			this->setSliderArrangement(SliderArrangement::Cab);
		}
	});

	m_SpinA = new QSpinBox();
	m_SpinB = new QSpinBox();
	m_SpinC = new QSpinBox();
	m_SpinAlpha = new QSpinBox();
	m_SpinAlpha->setMinimum(0);
	m_SpinAlpha->setMaximum(255);
	m_SpinAlpha->setValue(m_Colour.alpha());

	connect(m_SpinA, &QSpinBox::valueChanged, this, [this](int value) {
		if(!this->m_EventLock) {
			AbcaQuad abca = this->abcaFromColour(this->m_Colour);
			abca.A = value;
			this->setColour(this->colourFromAbca(abca));
		}
	});
	connect(m_SpinB, &QSpinBox::valueChanged, this, [this](int value) {
		if(!this->m_EventLock) {
			AbcaQuad abca = this->abcaFromColour(this->m_Colour);
			abca.B = value;
			this->setColour(this->colourFromAbca(abca));
		}
	});
	connect(m_SpinC, &QSpinBox::valueChanged, this, [this](int value) {
		if(!this->m_EventLock) {
			AbcaQuad abca = this->abcaFromColour(this->m_Colour);
			abca.C = value;
			this->setColour(this->colourFromAbca(abca));
		}
	});
	connect(m_SpinAlpha, &QSpinBox::valueChanged, this, [this](int value) {
		if(!this->m_EventLock) {
			AbcaQuad abca = this->abcaFromColour(this->m_Colour);
			abca.Alpha = value;
			this->setColour(this->colourFromAbca(abca));
		}
	});

	this->updateImages(true, true, true);
	this->setColourSelectionModel(ColourSelectionModel::Hsv);
	this->setSliderArrangement(SliderArrangement::Abc);

	QGridLayout* layout = new QGridLayout();
	layout->setSpacing(8);

	QHBoxLayout* modelSelBox = new QHBoxLayout();
	modelSelBox->addWidget(hsvButton);
	modelSelBox->addWidget(hslButton);
	modelSelBox->addWidget(rgbButton);
	modelSelBox->addWidget(cmyButton);

	QHBoxLayout* rowA = new QHBoxLayout();
	rowA->addWidget(abcButton);
	rowA->addWidget(m_SpinA);

	QHBoxLayout* rowB = new QHBoxLayout();
	rowB->addWidget(bacButton);
	rowB->addWidget(m_SpinB);

	QHBoxLayout* rowC = new QHBoxLayout();
	rowC->addWidget(cabButton);
	rowC->addWidget(m_SpinC);

	QHBoxLayout* rowAlpha = new QHBoxLayout();
	rowAlpha->addWidget(new QLabel("Alpha"));
	rowAlpha->addWidget(m_SpinAlpha);

	QVBoxLayout* modelEditorBox = new QVBoxLayout();
	modelEditorBox->addLayout(rowA);
	modelEditorBox->addLayout(rowB);
	modelEditorBox->addLayout(rowC);
	modelEditorBox->addLayout(rowAlpha);

	layout->addLayout(modelSelBox, 0, 0, 1, 0);
	layout->addWidget(m_SquareSlider, 1, 0);
	layout->addWidget(m_PrimarySlider, 1, 1);
	layout->addLayout(modelEditorBox, 1, 2);
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
	m_ArrangementActions->deleteLater();
	m_SelectionModelActions->deleteLater();
}

void ColourSelector::setColour(const QColor& colour) {
	bool alphaDiff = colour.alpha() != m_Colour.alpha();
	bool rgbaDiff = (colour.rgba() != m_Colour.rgba()) || (colour.hue() != m_Colour.hue());

	bool squareDiff = this->slidersFromColour(colour).SquareVal != this->slidersFromColour(m_Colour).SquareVal;
	bool primaryDiff = this->slidersFromColour(colour).PrimaryVal != this->slidersFromColour(m_Colour).PrimaryVal;

	// std::cout << "colour: " << std::hex << colour.rgba() << ", m_Colour: " << std::hex << m_Colour.rgba() << std::endl;

	emit colourChanged(colour);
	this->m_Colour = colour;

	updateImages(primaryDiff, squareDiff, true);
	this->enableEventLock();
	this->updateUi(squareDiff, primaryDiff, alphaDiff, rgbaDiff);
	this->disableEventLock();
	this->update();
}

void ColourSelector::setColourSelectionModel(ColourSelectionModel model) {
	emit colourSelectionModelChanged(model);
	m_SelectionModel = model;

	// QColor is a bit weird sometimes, and equivalent HSL (for example) colours aren't necessarily equivalent in their RGB values,
	// as stored in the QColor, and so fail rgb equivalency asserts.
	// This aims to work around that by updating m_Colour to be set in the way the colour model sets it
	// NOTE: Although now I am actually not sure I need it
	// SliderInfo newSliderInfo = this->slidersFromColour(m_Colour);
	// QColor newColour = this->colourFromSliders(newSliderInfo.SquareVal, newSliderInfo.PrimaryVal, newSliderInfo.AlphaVal);
	// m_Colour = newColour;

	// TODO: Probably put in dedicated function
	switch(m_SelectionModel) {
		case ColourSelectionModel::Hsv: {
			m_AbcAction->setText("Hue");
			m_BacAction->setText("Saturation");
			m_CabAction->setText("Value");

			m_HsvAction->setChecked(true);
			break;
		}
		case ColourSelectionModel::Hsl: {
			m_AbcAction->setText("Hue");
			m_BacAction->setText("Saturation");
			m_CabAction->setText("Lightness");

			m_HslAction->setChecked(true);
			break;
		}
		case ColourSelectionModel::Rgb: {
			m_AbcAction->setText("Red");
			m_BacAction->setText("Green");
			m_CabAction->setText("Blue");

			m_RgbAction->setChecked(true);
			break;
		}
		case ColourSelectionModel::Cmy: {
			m_AbcAction->setText("Cyan");
			m_BacAction->setText("Magenta");
			m_CabAction->setText("Yellow");

			m_CmyAction->setChecked(true);
			break;
		}
	}

	this->enableEventLock(); // Need to capture the updateUiBounds in this event lock as this *can* issue a setValue if the current value is outwith the new bounds
	this->updateUiBounds();
	this->updateImages(true, true, false);
	this->updateUi(true, true, false, false);
	this->disableEventLock();
	this->update();
}

void ColourSelector::setSliderArrangement(SliderArrangement arrangement) {
	emit sliderArrangementChanged(arrangement);
	m_Arrangement = arrangement;

	switch(m_Arrangement) {
		case SliderArrangement::Abc: {
			m_AbcAction->setChecked(true);
			break;
		}
		case SliderArrangement::Bac: {
			m_BacAction->setChecked(true);
			break;
		}
		case SliderArrangement::Cab: {
			m_CabAction->setChecked(true);
			break;
		}
	}

	this->enableEventLock(); // Need to capture the updateUiBounds in this event lock as this *can* issue a setValue if the current value is outwith the new bounds
	this->updateUiBounds();
	this->updateImages(true, true, false);
	this->updateUi(true, true, false, false);
	this->disableEventLock();
	this->update();
}

void ColourSelector::enableEventLock() {
	m_EventLock = true;
}

void ColourSelector::disableEventLock() {
	m_EventLock = false;
}

void ColourSelector::updateUiBounds() {
	int amin = 0;
	int amax = 0;
	int bmin = 0;
	int bmax = 0;
	int cmin = 0;
	int cmax = 0;

	switch(m_SelectionModel) {
		case ColourSelectionModel::Hsv:
		case ColourSelectionModel::Hsl: {
			amax = 359; // NOTE: Since this is maxing out at 359, not 360, does this affect anything else?
			bmax = 255;
			cmax = 255;
			break;
		}
		case ColourSelectionModel::Rgb:
		case ColourSelectionModel::Cmy: {
			amax = 255;
			bmax = 255;
			cmax = 255;
			break;
		}
		default:
			assert(false);
	}

	m_SpinA->setMinimum(amin);
	m_SpinA->setMaximum(amax);
	m_SpinB->setMinimum(bmin);
	m_SpinB->setMaximum(bmax);
	m_SpinC->setMinimum(cmin);
	m_SpinC->setMaximum(cmax);

	switch(m_Arrangement) {
		case SliderArrangement::Abc: {
			m_PrimarySlider->setMinimum(amin);
			m_PrimarySlider->setMaximum(amax);
			m_SquareSlider->setMinimum(QVariant(QPoint(bmin, cmin)));
			m_SquareSlider->setMaximum(QVariant(QPoint(bmax, cmax)));
			break;
		}
		case SliderArrangement::Bac: {
			m_PrimarySlider->setMinimum(bmin);
			m_PrimarySlider->setMaximum(bmax);
			m_SquareSlider->setMinimum(QVariant(QPoint(amin, cmin)));
			m_SquareSlider->setMaximum(QVariant(QPoint(amax, cmax)));
			break;
		}
		case SliderArrangement::Cab: {
			m_PrimarySlider->setMinimum(cmin);
			m_PrimarySlider->setMaximum(cmax);
			m_SquareSlider->setMinimum(QVariant(QPoint(amin, bmin)));
			m_SquareSlider->setMaximum(QVariant(QPoint(amax, bmax)));
			break;
		}
		default:
			assert(false);
	}
}

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
	SliderInfo sliderInfo = this->slidersFromColour(m_Colour);

	if(updateSquareSlider) {
		m_SquareSlider->setValue(sliderInfo.SquareVal);
	}
	if(updatePrimarySlider) {
		m_PrimarySlider->setValue(sliderInfo.PrimaryVal);
	}
	if(updateAlphaSlider) {
		m_AlphaSlider->setValue(sliderInfo.AlphaVal);
	}
	if(updateHex) {
		m_HexEntry->setText(m_Colour.name(QColor::NameFormat::HexArgb).sliced(1).toLower());
	}

	AbcaQuad abca = this->abcaFromColour(m_Colour);

	m_SpinA->setValue(abca.A);
	m_SpinB->setValue(abca.B);
	m_SpinC->setValue(abca.C);
	m_SpinAlpha->setValue(abca.Alpha);

	// This is now causing more trouble than it's worth - E.g. when specifying a colour in hex, in HSL mode, this will crash if the colour doesn't fit into HSL
	// QColor fromSliders = this->colourFromSliders(m_SquareSlider->value(), m_PrimarySlider->value(), m_AlphaSlider->value());
	// assert(fromSliders.rgba() == m_Colour.rgba());
}

void ColourSelector::genSquareImg() {
	QRgb* pixels = (QRgb*)m_SquareImg->scanLine(0);

	for(int i = 0; i < m_SquareImg->width(); i++) {
		for(int j = 0; j < m_SquareImg->height(); j++) {
			int a = utils::map(i, 0, m_SquareImg->width(), m_SquareSlider->minimum().toPoint().x(), m_SquareSlider->maximum().toPoint().x());
			int b = utils::map(j, 0, m_SquareImg->height(), m_SquareSlider->minimum().toPoint().y(), m_SquareSlider->maximum().toPoint().y());

			SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
			QColor col = this->colourFromSliders(QVariant(QPoint(a, b)), sliderInfo.PrimaryVal, 255);
			pixels[i + j * m_SquareImg->width()] = col.rgba();
		}
	}
}

void ColourSelector::genPrimarySliderImg() {
	QRgb* pixels = (QRgb*)m_PrimarySliderImg->scanLine(0);

	for(int j = 0; j < m_PrimarySliderImg->height(); j++) {
		int p = utils::map(j, 0, m_PrimarySliderImg->height(), m_PrimarySlider->minimum(), m_PrimarySlider->maximum());
		SliderInfo sliderInfo = this->slidersFromColour(m_Colour);
		QColor col = this->colourFromSliders(sliderInfo.SquareVal, p, 255);
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

QColor ColourSelector::colourFromSliders(const QVariant& squareVal, int primaryVal, int alphaVal) const {
	AbcaQuad abca = this->abcaFromSliders({
		.SquareVal = squareVal,
		.PrimaryVal = primaryVal,
		.AlphaVal = alphaVal
	});

	return this->colourFromAbca(abca);
}

SliderInfo ColourSelector::slidersFromColour(const QColor& col) const {
	AbcaQuad abca = abcaFromColour(col);

	SliderInfo info = this->slidersFromAbca(abca);

	// This is probably fine to disable... It is currently causing issues because at this point the min/max of the sliders may not have been set correctly
	// for the current colour selection model
	// assert(info.SquareVal.toPoint().x() >= m_SquareSlider->minimum().toPoint().x() && info.SquareVal.toPoint().y() >= m_SquareSlider->minimum().toPoint().y());
	// assert(info.SquareVal.toPoint().x() <= m_SquareSlider->maximum().toPoint().x() && info.SquareVal.toPoint().y() <= m_SquareSlider->maximum().toPoint().y());
	// assert(info.PrimaryVal >= m_PrimarySlider->minimum() && info.PrimaryVal <= m_PrimarySlider->maximum());
	// assert(info.AlphaVal >= m_AlphaSlider->minimum() && info.AlphaVal <= m_AlphaSlider->maximum());

	return info;
}

QColor ColourSelector::colourFromAbca(const AbcaQuad& abca) const {
	switch(m_SelectionModel) {
		case ColourSelectionModel::Hsv: {
			// Qt returns -1 for achromatic colours, such as black - Not usually an issue but can be an issue on startup. This corrects that flaw
			int hue = qMax(abca.A, 0);
			int sat = abca.B;
			int val = abca.C;

			QColor col = QColor::fromHsv(hue, sat, val, abca.Alpha);

			assert(hue == col.hue());
			assert(sat == col.hsvSaturation());
			assert(val == col.value());

			return col;
		}
		case ColourSelectionModel::Hsl: {
			int hue = qMax(abca.A, 0);
			int sat = abca.B;
			int lig = abca.C;

			QColor col = QColor::fromHsl(hue, sat, lig, abca.Alpha);

			assert(hue == qMax(col.hslHue(), 0));
			assert(sat == col.hslSaturation());
			assert(lig == col.lightness());

			return col;
		}
		case ColourSelectionModel::Rgb: {
			int red = abca.A;
			int green = abca.B;
			int blue = abca.C;

			QColor col = QColor(red, green, blue, abca.Alpha);

			assert(red == col.red());
			assert(green == col.green());
			assert(blue == col.blue());

			return col;
		}
		case ColourSelectionModel::Cmy: {
			int red = 255 - abca.A;
			int green = 255 - abca.B;
			int blue = 255 - abca.C;

			QColor col = QColor(red, green, blue, abca.Alpha);

			assert(red == col.red());
			assert(green == col.green());
			assert(blue == col.blue());

			return col;
		}
	}

	assert(false);
	return *(QColor*)nullptr;
}

AbcaQuad ColourSelector::abcaFromColour(const QColor& col) const {
	AbcaQuad abca;

	switch(m_SelectionModel) {
		case ColourSelectionModel::Hsv: {
			abca = {
				.A = qMax(col.hue(), 0),
				.B = col.hsvSaturation(),
				.C = col.value(),
				.Alpha = col.alpha()
			};
			break;
		}
		case ColourSelectionModel::Hsl: {
			abca = {
				.A = qMax(col.hslHue(), 0),
				.B = col.hslSaturation(),
				.C = col.lightness(),
				.Alpha = col.alpha()
			};
			break;
		}
		case ColourSelectionModel::Rgb: {
			abca = {
				.A = qMax(col.red(), 0),
				.B = col.green(),
				.C = col.blue(),
				.Alpha = col.alpha()
			};
			break;
		}
		case ColourSelectionModel::Cmy: {
			abca = {
				.A = 255 - qMax(col.red(), 0),
				.B = 255 - col.green(),
				.C = 255 - col.blue(),
				.Alpha = col.alpha()
			};
			break;
		}
		default:
			assert(false);
	}

	return abca;
}

AbcaQuad ColourSelector::abcaFromSliders(const SliderInfo& info) const {
	int a = 0;
	int b = 0;
	int c = 0;

	switch(m_Arrangement) {
		case SliderArrangement::Abc: {
			a = info.PrimaryVal;
			b = info.SquareVal.toPoint().x();
			c = info.SquareVal.toPoint().y();
			break;
		}
		case SliderArrangement::Bac: {
			a = info.SquareVal.toPoint().x();
			b = info.PrimaryVal;
			c = info.SquareVal.toPoint().y();
			break;
		}
		case SliderArrangement::Cab: {
			a = info.SquareVal.toPoint().x();
			b = info.SquareVal.toPoint().y();
			c = info.PrimaryVal;
			break;
		}
		default:
			assert(false);
	}

	AbcaQuad abca = {
		.A = a,
		.B = b,
		.C = c,
		.Alpha = info.AlphaVal
	};

	SliderInfo verifyInfo = this->slidersFromAbca(abca);
	assert(info.SquareVal.toPoint() == verifyInfo.SquareVal.toPoint());
	assert(info.PrimaryVal == verifyInfo.PrimaryVal);
	assert(info.AlphaVal == verifyInfo.AlphaVal);

	return abca;
}

SliderInfo ColourSelector::slidersFromAbca(const AbcaQuad& abca) const {
	int p = 0;
	int x = 0;
	int y = 0;

	switch(m_Arrangement) {
		case SliderArrangement::Abc: {
			p = abca.A;
			x = abca.B;
			y = abca.C;
			break;
		}
		case SliderArrangement::Bac: {
			p = abca.B;
			x = abca.A;
			y = abca.C;
			break;
		}
		case SliderArrangement::Cab: {
			p = abca.C;
			x = abca.A;
			y = abca.B;
			break;
		}
		default:
			assert(false);
	}

	return {
		.SquareVal = QVariant(QPoint(x, y)),
		.PrimaryVal = p,
		.AlphaVal = abca.Alpha
	};
}
