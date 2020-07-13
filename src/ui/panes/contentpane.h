#ifndef CONTENTPANE_H
#define CONTENTPANE_H

#include <QWidget>
#include <QMdiArea>

/**
 * @todo write docs
 */
class ContentPane : public QWidget {
	public:
		explicit ContentPane(QWidget* parent = nullptr);
		~ContentPane();
};

#endif // CONTENTPANE_H
