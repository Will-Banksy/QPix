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

You need Qt6 (I'm using Qt 6.6.2 but it should work on earlier versions), CMake >3.20 and Ninja.

You will also need a C++20 compliant compiler.

To build the project using CMake, run:

```bash
cmake --build --preset=<debug|release>
```

The executable will be output as `target/(debug|release)/qpix(.exe)?` (depending on configuration & platform).