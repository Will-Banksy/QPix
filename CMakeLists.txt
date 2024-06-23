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

set(SOURCES_CPP
	src/main.cpp


	src/model/AppModel.cpp
	src/model/ProjectModel.cpp


	src/model/tools/AbstractTool.cpp
	src/model/tools/PencilTool.cpp
	src/model/tools/BucketTool.cpp
	src/model/tools/HandTool.cpp


	src/ui/layouts/FlowLayout.cpp


	src/ui/widgets/ColourButton.cpp
	src/ui/widgets/ColourSelector.cpp
	src/ui/widgets/ColourSlider.cpp


	src/ui/components/AppView.cpp
	src/ui/components/ProjectView.cpp

	src/ui/components/canvas/ProjectCanvasView.cpp

	src/ui/components/status/StatusZoomView.cpp

	src/ui/components/toolbars/ToolSelectView.cpp
	src/ui/components/toolbars/ColourSelectView.cpp

	src/ui/components/floating/FloatingView.cpp

	src/ui/components/floating/tooltip/FloatingTooltipView.cpp
	src/ui/components/floating/tooltip/FloatingInfoView.cpp
	src/ui/components/floating/tooltip/HoverInfoEventFilter.cpp

	src/ui/components/floating/modal/FloatingModalView.cpp


	src/utils/Utils.cpp
	src/utils/PaintUtils.cpp
)

set(SOURCES_H
	${SOURCES_CPP}

	src/model/AppModel.h
	src/model/ProjectModel.h


	src/model/tools/AbstractTool.h
	src/model/tools/PencilTool.h
	src/model/tools/BucketTool.h
	src/model/tools/HandTool.h


	src/ui/layouts/FlowLayout.h


	src/ui/widgets/ColourButton.h
	src/ui/widgets/ColourSelector.h
	src/ui/widgets/ColourSlider.h


	src/ui/components/AppView.h
	src/ui/components/ProjectView.h

	src/ui/components/canvas/ProjectCanvasView.h

	src/ui/components/status/StatusZoomView.h

	src/ui/components/toolbars/ToolSelectView.h
	src/ui/components/toolbars/ColourSelectView.h

	src/ui/components/floating/FloatingView.h

	src/ui/components/floating/tooltip/FloatingTooltipView.h
	src/ui/components/floating/tooltip/FloatingInfoView.h
	src/ui/components/floating/tooltip/HoverInfoEventFilter.h

	src/ui/components/floating/modal/FloatingModalView.h


	src/utils/Utils.h
	src/utils/Nullable.h
	src/utils/PaintUtils.h
)

set(SOURCES
	${SOURCES_CPP}
	${SOURCES_H}
)

# Tell CMake to create the executable qpix
add_executable(qpix ${SOURCES} ${QT_RESOURCES})

# So I don't have to include the src in #include <header> - includes that directory for target qpix
target_include_directories(qpix PRIVATE src)

# Use the Widgets module from Qt 5 - do it for target qpix
target_link_libraries(qpix Qt6::Widgets)

# Install the executable
install(TARGETS qpix DESTINATION bin)