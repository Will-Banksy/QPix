configure-rls:
	mkdir -p target/release
	cd target/release && cmake -DCMAKE_BUILD_TYPE=Release ../..

build-rls: configure-rls
	cd target/release && make

run-rls: build-rls
	./target/release/qpix

configure-dbg:
	mkdir -p target/debug
	cd target/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..

build-dbg: configure-dbg
	cd target/debug && make

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