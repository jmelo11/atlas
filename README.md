# Atlas: Financial Library

## Examples

Main examples can be found in [C++](https://github.com/jmelo11/atlas/tree/main/examples/pricing) and [Python](https://github.com/jmelo11/atlas/blob/main/python/examples/pricingExample.ipynb)

## Description

Atlas aims to become a real alternative to existing financial tools on the market, offering greater performance, simplicity and scope.

- Performance: In terms of performance, to date, the only usable alternative in productive systems is QuantLib, which has a myriad of tools for valuing various instruments. However, it is important to mention that QuantLib is not intended to be fast (it does not have multithreading by default, for example), but focuses on flexibility. We believe there is a space in the market where Atlas can differentiate itself by narrowing the scope of available tools and focusing on speed, building from the ground up with that goal in mind.

- Simplicity: Simplicity is another key aspect of Atlas. The aim is to have a simple but practical tool, so that users can learn to use it easily, and flexible enough to implement the most advanced financial instruments in the market.

Some of the technical features to be incorporated in Atlas are:

- Multithreading: having a structure that allows the library to be used in a thread-safe way is crucial in order to get the most out of it.

- Automatic differentiation: AAD has become a standard for the calculation of sensitivities in the financial world, enabling the possibility of automatically calculating risks without having to recalculate all our instruments. This functionality is crucial if, for example, you are looking to calculate XVAs or other metrics.

## Documentation

You can find the documentation available until today at <https://jmelo11.github.io/atlasdocs/>.

## Roadmap

### General

- [x] Basic class structure (under development)
- [x] Automatic differentiation
- [x] CPU Parallelisation
- [x] Port to Python
- [ ] Portfolio compression
- [x] Transformation to local currency
- [x] Currency precision
- [ ] GPU Parallelisation (depends on feasibility)
- [ ] Instrument customisation (incorporate QL parameters)

### Functionalities

- [x] NPV
- [x] Sensitivities (e.g. Duration)
- [x] Par Rates
- [x] ZSpread

### Instruments

- [x] Linear Products
  - [x] Loans
    - [x] Fixed rate loans (bullet)
    - [x] Fixed rate loans (equal instalments)
    - [x] Fixed Rate Loans (custom payment profile)
    - [x] Floating rate loans (bullet)
    - [x] Loans floating rate (custom payment profile)
    - [x] Mixed rate loans
  - Derivatives
    - [x] Forwards
    - [ ] Vanilla swaps
    - [ ] Xccy
  - OIS
  - Fixed Income
    - Instruments with market conventions

- Non-linear products

### Curves

- [x] Interpolation with AD support
- [x] Curves with AD support
- [ ] Bootstrap with AD support

### Models

- [x] Generation of rates based on static curves
- [ ] LGM for generation of market trajectories

### Prerequisites

Before compiling the project, you need to install the following libraries:

- Boost: <https://www.boost.org/>
- QuantLib: <https://github.com/lballabio/quantlib>

The future goal is to replace these dependencies with internal versions of the project.

#### Recommended directory structure

To facilitate the compilation process, it is suggested to organise the directories as follows:

- builds
  - release
  - debug
- libs
  - QuantLib
  - boost

#### Compiling QuantLib with CMake (Windows)

1. Inside the libs/QuantLib directory, create a folder called build.
2. Open a terminal and navigate to the newly created build folder.
3. Run the following commands, replacing <path/to/installed/libs> and <your/boost/path> with the appropriate paths:

##### Debug

```cmd
    cmake -S . -B . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/installed/libs>' -DBoost_INCLUDE_DIR='<your/boost/path>' -DQL_BUILD_BENCHMARK=OFF - DQL_BUILD_EXAMPLES=OFF -DQL_BUILD_TEST_SUITE=OFF -DCMAKE_CXX_FLAGS="/MDdd /EHsc /MP" -DQL_TAGGED_LAYOUT=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebugDLL" -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebugDLL" -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebugDLL"

    cmake --build . --target INSTALL --config <config>
```

##### Release

```cmd
    cmake -S . -B   . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/installed/libs>' -DBoost_INCLUDE_DIR='<your/boost/path>' -DQL_BUILD_BENCHMARK=OFF -DQL_BUILD_BENCHMARK=OFF -DQL_BUILD_EXAMPLES=OFF -DQL_BUILD_TEST_SUITE=OFF -DQL_BUILD_EXAMPLES=OFF DQL_BUILD_EXAMPLES=OFF -DQL_BUILD_TEST_SUITE=OFF -DCMAKE_CXX_FLAGS="/MD /EHsc /MP" -DQL_TAGGED_LAYOUT=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDLL" -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDLL" -DQL_TAGGED_LAYOUT=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDLL"

    cmake --build . --target INSTALL --config <config>
```

Where config can be Debug or Release, depending on the configuration you want to compile.

#### Boost Installation

Boost includes an installer. Follow the instructions at <https://www.boost.org/doc/libs/1_82_0/more/getting_started/windows.html> to install it on your system.

#### Atlas compilation

1. Inside the Atlas project directory, create a folder named build.
2. Open a terminal and navigate to the newly created build folder.
3. Run the following commands, replacing ```path/to/installed/libds``` and ```your/boost/path``` with the appropriate paths:

```cmd
    cmake -S . -B . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/installed/libs>' -DBoost_INCLUDE_DIR='<your/boost/path>'
    cmake --build . --target install --config <config>
```

cmake -S . -B . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/installed/libs>' -DBoost_INCLUDE_DIR='<your/boost/path>'

#### Python module installation

In case you want to install the package from this repository, you need to configure the ```setup.py``` file. In this case, it will also be necessary to have pybind11 installed. For consistency it is recommended to use CMAKE for this, as this way the ```find_package`` command will work. Once installed and the ````BUID_PYTHON=ON``` option is selected when configuring Atlas, you just need to:

1. Go with the terminal to the ```python``` folder.
2. Execute the command ````pip install .````.

It is also possible to install the standard package via PYPI:
```pip install atlas-finance``` or ```pip install atlas-finance-noad```.
