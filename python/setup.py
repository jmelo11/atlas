# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
from pathlib import Path
from sys import platform

__version__ = "2.0.1"

LIB_DIR = Path("C:/Users/jmelo/Desktop/Desarrollo/c++/builds/release")
include_dirs = [str(LIB_DIR / 'include'), 'C:/Users/jmelo/Desktop/Desarrollo/c++/builds/release/boost']
library_dirs = [str(LIB_DIR / 'lib')]

libraries = ["QuantLib", "Atlas", "xad64_vc142_md"]
extra_compile_args = ['-std=c++17', '/std:c++17']

ext_modules = [
    Pybind11Extension("Atlas",
                      ["module.cpp"],
                      include_dirs=include_dirs,
                      library_dirs=library_dirs,
                      libraries=libraries,
                      define_macros=[('VERSION_INFO', __version__)],
                      extra_compile_args=extra_compile_args),
]

setup(
    name="atlas-finance",
    version=__version__,
    author="Jose Melo",
    author_email="jmelo@live.cl",
    description="Pricing library for Python",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)
