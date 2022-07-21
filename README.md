# Atlas

C++ libraries for internal dev.

- atlas: simplified pricing library
- datamanager: c++ postgres integration
- parsers: string-to-quantlib parsing library
- testsuit: tests

## Compiling

For compiling, QuantLib and Boost must be downloaded and compiled.

System variables:

- Boost:  $(BOOST_PATH)
- QuantLib: $(QL_PATH)

### Dependencies

- Boost:
  - `boost_1_69_0 or newer`
- QuantLib:
  - `QuantLib-1.18.0 or newer`
- LibPQXX:
  - `libpqxx-8.4.0 or newer`
