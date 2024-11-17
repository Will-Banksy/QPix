#include "FlowLayout.h"
#include <QObject>
#include <QWidget>
#include <QLayout>

FlowLayout::FlowLayout(QWidget* parent) : FlowLayout(s_DefaultSpacing, parent) {
}

FlowLayout::FlowLayout(int spacing, QWidget* parent) : FlowLayout(spacing, spacing, parent) {
}

FlowLayout::FlowLayout(int spacingX, int spacingY, QWidget* parent) : QLayout(parent), m_SpacingX(spacingX), m_SpacingY(spacingY) {
}

FlowLayout::~FlowLayout() {
	this->clear();
}

void FlowLayout::clear() {
	QLayoutItem* item;
	while((item = this->takeAt(0))) { // It's not happy unless you use double parenthesis
		// Delete inner items
		if(item->widget() != nullptr) {
			item->widget()->deleteLater();
		}
		if(item->layout() != nullptr) {
			item->layout()->deleteLater();
		}
		if(item->spacerItem() != nullptr) {
			delete item->spacerItem();
		}
		delete item;
	}
}

int FlowLayout::count() const {
	return m_Items.size();
}

QLayoutItem* FlowLayout::takeAt(int index) {
	// QVector::take does not do index checking
	return index >= 0 && index < m_Items.size() ? m_Items.takeAt(index) : nullptr;
}

QLayoutItem* FlowLayout::itemAt(int index) const {
	// QVector::value() does index checking, and returns nullptr if we are outside the valid range
	return m_Items.value(index);
}

void FlowLayout::addItem(QLayoutItem* item) {
	m_Items.append(item);
}

QSize FlowLayout::sizeHint() const {
	return minimumSize();
}

void FlowLayout::setGeometry(const QRect& rect) { // I believe 'rect' is the rectangle that contains the layout
	QLayout::setGeometry(rect);
	// Here is where the actual layouting is done

	this->layoutItems(rect, false);
}

bool FlowLayout::hasHeightForWidth() const {
	return true;
}

int FlowLayout::heightForWidth(int width) const {
	return layoutItems(QRect(0, 0, width, 0));
}

int FlowLayout::layoutItems(const QRect& rect, bool dryRun) const {
	if(m_Items.size() == 0) {
		return 0;
	}

	QVector<QRect> itemBounds;

	QPoint corner = rect.topLeft() + QPoint(contentsMargins().left(), contentsMargins().top()); // Last item's top right-hand corner, or the layout's (0, 0)/top left corner if it's the first one
	corner.setX(corner.x() - m_SpacingX); // So that the first item doesn't have too the initial spacingX added to it
	for(QLayoutItem* item : m_Items) {
		QSize sizeHint = item->sizeHint();
		int x = corner.x();
		int y = corner.y();
		bool onNewLine = (x + sizeHint.width() + contentsMargins().right()) > rect.width();
		if(onNewLine) { // If it turns out we need to arrange ourselves on a new line because there isn't enough horizontal space to fit them all in, then do so
			x = rect.left() + contentsMargins().left();
			// The y-increment - the last item's height or 0 if there wasn't a last one
			int yInc = itemBounds.isEmpty() ? 0 : itemBounds.last().height();
			y = corner.y() + yInc + m_SpacingY; // Add the y-increment and the spacing onto the last item's top right corner y position
		} else {
			x += m_SpacingX; // Add the horizontal spacing onto the right side of the last item layed out - that is the x coordinate of this item, unless we're on a new line
		}
		QRect bounds = QRect(x, y, item->sizeHint().width(), item->sizeHint().height());
		if(!dryRun) { // Obviously if we're doing a dry run (to get the height) then we don't want to position all the m_Items again, that'll be expensive
			item->setGeometry(bounds);
		}
		corner = bounds.topRight();
		itemBounds.append(bounds);
	}
	return corner.y() + itemBounds.last().height(); // Return the height that all the m_Items take up
}

QSize FlowLayout::minimumSize() const {
	// Basically since this hasHeightForWidth, we can't exactly compute a definitive minimumSize, so we'll just return the minimumSize of the biggest item we have plus of course the margins
	QSize size;
	for(QLayoutItem* item : m_Items) {
		size = size.expandedTo(item->minimumSize());
	}
	size += QSize(contentsMargins().left() + contentsMargins().right(), contentsMargins().top() + contentsMargins().bottom());
	return size;
}