cmake_minimum_required(VERSION 3.0)
project(PixelEditor)

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
qt5_add_resources(QT_RESOURCES qtres.qrc)

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
	src/ui/panes/flowscrollpane.cpp

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
	src/ui/panes/flowscrollpane.h

	src/tools/tool.h
	src/tools/pencil.h
	src/tools/eraser.h

	src/utils/algorithms.h
	src/utils/helper.h
)

# Tell CMake to create the helloworld executable
add_executable(pixeleditor ${SOURCES} ${QT_RESOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(pixeleditor Qt5::Widgets)

# Install the executable
install(TARGETS pixeleditor DESTINATION bin)
