// modules
#include "aad.hpp"
#include "couponlegs.hpp"
#include "currencies.hpp"
#include "datetime.hpp"
#include "enums.hpp"
#include "instruments.hpp"
#include "market.hpp"
#include "models.hpp"
#include "rates.hpp"
#include "visitors.hpp"

namespace py = pybind11;
using namespace Atlas;

PYBIND11_MODULE(Atlas, m) {
    m.doc() = "Atlas";  // optional module docstring
    py_aad(m);
    py_enums(m);
    py_datetime(m);
    py_currencies(m);
    py_couponlegs(m);
    py_rates(m);
    py_market(m);
    py_instruments(m);
    py_visitors(m);
    py_models(m);

    py::module_ submodule = m.def_submodule("visitors", "Visitors submodule");
    py_newvisitors(submodule);
}