# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
from pathlib import Path
from sys import platform

__version__ = "2.0.0"

LIB_DIR = Path("/Users/josemelo/desktop/dev/builds")
include_dirs = [str(LIB_DIR / 'include'), '/opt/homebrew/include']
library_dirs = [str(LIB_DIR / 'lib')]

if platform == "win32":
    libraries = ["QuantLib-x64-md", "xad", "Atlas"]
    extra_compile_args = ['-std=c++20']

else:
    libraries = ["QuantLib", "xad", "Atlas"]
    extra_compile_args = ['-std=c++20']

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
