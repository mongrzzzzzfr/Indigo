[![Build Status](https://travis-ci.org/epam/Indigo.svg?branch=master)](https://travis-ci.org/epam/Indigo)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/epam/indigo?branch=master&svg=true)](https://ci.appveyor.com/project/mkviatkovskii/indigo)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

# EPAM Indigo projects #

Copyright (c) 2009 to present EPAM Systems
Apache License version 2.0

## Introduction ##

This repository includes:
 * Bingo: Chemistry search engine for Oracle, Microsoft SQL Server and PostgreSQL databases
 * Indigo: Universal cheminformatics library with bindings to .NET, Java and Python, and the following tools:
  - Legio: GUI application for combinatorial chemistry
  - ChemDiff: Visual comparison of two SDF or SMILES files
  - indigo-depict: Molecule and reaction rendering utility
  - indigo-cano: Canonical SMILES generator
  - indigo-deco: R-Group deconvolution utility

Detailed documentation is available at <http://lifescience.opensource.epam.com>

## Download ##
<https://lifescience.opensource.epam.com/download/indigo/index.html>

Bindings:
* .NET: <https://www.nuget.org/packages/Indigo.Net>
* Java: <https://search.maven.org/search?q=g:com.epam.indigo>
* Python: <https://pypi.org/project/epam.indigo/>

## Source code organization ##

Main directory structure layout:
 * api: Indigo API sources
 * bingo: Bingo sources
 * build_scripts: CMake and Python scripts for building all the sources
 * third_party: sources for third-party libraries
 * utils: utilities sources
 * common|graph|layout|molecule|reaction|render2d: indigo-core sources

Each project is placed in the corresponding directory with CMakeList.txt configuration
file, that does not include other projects. In order to build the whole project with the
correct references you need to use CMake configurations from the build_scripts directory.

## Build instructions ##

All the CMake projects are placed in `build_scripts` directories. You can use them manually,
or execute preconfigured scripts that does all the job.

## Bingo build instructions ##

To generate project configuration, build the source code, and create the archives for
installation you need to execute `build_scripts/bingo-release.py`:
```bash
python build_scripts/bingo-release.py --preset=linux32 --dbms=[postgres|oracle|sqlserver]
```
The are different cmake presets:
	linux32, linux64, win32, win64, mac10.7 (and also all later mac10.x versions)

## Indigo build instructions ##

### Build dependencies
#### Linux (GCC 4.9+ or Clang 3.5+)
APT-based requirements (Debian or Ubuntu):
```bash
sudo apt install cmake libfreetype6-dev libfontconfig1-dev
```
RPM-based requirements (RedHat, CentOS, Fedora): install Developer Toolset if your OS does not have GCC 4.9+ and then install dependencies:
```bash
sudo yum install cmake freetype-devel fontconfig-devel
```

### Build native Indigo libraries:
```bash
python build_scripts/indigo-release-libs.py
```
By default build script will use CMake default settings for current platform, but you can also select a preset for 
targeting special version of build tools. There is a `--preset` parameter for that, and here are the currently available 
presets:
* `win32-2013`, `win64-2013`: Visual Studio 2013
* `win32-2015`, `win64-2015`: Visual Studio 2015
* `win32-2017`, `win64-2017`: Visual Studio 2017
* `win32-2019`, `win64-2019`: Visual Studio 2019
* `win32-mingw`: MinGW
* `win64-mingw`: MinGW-w64 
* `linux32`, `linux64`: GCC or Clang on Linux with C++11 support
* `linux32-universal`, linux64-universal: GCC on Linux with statically linked libstdc++ for using on older Linux systems without C++11 support
* `mac10.7`, `mac10.8`, `mac10.9`, `mac10.10`, `mac10.11`, `mac10.12`, `mac10.12`, `mac10.13`, `mac10.14`, `mac10.15`: target Mac OS X or macOS
* `mac-universal`: targeting Mac OS X 10.7 as first version with C++11 support, should work on all Mac OS X 10.7+ systems

### Build bindings for .NET, Java or Python:
First you need to build Indigo native libraries (see previous section). Then
```bash
python build_scripts/indigo-make-by-libs.py --type=<wrapper_type>
```
There are following wrappers:
* `dotnet` for .NET Standard 2.0
* `java` for Java 1.6+
* `python` for Python 2.7 and 3.0+
You may also select comma-separated list of them, or skip `type` selection to build all wrappers.
#### .NET build dependencies
* .NET Core SDK 2.0+ (that supports targeting .NET Standard 2.0), and `dotnet` executable in PATH.
* `powershell` command in PATH. On Linux you may need to run `ln -fs /usr/bin/pwsh /usr/bin/powershell` before 
building wrappers.
#### Java build dependencies
* JDK 1.6+.
* Maven 3+, `mvn` in PATH.

### Build native Indigo utils
`````bash
python build_scripts/indigo-release-utils.py 
`````
Compiler presets could be also specified for building utils, see "Build native Indigo libraries" section for details.
### Build Java Utils
First you need to build Indigo native libraries and wrappers for Java (see previous sections).
Then run
`````bash
python build_scripts/indigo-release-java-utils.py
`````