# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import os

from pathlib import Path
__version__ = "0.0.1"

BASE_DIR = Path(__file__).absolute().parent.parent.resolve()

ext_modules = [
    Pybind11Extension("atlaspython",
        ["module.cpp"],
        include_dirs=[str(BASE_DIR / 'atlas'),                      
                      os.environ['QL_PATH'],                  
                      os.environ['BOOST_PATH'],
                      str(BASE_DIR / 'packages/json/include'),
                      str(BASE_DIR / 'packages/pybind11_json/include')],                  

        library_dirs=[os.environ['QL_PATH'] + '/lib',                      
                      os.environ['BOOST_PATH'] + '/libs',
                      str(BASE_DIR / 'build/atlas/x64/Release'),],

        libraries=['QuantLib-x64-mt','atlas'],
        # Example: passing in the version to the compiled code
        define_macros = [('VERSION_INFO', __version__)]       
        ),
]

setup(
    name="atlaspython",
    version=__version__,
    author="Itau",
    author_email="jose.melo@itau.cl",
    description="pricers using pybind11",
    ext_modules=ext_modules,    
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)