Boxworld
=====
Compiling for Unix
---------
Make sure that [of](https://github.com/ofnode/of) and Boxworld share the same folder.

Change directory to Boxworld and perform these steps:
```bash
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=clang++
ninja
```
Compiling With QtCreator
---------
### [Reference here for compiling Boxworld with QtCreator](https://github.com/ofnode/of/wiki/Compiling-ofApp-with-IDE-on-Linux)

Open Qt Creator and setup it first. Go to "Help" -> "About Plugins..." and enable "ClangCodeModel" plugin. Restart Qt Creator to make it work. Now go to "Tools" -> "Options...", click on "Build & Run" and switch to CMake tab. Enable "Prefer Ninja generator" and hit OK.  open BoxWorld's CMakeLists.txt file as Qt Creator's project by going into "File" -> "Open File or Project..." menu. In the opened file dialog choose ofApp/CMakeLists.txt. For the "Arguments" field set C++ compiler to be clang++:
```bash
-DCMAKE_CXX_COMPILER=clang++
```

Compiling for Xcode
--------
```bash
cmake .. -G Xcode -DCMAKE_BUILD_TYPE=Debug
xcodebuild -configuration Debug
```
Compiling for Raspberry Pi
--------
Need CMake version 3.3.2, and GCC version >= 4.9.
```bash
cmake  ../ -G "Unix Makefiles"  -DOF_PLATFORM=armv7
make
```
Edit this page with [Dillinger](http://dillinger.io/)
--------
