#include "pixeleditor.h"
#include "ui_pixeleditor.h"

PixelEditor::PixelEditor(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::PixelEditor)
{
    m_ui->setupUi(this);
}

PixelEditor::~PixelEditor() = default;
