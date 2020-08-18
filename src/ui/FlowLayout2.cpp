#include "FlowLayout2.h"

FlowLayout2::FlowLayout2(QWidget* parent) : FlowLayout2(defaultSpacing, parent) {
}

FlowLayout2::FlowLayout2(int spacing, QWidget* parent) : FlowLayout2(spacing, spacing, parent) {
}

FlowLayout2::FlowLayout2(int spacingX, int spacingY, QWidget* parent) : QLayout(parent), spacingX(spacingX), spacingY(spacingY) {
}


FlowLayout2::~FlowLayout2() {
	// So I think anything that inherits from QObject gets automatically deleted?
	// But anyway, QLayoutItem doesn't inherit from QObject so we need to manually delete it
	QLayoutItem* item;
	while((item = takeAt(0))) { // It's not happy unless you use double parenthesis
		delete item;
	}
}

int FlowLayout2::count() const {
	return items.size();
}

QLayoutItem* FlowLayout2::takeAt(int index) {
	// QVector::take does not do index checking
	return index >= 0 && index < items.size() ? items.takeAt(index) : nullptr;
}

QLayoutItem* FlowLayout2::itemAt(int index) const {
	// QVector::value() does index checking, and returns nullptr if we are outside the valid range
	return items.value(index);
}

void FlowLayout2::addItem(QLayoutItem* item) {
	items.append(item);
}

QSize FlowLayout2::sizeHint() const {
	return minimumSize();
}

void FlowLayout2::setGeometry(const QRect& rect) { // I believe 'rect' is the rectangle that contains the layout
	QLayout::setGeometry(rect);
	// Here is where the actual layouting is done

	layoutItems(rect, false);
}

bool FlowLayout2::hasHeightForWidth() const {
	return true;
}

int FlowLayout2::heightForWidth(int width) const {
	return layoutItems(QRect(0, 0, width, 0));
}

int FlowLayout2::layoutItems(const QRect& rect, bool dryRun) const {
	QVector<QRect> itemBounds;

	QPoint corner = rect.topLeft(); // Last item's top right-hand corner, or the layout's (0, 0)/top left corner if it's the first one
	corner.setX(corner.x() - spacingX); // So that the first item doesn't have too the initial spacingX added to it
	for(QLayoutItem* item : items) {
		QSize sizeHint = item->sizeHint();
		int x = corner.x();
		int y = corner.y();
		bool onNewLine = (x + sizeHint.width()) > rect.width();
		if(onNewLine) { // If it turns out we need to arrange ourselves on a new line because there isn't enough horizontal space to fit them all in, then do so
			x = rect.left();
			// The y-increment - the last item's height or 0 if there wasn't a last one
			int yInc = itemBounds.isEmpty() ? 0 : itemBounds.last().height();
			y = corner.y() + yInc + spacingY; // Add the y-increment and the spacing onto the last item's top right corner y position
		} else {
			x += spacingX; // Add the horizontal spacing onto the right side of the last item layed out - that is the x coordinate of this item, unless we're on a new line
		}
		QRect bounds = QRect(x, y, item->sizeHint().width(), item->sizeHint().height());
		if(!dryRun) { // Obviously if we're doing a dry run (to get the height) then we don't want to position all the items again, that'll be expensive
			item->setGeometry(bounds);
		}
		corner = bounds.topRight();
		itemBounds.append(bounds);
	}
	return corner.y() + itemBounds.last().height(); // Return the height that all the items take up
}

QSize FlowLayout2::minimumSize() const {
	// Basically since this hasHeightForWidth, we can't exactly compute a definitive minimumSize, so we'll just return the minimumSize of the biggest item we have plus of course the margins
	QSize size;
	for(QLayoutItem* item : items) {
		size = size.expandedTo(item->minimumSize());
	}
	size += QSize(contentsMargins().left() + contentsMargins().right(), contentsMargins().top() + contentsMargins().bottom());
	return size;
}