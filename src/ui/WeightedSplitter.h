#ifndef SPLITTER_H
#define SPLITTER_H

#include <QSplitter>

/**
 * @todo write docs
 */
class WeightedSplitter : public QSplitter {
	public:
		~WeightedSplitter() = default;
		WeightedSplitter(Qt::Orientation orientation, QWidget* parent = nullptr);
		/**
		 * Sets the weights 0 - 1 of child widgets. widgets with weight 0 will not be resized upon the resizing of this WeightedSplitter
		 **/
		void setWeights(QList<float> weights);
		QList<float> weights;
		void autoSizeWidgets();

	protected:
		void resizeEvent(QResizeEvent* event) override;
};

#endif // SPLITTER_H
