# Saleae Synchronous Serial Analyzer

The Saleae Analyzer SDK is used to create Low Level Analyzers (LLA) for the Saleae Logic software via a plugin architecture. These plugins are used to decode protocol data from captured waveforms. In many cases you can use a [High Level Analyzer Extension](https://support.saleae.com/extensions/high-level-analyzer-quickstart) to process data from an existing protocol decoder instead of building a LLA.

## Prerequisites

### Windows

Dependencies:

- Visual Studio 2017 (or newer) with C++
- CMake 3.13+

**Visual Studio 2017**

_Note - newer versions of Visual Studio should be fine._

Setup options:

- Programming Languages > Visual C++ > select all sub-components.

Note - if CMake has any problems with the MSVC compiler, it's likely a component is missing.

**CMake**

Download and install the latest CMake release here.
https://cmake.org/download/

### MacOS

Dependencies:

- XCode with command line tools
- CMake 3.13+

Installing command line tools after XCode is installed:

```
xcode-select --install
```

Then open XCode, open Preferences from the main menu, go to locations, and select the only option under 'Command line tools'.

Installing CMake on MacOS:

1. Download the binary distribution for MacOS, `cmake-*-Darwin-x86_64.dmg`
2. Install the usual way by dragging into applications.
3. Open a terminal and run the following:

```
/Applications/CMake.app/Contents/bin/cmake-gui --install
```

_Note: Errors may occur if older versions of CMake are installed._

### Linux

Dependencies:

- CMake 3.13+
- gcc 5+

Misc dependencies:

```
sudo apt-get install build-essential
```

## Building Analyzer

### Windows

```bat
mkdir build
cd build
cmake .. -A x64
cmake --build .
:: built analyzer will be located at SampleAnalyzer\build\Analyzers\Debug\SimpleSerialAnalyzer.dll
```

### MacOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```

### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```
