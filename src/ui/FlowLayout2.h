#ifndef FLOWLAYOUT2_H
#define FLOWLAYOUT2_H

#include <QLayout>

/**
 * @todo write docs
 */
class FlowLayout2 : public QLayout {
	public:
		FlowLayout2(QWidget* parent = nullptr);
		FlowLayout2(int spacing, QWidget* parent = nullptr);
		FlowLayout2(int spacingX, int spacingY, QWidget* parent = nullptr);
		~FlowLayout2();

		int count() const override;
		QLayoutItem* takeAt(int index) override;
		QLayoutItem* itemAt(int index) const override;
		void addItem(QLayoutItem* item) override;
		QSize sizeHint() const override;
		void setGeometry(const QRect& rect) override;
		QSize minimumSize() const override;
		bool hasHeightForWidth() const override;
		int heightForWidth(int) const override;
		Qt::Orientations expandingDirections() const override { return { }; }; // The directions that the layout can expand

		/// Lays out the items, then returns the height it took to do so
		int layoutItems(const QRect& rect, bool dryRun = true) const;

		int horizontalSpacing() { return spacingX; };
		int verticalSpacing() { return spacingY; };
		void setHorizontalSpacing(int spacing) { spacingX = spacing; };
		void setVerticalSpacing(int spacing) { spacingY = spacing; };

	private:
		static const int defaultSpacing = 5;

		QVector<QLayoutItem*> items;
		int spacingX;
		int spacingY;

		QSize cachedSize;
		bool cacheDirty; // Whether the cached size needs to be updated
};

#endif // FLOWLAYOUT2_H
