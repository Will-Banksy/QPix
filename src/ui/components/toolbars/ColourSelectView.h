#ifndef COLOURSELECTVIEW_H
#define COLOURSELECTVIEW_H

#include <QWidget>

class AppModel;

class ColourSelectView : public QWidget {
	Q_OBJECT

public:
	explicit ColourSelectView(AppModel* model);
	~ColourSelectView() override;

private:
	AppModel* m_Model;
};

#endif // COLOURSELECTVIEW_H