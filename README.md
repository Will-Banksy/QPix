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

## Compile And Run

You need Qt6 and CMake 3.5 or higher (I'm using Qt 6.6.2 but it should work on earlier versions)

You will also need a C++20 compliant compiler.

### Compile

Navigate to project directory:

```bash
mkdir build && cd build
cmake ..
make
```
### Run

```bash
./qpix
```
