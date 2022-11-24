# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
from pathlib import Path
from sys import platform

__version__ = "1.0.0"

BASE_DIR = Path(__file__).absolute().parent.resolve()

include_dirs = []
library_dirs = []
libraries = []
if platform == "win32":
    LIB_DIR = Path('C:/Users/bloomberg/Desktop/Desarrollo/builds')
    include_dirs += [str(LIB_DIR / 'include')]
    library_dirs += [str(LIB_DIR / 'lib')]

    include_dirs += [str(LIB_DIR / 'boost')]
    libraries += ['QuantLib-x64-mt', 'QuantLibParser',
                  'nlohmann_json_schema_validator', 'Atlas']

else:
    if platform == "linux" or platform == "linux2":
        LIB_DIR = Path('/usr/local')
    else:
        LIB_DIR = Path('/Users/josemelo/Desktop/dev/builds')
        include_dirs += ['/opt/homebrew/opt/boost/include']
        library_dirs += ['/opt/homebrew/opt/boost/lib']

    include_dirs += [str(LIB_DIR / 'include')]
    library_dirs += [str(LIB_DIR / 'lib')]

    libraries += ['QuantLib', 'QuantLibParser',
                  'nlohmann_json_schema_validator', 'Atlas']

extra_compile_args = ['-std=c++20']

ext_modules = [
    Pybind11Extension("Atlas",
                      ["module.cpp"],
                      include_dirs=include_dirs,
                      library_dirs=library_dirs,
                      libraries=libraries,
                      define_macros=[('VERSION_INFO', __version__)],
                      extra_compile_args=extra_compile_args,
                      language="c++20"
                      ),
]

setup(
    name="Atlas",
    version=__version__,
    author="Itau",
    author_email="jose.melo@itau.cl",
    description="pricers using pybind11",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)
