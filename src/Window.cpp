#include "Window.h"
#include "Application.h"

Window::Window() {
	ui = new UI(this);
}

Window::~Window() {
	delete ui;
}

void Window::removeThis() {
	Application::closeWindow(this);
}

void Window::closeEvent(QCloseEvent* event) {
	removeThis();
}
