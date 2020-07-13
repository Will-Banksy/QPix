#include "weightedsplitter.h"
#include <QResizeEvent>
#include <iostream>
#include "src/utils/helper.h"

WeightedSplitter::WeightedSplitter(Qt::Orientation orientation, QWidget* parent) : QSplitter(orientation, parent) {
	weights = QList<float>();
}

void WeightedSplitter::setWeights(QList<float> weights) {
	this->weights = weights;
}

void WeightedSplitter::autoSizeWidgets() {
	QList<int>* sizes = new QList<int>();
	for(int i = 0; i < count(); i++) {
		QWidget* widget = this->widget(i);
		sizes->append(widget->sizeHint().width());
	}
	setSizes(*sizes);
}


void WeightedSplitter::resizeEvent(QResizeEvent* event) {
	// The first resizeEvent is -1 for height and width, because it was invisible before.
	if(event->oldSize().height() != -1 && event->oldSize().width() != -1) {
		if(weights.length() != sizes().length()) {
			std::cerr << "Nonfatal Error: weights list length is different to widgets list length";
			QSplitter::resizeEvent(event);
			return;
		}

		QSize prevSize = event->oldSize();
		QSize newSize = event->size();
		QList<int> prevSizes = sizes();
		QSplitter::resizeEvent(event);

		QList<int> newSizes = QList<int>(prevSizes);

		QSize diff = newSize - prevSize;

		auto sum = [](QList<float> list) {
			float s = list.at(0);
			for(int i = 1; i < list.length(); i++) {
				s += list.at(i);
			}
			return s;
		};
		float totalWeight = sum(weights);

		for(int i = 0; i < prevSizes.length(); i++) {
			if(orientation() == Qt::Horizontal) {
				int expansionW = qRound(utils::map(weights.at(i), 0, totalWeight, 0, diff.width()));
				newSizes[i] += expansionW;
			} else if(orientation() == Qt::Vertical) {
				int expansionH = qRound(utils::map(weights.at(i), 0, totalWeight, 0, diff.height()));
				newSizes[i] += expansionH;
			}
		}

		setSizes(newSizes);
	} else {
		QSplitter::resizeEvent(event);
	}
}
