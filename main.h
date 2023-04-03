#include "library.h"
#include "venv/lib/python3.8/site-packages/pybind11/include/pybind11/pybind11.h"
namespace py = pybind11;

PYBIND11_MODULE(library, m) {
m.def("add", &add);
};
