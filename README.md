# QPix

--- WIP, very incomplete ---

A simple little pixel art editor, using Qt Widgets. Focusing on performance and UI/UX.

![Screenshot](screenshots/screenshot_latest.png)

## Features

Note that since this is very incomplete, these features will likely change.

- Fast CPU-rasterized scrollable & zoomable canvas (works similar to Aseprite)
	- May change to being rendered fully on the GPU in the future but CPU works fine for now
- Featureful colour picker
	- Inspired by Java Swing's colour picker dialog
- Nice looking handcrafted UI (imo) with a lot of care taken to make it coherent

## Building

You need Qt6 (I'm using Qt 6.6.2 but it should work on earlier versions), CMake >3.20, GNU Make and Ninja (latter two not necessarily required, see below).

You will also need a C++20 compliant compiler.

### Using Helper Makefile

To build and run the release build using the simple helper [Makefile](Makefile), navigate to project directory and run:

```bash
make run-rls
```

You can also make the `run-dbg` rule to build and run the debug build.

The executable will be output as target/debug/qpix(.exe) or target/release/qpix(.exe) depending on version built.

### With CMake Directly

To build the project using CMake directly (possibly removing dependency on GNU Make and/or Ninja), run:

```bash
mkdir build && cd build
cmake --cmake-options ..
```

...followed by the command to build the project using whichever generator you got passed to CMake (or default) which is probably either `make` or `ninja`.

The executable will be output as build/qpix(.exe).