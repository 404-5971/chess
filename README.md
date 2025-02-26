# Chess - Raylib

This is a 2D chess game that is supposed to be cross platform and efficient to run. 
The game is meant to be played between two people for now on the same computer.

## Tech Stack used 
1. Programming language : *C*
2. Build System: [Cmake](https://cmake.org/)
3. Library : [Raylib](https://www.raylib.com/)

## Prerequisite
1. *Compiler*  : 
-  [gcc](https://gcc.gnu.org/)  / [clang](https://releases.llvm.org/download.html) : For unix based system.
-  [mingw-gcc](https://sourceforge.net/projects/mingw-w64/)  / [MSCV](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options?view=msvc-170) : For Windows OS.
2. *Build System*: 
- [Cmake](https://cmake.org/)
3. *Library* : 
- [Raylib](https://www.raylib.com/)

#### Extra For windows:
Please install microsoft visual studio as it helps speed up the process for the cmake.

## Features

## Build  Mechanism

For this partical project I have made a two type build system a script based build system and a manual build system.

### Automatic / Script Based build system

All you have to do for this is run the build script that is located in the script directory for the project file.
Got to your project directory by opening the terminal
The commands are:

1. Clone the project from github.
```
git clone https://github.com/tsdios/chess.git 
 ```
```
cd chess
```
2. After cloning it execute the build script

- UNIX based System(*Linux*, *macOS*, *others*)
```
sh scripts/build.sh
```
This will create a build directory and build the project it self

- Windows based system
```
scripts\build.bat
```
*This will have two out comes*:
- The first one is that if visual studio exist visual studio solution will be created.
- If not I will operate as simplar to as the unix system granted that we have to change some stuff of the file path.

3. Simply the executable can be found in the * */build/bin* * directory
- To run it :

 * UNIX Based system: 
``` sh scripts/run.sh```

 * Windows system: 
 - Do this if you do not have visual studio if you do this will not work. 
``` scripts\run.bat```
### Manual building 

1. *UNIX based system* and *Windows based system without visual studio*: 
Got to your project directory by opening the terminal
The commands are:

- Clone the project from github.
```
git clone https://github.com/tsdios/chess.git 

```
```
cd chess
```
- Create a build direcotry 
```
mkdir build
```
- Go to that directory
```
cd build
```
- Run the cmake command:
```
cmake ..
```
- Then inside the build direcotry make the executable wich is done via makefile.
``
make
``
- Go to the bin direcotry and run it from there.
```
cd bin & ./chess
```
2. *Windows system with visual studio* :

- Clone the project from github.
```
git clone https://github.com/tsdios/chess.git 

```
```
cd chess
```
- Create a build direcotry 
```
mkdir build
```
- Go to that directory
```
cd build
```
- Run the cmake command:
```
cmake ..
```
- Go to Explorer and click the visual studio solution created by this program.
- Build a release version from there.


## Future improvement
Maybe add a network protocol that lets it play over the network.

## Development Status
For detailed information about the project's development progress and roadmap, see [DEVELOPMENT.md](DEVELOPMENT.md).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

