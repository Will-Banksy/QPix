build-dbg: configure-dbg
	mkdir -p target/debug
	cd target/debug && make

configure-dbg:
	mkdir -p target/debug
	cd target/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..

run-dbg: build-dbg
	QT_QPA_PLATFORM=xcb ./target/debug/qpix

debug: build-dbg
	QT_QPA_PLATFORM=xcb lldb target/debug/qpix

# Is this small script horribly inefficient? Yes. Is it useful? Also yes
update-cmake-sources:
	sed -i 's|set(SOURCES_CPP.*|set(SOURCES_CPP)|' CMakeLists.txt
	sed -i 's|set(SOURCES_H.*|set(SOURCES_H)|' CMakeLists.txt

	find src -type f -name "*.cpp" | sort | xargs -I % sed -i 's|set(SOURCES_CPP|set(SOURCES_CPP %|' CMakeLists.txt
	find src -type f -name "*.h" | sort | xargs -I % sed -i 's|set(SOURCES_H|set(SOURCES_H %|' CMakeLists.txt