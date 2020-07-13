#ifndef FLOWSCROLLPANE_H
#define FLOWSCROLLPANE_H

#include <QScrollArea>

/**
 * @todo write docs
 */
class FlowScrollPane : public QScrollArea {
	public:
		FlowScrollPane(QWidget* parent = nullptr);
		~FlowScrollPane();

		void addWidget(QWidget* widget);
		void removeWidget(QWidget* widget);

		void setMargins(QMargins margins);
		QMargins getMargins();

		void recalculateMinWidth();

	protected:
		void resizeEvent(QResizeEvent* event) override;

	private:
		QList<QWidget*> children;
		QMargins margins;
};

#endif // FLOWSCROLLPANE_H