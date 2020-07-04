#ifndef PIXELEDITOR_H
#define PIXELEDITOR_H

#include <QMainWindow>
#include <QScopedPointer>

class PixelEditor : public QMainWindow
{
    Q_OBJECT

	public:
		explicit PixelEditor(QWidget *parent = nullptr);
		~PixelEditor() override;
};

#endif // PIXELEDITOR_H
