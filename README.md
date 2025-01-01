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

You need Qt6 and CMake 3.20 or higher (I'm using Qt 6.6.2 but it should work on earlier versions)

You will also need a C++20 compliant compiler.

To build and run the release build, navigate to project directory and run:

```bash
make run-rls
```
You can also make the `run-dbg` rule to build and run the debug build.

The executable will be output as target/debug/qpix(.exe) or target/release/qpix(.exe) depending on version built.
