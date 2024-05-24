# Minimum required version of cmake to build this project
cmake_minimum_required(VERSION 3.5)

# Set project name and version, and languages used
project(QPix VERSION 1.0 LANGUAGES CXX)

# Instruct CMake to run moc (Qt's Meta Object Compiler) automatically when needed
set(CMAKE_AUTOMOC ON)

# Use c++20, -g for including debugging symbols, -O0 for 0 optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++20")

# Find the QtWidgets library
find_package(Qt6Widgets CONFIG REQUIRED)

# Find the Qt6Core library
find_package(Qt6Core CONFIG REQUIRED)

# Add the resources
qt6_add_resources(QT_RESOURCES resources.qrc)

qt6_standard_project_setup()

set(pixeleditor_CPP
	src/main.cpp
	src/EditorTools.cpp
	src/Project.cpp
	src/Application.cpp
	src/ProjectManager.cpp
	src/Icons.cpp

	src/model/AppModel.cpp
	src/model/ProjectModel.cpp

	src/model/tools/AbstractTool.cpp
	src/model/tools/PencilTool.cpp
	src/model/tools/BucketTool.cpp

	src/ui/Window.cpp
	src/ui/UI.cpp
	src/ui/Canvas.cpp
	src/ui/FlowLayout2.cpp
	src/ui/ProjectScene.cpp
	src/ui/CanvasView.cpp
	src/ui/TabbedProjectView.cpp

	src/ui/widgets/ToolButton.cpp

	src/ui/components/AppView.cpp
	src/ui/components/ProjectView.cpp

	src/ui/components/canvas/ProjectCanvasView.cpp

	src/ui/components/status/StatusZoomView.cpp

	src/ui/components/toolbars/ToolSelectView.cpp

	src/ui/components/floating/FloatingView.cpp
	src/ui/components/floating/FloatingInfoView.cpp
	src/ui/components/floating/HoverInfoEventFilter.cpp

	src/tools/ToolOptionWidget.cpp
	src/tools/Tool.cpp
	src/tools/Pencil.cpp
	src/tools/Eraser.cpp
	src/tools/Fill.cpp
	src/tools/Line.cpp
	src/tools/ScrollHand.cpp
	src/tools/RectangularSelect.cpp

	src/utils/Painter.cpp
	src/utils/Utils.cpp
	src/utils/Colour.cpp
	src/utils/Brush.cpp
	src/utils/BrushMatrix.cpp
	src/utils/PaintUtils.cpp
)

set(SOURCES
	${pixeleditor_CPP}

	src/EditorTools.h
	src/Project.h
	src/Application.h
	src/ProjectManager.h
	src/Icons.h

	src/model/AppModel.h
	src/model/ProjectModel.h

	src/model/tools/AbstractTool.h
	src/model/tools/PencilTool.h
	src/model/tools/BucketTool.h

	src/ui/Window.h
	src/ui/UI.h
	src/ui/Canvas.h
	src/ui/FlowLayout2.h
	src/ui/ProjectScene.h
	src/ui/CanvasView.h
	src/ui/TabbedProjectView.h

	src/ui/widgets/ToolButton.h

	src/ui/components/AppView.h
	src/ui/components/ProjectView.h

	src/ui/components/canvas/ProjectCanvasView.h

	src/ui/components/status/StatusZoomView.h

	src/ui/components/toolbars/ToolSelectView.h

	src/ui/components/floating/FloatingView.h
	src/ui/components/floating/FloatingInfoView.h
	src/ui/components/floating/HoverInfoEventFilter.h

	src/tools/ToolOptionWidget.h
	src/tools/Tool.h
	src/tools/Pencil.h
	src/tools/Eraser.h
	src/tools/Fill.h
	src/tools/Line.h
	src/tools/ScrollHand.h
	src/tools/RectangularSelect.h

	src/utils/Painter.h
	src/utils/Utils.h
	src/utils/Colour.h
	src/utils/Brush.h
	src/utils/BrushMatrix.h
	src/utils/Nullable.h
	src/utils/PaintUtils.h
)

# Tell CMake to create the executable qpix
add_executable(qpix ${SOURCES} ${QT_RESOURCES})

# So I don't have to include the src in #include <header> - includes that directory for target qpix
target_include_directories(qpix PRIVATE src)

# Use the Widgets module from Qt 5 - do it for target qpix
target_link_libraries(qpix Qt6::Widgets)

# Install the executable
install(TARGETS qpix DESTINATION bin)