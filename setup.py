from setuptools import setup, Extension


def get_c_extension():
    import os
    import numpy as np
    from sys import platform
    include_dirs = [np.get_include(), 'c']
    sources = ['c/_segfault.c', 'c/dyn_mat.c', 'c/load_libs.c']

    if platform == 'darwin':
        brew_prefix = '/usr/local'
        os.environ['CC'] = f'{brew_prefix}/opt/llvm/bin/clang'
        link_args = [f'-L{brew_prefix}/opt/llvm/lib', '-fopenmp']
        compile_args = ['-fopenmp']
    else:
        link_args = None
        compile_args = ['/openmp']

    c_extension = Extension(
        'segfault._segfault',
        extra_compile_args=compile_args,
        extra_link_args=link_args,
        include_dirs=include_dirs,
        sources=sources
    )
    return c_extension


def run_setup():
    setup(
        name='segfault',
        install_requires=['scipy>=1.2.0'],
        ext_modules=[get_c_extension()]
    )


run_setup()
