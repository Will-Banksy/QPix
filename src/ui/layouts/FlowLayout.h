#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>

// Based on: https://doc.qt.io/qt-6/qtwidgets-layouts-flowlayout-example.html
class FlowLayout : public QLayout { // TODO: Get this layout working nicely, and actually apply margins properly
public:
	FlowLayout(QWidget* parent = nullptr);
	FlowLayout(int spacing, QWidget* parent = nullptr);
	FlowLayout(int spacingX, int spacingY, QWidget* parent = nullptr);
	~FlowLayout();

	void clear();
	void addItem(QLayoutItem* item) override;

protected:
	int count() const override;
	QLayoutItem* takeAt(int index) override;
	QLayoutItem* itemAt(int index) const override;
	QSize sizeHint() const override;
	void setGeometry(const QRect& rect) override;
	QSize minimumSize() const override;
	bool hasHeightForWidth() const override;
	int heightForWidth(int) const override;
	Qt::Orientations expandingDirections() const override { return { }; }; // The directions that the layout can expand

private:
	static const int s_DefaultSpacing = 5; // TODO: Maybe change this to external const

	QVector<QLayoutItem*> m_Items;
	int m_SpacingX;
	int m_SpacingY;

	/// Lays out the items, then returns the height it took to do so
	int layoutItems(const QRect& rect, bool dryRun = true) const;

	int horizontalSpacing() { return m_SpacingX; };
	int verticalSpacing() { return m_SpacingY; };
	void setHorizontalSpacing(int spacing) { m_SpacingX = spacing; };
	void setVerticalSpacing(int spacing) { m_SpacingY = spacing; };
};

#endif // FLOWLAYOUT_H
