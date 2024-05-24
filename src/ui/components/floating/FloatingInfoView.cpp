#include "FloatingInfoView.h"
#include <QVBoxLayout>
#include <QLabel>

FloatingInfoView::FloatingInfoView() : QWidget(), m_Title(new QLabel()), m_Body(new QLabel()) {
	QVBoxLayout* layout = new QVBoxLayout();

	layout->addWidget(m_Title);
	layout->addWidget(m_Body);

	this->setLayout(layout);
}

FloatingInfoView::~FloatingInfoView() {
}

void FloatingInfoView::setTitle(const QString& text) {
	m_Title->setText(text);
}

void FloatingInfoView::setBody(const QString& text) {
	m_Body->setText(text);
}
