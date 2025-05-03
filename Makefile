# Is this small script horribly inefficient? Yes. Is it useful? Also yes
update-cmake-sources:
	sed -i 's|set(SOURCES_CPP.*|set(SOURCES_CPP)|' CMakeLists.txt
	sed -i 's|set(SOURCES_H.*|set(SOURCES_H)|' CMakeLists.txt

	find src -type f -name "*.cpp" | sort | xargs -I % sed -i 's|set(SOURCES_CPP|set(SOURCES_CPP %|' CMakeLists.txt
	find src -type f -name "*.h" | sort | xargs -I % sed -i 's|set(SOURCES_H|set(SOURCES_H %|' CMakeLists.txt