#include "FloatingView.h"
#include "model/AppModel.h"
#include <QStackedLayout>
#include "FloatingInfoView.h"
#include <iostream>
#include <QVBoxLayout>
#include "utils/Utils.h"

FloatingView::FloatingView(QWidget* parent, AppModel* model) : QWidget(parent), m_InfoView(new FloatingInfoView()) {
	this->setVisible(false);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);
	this->setObjectName("floating");

	// this->setContentsMargins(4, 4, 4, 4); // Hard coding this for now

	QStackedLayout* layout = new QStackedLayout();

	layout->addWidget(m_InfoView);

	this->setLayout(layout);

	connect(model, &AppModel::floatingInfoRequested, this, &FloatingView::showFloatingInfo);
	connect(model, &AppModel::hideFloating, this, &FloatingView::hide);
}

FloatingView::~FloatingView() {
}

void FloatingView::showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position) {
	std::cerr << "showFloatingInfo" << std::endl;

	if(this->window() != src->window()) { // Can't display on other windows
		return;
	}

	this->setVisible(true);

	m_InfoView->setTitle(title);
	m_InfoView->setBody(body);
	m_InfoView->updateGeometry();

	((QStackedLayout*)this->layout())->setCurrentWidget(m_InfoView);

	this->adjustSize();

	this->reposition(src, position);

	this->update();
}

void FloatingView::showColourSelect(QWidget* src, QColor current) {
}

void FloatingView::hide() {
	this->setVisible(false);
}

void FloatingView::reposition(QWidget* src, FloatingPosition position) {
	const QPoint gap = QPoint(8, 8);

	QPointF srcPos = src->mapTo(src->window(), QPoint(0, 0));
	QSize srcSize = src->size();

	int newX = srcPos.x() + (srcSize.width() / 2) - this->width() / 2;
	int newY = srcPos.y() + (srcSize.height() / 2) - this->height() / 2;

	QPointF newPos = QPoint(newX, newY);

	QPoint toRight = newPos.toPoint() + QPoint(srcSize.width() / 2, 0) + QPoint(this->width() / 2, 0) + QPoint(gap.x(), 0);
	QRect toRightRect = QRect(toRight, this->size());
	QPoint toLeft = newPos.toPoint() - QPoint(srcSize.width() / 2, 0) - QPoint(this->width() / 2, 0) - QPoint(gap.x(), 0);
	QRect toLeftRect = QRect(toLeft, this->size());
	QPoint toBottom = newPos.toPoint() + QPoint(0, srcSize.height() / 2) + QPoint(0, this->height() / 2) + QPoint(0, gap.y());
	QRect toBottomRect = QRect(toBottom, this->size());
	QPoint toTop = newPos.toPoint() - QPoint(0, srcSize.height() / 2) - QPoint(0, this->height() / 2) - QPoint(0, gap.y());
	QRect toTopRect = QRect(toTop, this->size());

	QRect outerRect = QRect(0, 0, this->window()->width(), this->window()->height());

	switch(position) {
		case FloatingPosition::Right: {
			QRect adjusted = utils::adjustedToWithin(outerRect, toRightRect);
			this->move(adjusted.topLeft());
			break;
		}
		case FloatingPosition::Left: {
			QRect adjusted = utils::adjustedToWithin(outerRect, toLeftRect);
			this->move(adjusted.topLeft());
			break;
		}
		case FloatingPosition::Bottom: {
			QRect adjusted = utils::adjustedToWithin(outerRect, toBottomRect);
			this->move(adjusted.topLeft());
			break;
		}
		case FloatingPosition::Top: {
			QRect adjusted = utils::adjustedToWithin(outerRect, toTopRect);
			this->move(adjusted.topLeft());
			break;
		}
		case FloatingPosition::Unspecified: {
			int toRightIntersectionArea = utils::area(outerRect.intersected(toRightRect));
			int toLeftIntersectionArea = utils::area(outerRect.intersected(toLeftRect));
			int toBottomIntersectionArea = utils::area(outerRect.intersected(toBottomRect));
			int toTopIntersectionArea = utils::area(outerRect.intersected(toTopRect));

			int minArea = std::max({
				toRightIntersectionArea,
				toLeftIntersectionArea,
				toBottomIntersectionArea,
				toTopIntersectionArea
			});

			QRect rect = QRect();
			if(minArea == toRightIntersectionArea) {
				rect = toRightRect;
			} else if(minArea == toLeftIntersectionArea) {
				rect = toLeftRect;
			} else if(minArea == toBottomIntersectionArea) {
				rect = toBottomRect;
			} else if(minArea == toTopIntersectionArea) {
				rect = toTopRect;
			}

			QRect adjusted = utils::adjustedToWithin(outerRect, rect);

			this->move(adjusted.topLeft());
			break;
		}
	}
}
