#ifndef FA2F77F8_CEEF_4BDE_8D49_1D807BDAF93A
#define FA2F77F8_CEEF_4BDE_8D49_1D807BDAF93A

#include "config.hpp"
#include <atlas/models/spotmarketdatamodel.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyModel : public Model<NumType> {
        using Model<NumType>::Model;

        MarketData<NumType> marketData(const Date& evalDate = Date()) const override {
            PYBIND11_OVERRIDE_PURE(MarketData<NumType>, /* Return type */
                                   Model<NumType>,      /* Parent class */
                                   marketData,          /* Name of function in C++ (must match Python name) */
                                   evalDate             /* Argument(s) */
            );
        };
    };

};  // namespace Aux

void py_models(py::module& m) {
    py::class_<Model<NumType>, Aux::PyModel>(m, "Model")
        .def("marketData", &Model<NumType>::marketData)
        .def("marketRequest", py::overload_cast<>(&Model<NumType>::marketRequest, py::const_))
        .def("marketRequest", py::overload_cast<const MarketRequest&>(&Model<NumType>::marketRequest));

    py::class_<SpotMarketDataModel<NumType>, Model<NumType>>(m, "SpotMarketDataModel")
        .def(py::init<const MarketRequest&, const MarketStore<NumType>&>(), py::arg("marketRequest"), py::arg("store"));
};

#endif /* FA2F77F8_CEEF_4BDE_8D49_1D807BDAF93A */
