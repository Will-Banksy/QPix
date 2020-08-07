# Minimum required version of cmake to build this project
cmake_minimum_required(VERSION 3.1)

# Set project name and version, and languages used
project(QPix VERSION 1.0 LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
# set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Find the Qt5Core library
find_package(Qt5Core)

# Add the resources
qt5_add_resources(QT_RESOURCES resources.qrc)

set(pixeleditor_CPP
	src/main.cpp
	src/editortools.cpp

	src/ui/pixeleditor.cpp
	src/ui/canvas.cpp
	src/ui/weightedsplitter.cpp
	src/ui/flowlayout.cpp

	src/ui/panes/contentpane.cpp
	src/ui/panes/canvaspane.cpp
	src/ui/panes/toolpane.cpp

	src/tools/tool.cpp
	src/tools/pencil.cpp
	src/tools/eraser.cpp

	src/utils/algorithms.cpp
	src/utils/helper.cpp
)

set(SOURCES
	${pixeleditor_CPP}

	src/editortools.h

	src/ui/pixeleditor.h
	src/ui/canvas.h
	src/ui/weightedsplitter.h
	src/ui/flowlayout.h

	src/ui/panes/contentpane.h
	src/ui/panes/canvaspane.h
	src/ui/panes/toolpane.h

	src/tools/tool.h
	src/tools/pencil.h
	src/tools/eraser.h

	src/utils/algorithms.h
	src/utils/helper.h
)

# Tell CMake to create the executable
add_executable(qpix ${SOURCES} ${QT_RESOURCES})

# Use the Widgets module from Qt 5
target_link_libraries(qpix Qt5::Widgets)

# Install the executable
install(TARGETS qpix DESTINATION bin)
