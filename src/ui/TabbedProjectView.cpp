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