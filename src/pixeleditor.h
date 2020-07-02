#ifndef PIXELEDITOR_H
#define PIXELEDITOR_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class PixelEditor;
}

class PixelEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit PixelEditor(QWidget *parent = nullptr);
    ~PixelEditor() override;

private:
    QScopedPointer<Ui::PixelEditor> m_ui;
};

#endif // PIXELEDITOR_H
