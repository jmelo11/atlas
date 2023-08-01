// modules
// #include "aad.hpp"
#include "couponlegs.hpp"
#include "currencies.hpp"
#include "datetime.hpp"
#include "enums.hpp"
#include "instruments.hpp"
#include "market.hpp"
#include "models.hpp"
#include "parsers.hpp"
#include "rates.hpp"
#include "visitors.hpp"

namespace py = pybind11;
using namespace Atlas;

PYBIND11_MODULE(Atlas, m) {
    m.doc() = "Atlas";
    // py_aad(m);
    py_enums(m);
    py_datetime(m);
    py_currencies(m);
    py_couponlegs(m);
    py_rates(m);
    py_market(m);
    py_instruments(m);
    py_models(m);

    py::module_ visitors = m.def_submodule("Visitors", "Visitors submodule");
    py_visitors(visitors);

    py::module_ parsers = m.def_submodule("Parsers", "Parsers submodule");
    py_parsers(parsers);
}