#ifndef FA2F77F8_CEEF_4BDE_8D49_1D807BDAF93A
#define FA2F77F8_CEEF_4BDE_8D49_1D807BDAF93A

#include <atlas/models/spotmarketdatamodel.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyModel : public Model<dual> {
        using Model<dual>::Model;

        MarketData<dual> marketData(const Date& evalDate = Date()) const override {
            PYBIND11_OVERRIDE_PURE(MarketData<dual>, /* Return type */
                                   Model<dual>,      /* Parent class */
                                   marketData,       /* Name of function in C++ (must match Python name) */
                                   evalDate          /* Argument(s) */
            );
        };
    };

};  // namespace Aux

void py_models(py::module& m) {
    py::class_<Model<dual>, Aux::PyModel>(m, "Model")
        .def("marketData", &Model<dual>::marketData)
        .def("marketRequest", py::overload_cast<>(&Model<dual>::marketRequest, py::const_))
        .def("marketRequest", py::overload_cast<const MarketRequest&>(&Model<dual>::marketRequest));

    py::class_<SpotMarketDataModel<dual>, Model<dual>>(m, "SpotMarketDataModel")
        .def(py::init<const MarketRequest&, const MarketStore<dual>&>(), py::arg("marketRequest"), py::arg("store"));
};

#endif /* FA2F77F8_CEEF_4BDE_8D49_1D807BDAF93A */
