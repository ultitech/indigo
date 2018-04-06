# indigo

[![Travis-CI Status](https://travis-ci.org/ultitech/indigo.svg?branch=master)](https://travis-ci.org/ultitech/indigo)
[![AppVeyor Status](https://ci.appveyor.com/api/projects/status/oly0sfoyi8vcdkba?svg=true)](https://ci.appveyor.com/project/ultitech/indigo)
[![Release](https://img.shields.io/badge/release-v1.0-blue.svg)](https://github.com/ultitech/indigo/releases/latest)
[![License: zlib](https://img.shields.io/badge/license-zlib-blue.svg)](http://www.zlib.net/zlib_license.html)

indigo is a cross-platform non-euclidian 3D maze game.

## Getting Started

This project was created as a graphic demo to test a non-euclidian environment using SDL2 on Linux, Mac, and Windows.

The latest binary releases can be found [here](https://github.com/ultitech/indigo/releases/latest) or you can build the project yourself.

### Prerequisites

indigo requires [cmake](http://cmake.org) for compilation and depends on one third-party open source library:

+ [SDL2](https://www.libsdl.org)

#### Linux

+ build-essential

#### Mac

+ [Xcode](https://itunes.apple.com/us/app/xcode/id497799835?mt=12)

#### Windows

+ [Visual Studio](https://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx) or
+ [MinGW](http://www.mingw.org)

### Compiling

#### Linux

If you haven't already, install the build-essentials and cmake, e.g.
```Bash
sudo apt-get install build-essential cmake
```

Install all required libraries using your preferred package manager, e.g.
```Bash
sudo apt-get install libsdl2-dev
```

Next create your build directory inside the indigo folder and run `cmake ..`, e.g.
```Bash
mkdir build && cd build && cmake ..
```

Finally run `make` to compile the project.

#### Mac

If you haven't already, install Xcode and cmake.

Download the latest [SDL2 Development Library](https://www.libsdl.org/download-2.0.php) and copy the `SDL2.framework` framework into `/Library/Frameworks`.

Next create your build directory inside the indigo folder and run `cmake ..`. This creates a UNIX Makefile; to create a Xcode project file instead, use `cmake -G "Xcode" ..`, e.g.
```Bash
mkdir build && cd build && cmake -G "Xcode" ..
```

Finally run `make` or open up the Xcode project, select the indigo target, and hit run.


#### Windows

##### Visual Studio
If you haven't already, install Visual Studio and cmake.

Download the latest SDL2 librariy, place it in your preferred location, and set its path as a environment variable (SDL2).

Next create your build directory inside the indigo folder and run `cmake ..`. To [specify](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html#ide-build-tool-generators) the Visual Studio version and build architecture use `cmake -G "Visual Studio ` followed by version, year, and architecture, e.g. `12 2013 Win64" ..`.

Finally open up the indigo solution and hit build.

## Authors

Copyright © 2018 by [ultitech](https://www.ulti.tech)

* **Lukas Feller** - *Lead Programmer* - [luckyxxl](https://github.com/luckyxxl)
* **David Greiner** - *Programmer* - [USADavid](https://github.com/USADavid)

## License

This project is licensed under the zlib License - see the [LICENSE](LICENSE) file for details.
