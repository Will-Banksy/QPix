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

	for(int i = 0; i < projects.count(); i++) {
		addProject(projects.at(i));
	}

	connect(tabbar, &QTabBar::currentChanged, this, &TabbedProjectView::setCurrentProject);
	connect(tabbar, &QTabBar::tabCloseRequested, this, &TabbedProjectView::handleTabClose);

	QGridLayout* layout = new QGridLayout();
// 	layout->addWidget(tabbar, 0, 0);
	layout->addWidget(view, 1, 0);
	setLayout(layout);

	setCurrentProject(0);
}

void TabbedProjectView::addProject(Project* project) {
	// FIXME Okay so really wierd - tabbar apparently has a really small address like 0x71 or 0x81 at this point, so trying to access the value of it, even to check if it's a nullptr, causes SIGSEGV
	// WHY I do not know - In the constructor it had a normal one, and it's not like this function and the constructor are being called from different threads
	tabbar->addTab(project->name);
	tabbar->setTabData(tabbar->count() - 1, project->id);
}

void TabbedProjectView::closeProject(Project* project) {
	ProjectManager::closeProject(project);
}

void TabbedProjectView::handleTabClose(int tabIndex) {
	Project* project = ProjectManager::fromId(tabbar->tabData(tabIndex).toInt());
	closeProject(project);
}

void TabbedProjectView::setCurrentProject(int tabIndex) {
	Project* project = ProjectManager::fromId(tabbar->tabData(tabIndex).toInt());
	if(project) {
		view->setScene(project->scene);
	} else {
		view->setScene(nullptr);
	}
}

//TabbedProjectView::TabbedProjectView(CanvasView* view, QList<Project*> projects, QWidget* parent) : QWidget(parent), view(view) {
//	tabbar = new QTabBar();
//	tabbar->setTabsClosable(true);
//	tabbar->setMovable(true);
//
//	for(int i = 0; i < projects.count(); i++) {
//		addProject(projects.at(i));
//		addId(i, projects.at(i));
//	}
//
//	connect(tabbar, &QTabBar::currentChanged, this, &TabbedProjectView::setCurrentProject);
//	connect(tabbar, &QTabBar::tabCloseRequested, this, &TabbedProjectView::handleTabClose);
//	connect(tabbar, &QTabBar::tabMoved, [this](int from, int to) {
//		Project* projDest = this->fromId(to);
//		Project* projSrc = this->fromId(from);
//		std::cout << "from: " << from << ", to: " << to << std::endl;
//		// Swap the ids
//		int temp = projDest->tabId;
//		projDest->tabId = projSrc->tabId;
//		projSrc->tabId = temp;
//
//		projectTabIds[from] = projDest;
//		projectTabIds[to] = projSrc;
//	});
//
//	QGridLayout* layout = new QGridLayout();
//	layout->addWidget(tabbar, 0, 0);
//	layout->addWidget(view, 1, 0);
//	setLayout(layout);
//
//	setCurrentProject(0);
//}
//
//TabbedProjectView::~TabbedProjectView() {
//}
//
//void TabbedProjectView::addProject(Project* project) {
//	addId(projects.count(), project);
//	projects.append(project);
//	tabbar->addTab(project->name);
//}
//
//void TabbedProjectView::setCurrentProject(int index) {
//	if(index == -1) {
//		view->setScene(nullptr);
//		return;
//	} else if(index >= projects.count()) {
//		index = projects.count() - 1;
//	}
//	std::cout << "Hello. Index: " << index << std::endl;
//	view->setScene(fromId(index)->scene);
//// 	view->setTransform(fromId(index)->viewTransform); TODO Implement something here
//}
//
//void TabbedProjectView::handleTabClose(int index) {
//	Project* project = fromId(index);
//	if(project->saved) {
//		closeProject(project);
//	} else {
//		QMessageBox::StandardButton reply = QMessageBox::question(window(), tr("Close Unsaved Project?"),
//																  tr("This project contains unsaved work. Are you sure you want to close it?"),
//																  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
//		if(reply == QMessageBox::Yes) {
//			closeProject(project);
//		}
//	}
//}
//
//void TabbedProjectView::closeProject(Project* project) {
//	int i = id(project);
//	removeId(i);
//	tabbar->removeTab(i);
//	projects.removeAll(project);
//}
//
//Project* TabbedProjectView::fromId(int id) {
//	return projectTabIds.value(id);
//}
//
//int TabbedProjectView::id(Project* project) {
//	return project->tabId;
//}
//
//void TabbedProjectView::addId(int id, Project* project) {
//	project->tabId = id;
//	projectTabIds[id] = project;
//}
//
//void TabbedProjectView::removeId(int id) {
//	// FIX THIS
//	for(int i = 0; i < projects.count(); i++) {
//		Project* proj = fromId(i);
//		if(i > id) {
//			addId(i - 1, proj);
//		}
//	}
//	projectTabIds.remove(projects.count() - 1);
//}

// Project* TabbedProjectView::projectRemoved() {
// }
//
// void TabbedProjectView::removeProject(Project* project) {
// }