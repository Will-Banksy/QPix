# Minimum required version of cmake to build this project
cmake_minimum_required(VERSION 3.1)

# Set project name and version, and languages used
project(QPix VERSION 1.0 LANGUAGES CXX)

# Instruct CMake to run moc (Meta Object Compiler) automatically when needed
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Find the Qt5Core library
find_package(Qt5Core)

# Add the resources
qt5_add_resources(QT_RESOURCES resources.qrc)

set(pixeleditor_CPP
	src/main.cpp
	src/EditorTools.cpp

	src/ui/PixelEditor.cpp
	src/ui/Canvas.cpp
	src/ui/FlowLayout2.cpp

	src/ui/panes/CanvasPane.cpp
	src/ui/panes/ToolPane.cpp

	src/ui/widgets/ToolButton.cpp

	src/tools/Tool.cpp
	src/tools/Pencil.cpp
	src/tools/Eraser.cpp
	src/tools/Fill.cpp
	src/tools/Line.cpp

	src/utils/Algorithms.cpp
	src/utils/Helper.cpp
	src/utils/Colour.cpp
	src/utils/Brush.cpp
	src/utils/BrushMatrix.cpp
)

set(SOURCES
	${pixeleditor_CPP}

	src/EditorTools.h

	src/ui/PixelEditor.h
	src/ui/Canvas.h
	src/ui/FlowLayout2.h

	src/ui/panes/CanvasPane.h
	src/ui/panes/ToolPane.h

	src/ui/widgets/ToolButton.h

	src/tools/Tool.h
	src/tools/Pencil.h
	src/tools/Eraser.h
	src/tools/Fill.h
	src/tools/Line.h

	src/utils/Algorithms.h
	src/utils/Helper.h
	src/utils/Colour.h
	src/utils/Brush.h
	src/utils/BrushMatrix.h
)

# Tell CMake to create the executable qpix
add_executable(qpix ${SOURCES} ${QT_RESOURCES})

# So I don't have to include the src in #include <header> - includes that directory for target qpix
target_include_directories(qpix PRIVATE src)

# Use the Widgets module from Qt 5 - do it for target qpix
target_link_libraries(qpix Qt5::Widgets)

# Install the executable
install(TARGETS qpix DESTINATION bin)