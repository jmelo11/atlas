# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
from pathlib import Path
from sys import platform
import os 

__version__ = "2.1.3"

LIB_DIR = Path("/Users/josemelo/desktop/dev/builds")
include_dirs = [str(LIB_DIR / 'include'), '/opt/homebrew/include']
library_dirs = [str(LIB_DIR / 'lib')]

# get INSTALL_PATH env variable to include and lib paths
install_path = os.environ.get('INSTALL_DIR')
if install_path:
    include_dirs.append(install_path + '/include')
    library_dirs.append(install_path + '/lib')

libraries = ["QuantLib", "Atlas"]
ad_lib_name = ""
if ad_lib_name != "":
    libraries.append(ad_lib_name)


if platform == 'win32':
    extra_compile_args = ['/std:c++20']
else:
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
    name="atlas-finance-noad",
    version=__version__,
    author="Jose Melo",
    author_email="jmelo@live.cl",
    description="Pricing library for Python",
    ext_modules=ext_modules,    
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)
