#include "TabbedProjectView.h"
#include "CanvasView.h"
#include <QTabBar>
#include <QGridLayout>
#include <iostream>
#include <QMessageBox>
#include "ProjectManager.h"

TabbedProjectView::TabbedProjectView(CanvasView* view, QList<Project*> projects, QWidget* parent) : QWidget(parent), view(view) {
	tabbar = new QTabBar();
	tabbar->setTabsClosable(true);
	tabbar->setMovable(true);

	connect(tabbar, &QTabBar::currentChanged, this, &TabbedProjectView::setCurrentProject);
	connect(tabbar, &QTabBar::tabCloseRequested, this, &TabbedProjectView::handleTabClose);

	for(int i = 0; i < projects.count(); i++) {
		registerNewProject(projects.at(i));
	}

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(tabbar, 0, 0);
	layout->addWidget(view, 1, 0);
	setLayout(layout);
}

void TabbedProjectView::registerNewProject(Project* project) {
	int tabIndex = tabbar->addTab(project->name);
	tabbar->setTabData(tabIndex, project->id);
	if(tabbar->count() == 1) {
		setCurrentProject(0);
	}
}

void TabbedProjectView::registerCloseProject(Project* project, int tabIndex) {
	if(tabIndex != -1) {
		tabbar->removeTab(tabIndex);
		return;
	}

	for(int i = 0; i < tabbar->count(); i++) {
		int projId = tabbar->tabData(i).toInt();
		if(projId == project->id) {
			tabbar->removeTab(i);
		}
	}
}

void TabbedProjectView::handleTabClose(int tabIndex) {
	Project* project = ProjectManager::fromId(tabbar->tabData(tabIndex).toInt());
	if(project->saved) {
		project->close(tabIndex);
	} else {
		QMessageBox::StandardButton reply = QMessageBox::warning(window(), tr("Save Project - QPix"),
																  tr("This project contains unsaved work. Do you want to save it?"),
																  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if(reply == QMessageBox::Yes) {
			project->save();
			project->close(tabIndex);
		} else if(reply == QMessageBox::No) {
			project->close(tabIndex);
		}
	}
}

void TabbedProjectView::setCurrentProject(int tabIndex) {
	if(tabbar->currentIndex() != tabIndex) {
		tabbar->setCurrentIndex(tabIndex);
	}
	if(tabIndex == -1) {
		view->setScene(nullptr);
		return;
	}
	Project* project = ProjectManager::fromId(tabbar->tabData(tabIndex).toInt());
	if(project) {
		view->setScene(project->scene);
		view->setTransform(project->viewTransform);
		view->scaleAmt = project->viewScaleAmt;
	} else {
		view->setScene(nullptr);
	}
}

QList<Project*> TabbedProjectView::getProjects() {
	QList<Project*> projects;
	for(int i = 0; i < tabbar->count(); i++) {
		projects.append(ProjectManager::fromId(tabbar->tabData(i).toInt()));
	}
	return projects;
}

void TabbedProjectView::closeAllProjects() {
	for(int i = 0; i < tabbar->count();) {
		ProjectManager::fromId(tabbar->tabData(i).toInt())->close();
	}
}