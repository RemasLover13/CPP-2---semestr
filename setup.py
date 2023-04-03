import pybind11
from distutils.core import setup, Extension

ext_modules = [
    Extension(
        'library',
        ['library.cpp', 'main.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
]

setup(
    name='library',
    version='0.0.1',
    author='edik krmakov',
    author_email='krmakov2001@mail.ru',
    description='pybind11 extension',
    ext_modules=ext_modules,
    requires=['pybind11']  # не забываем указать зависимость от pybind11
)