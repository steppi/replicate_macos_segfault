#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL SEGFAULT_NPY_ARRAY_API
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <Python.h>
#include <numpy/arrayobject.h>
#include "load_libs.h"
#include "dyn_mat.h"

static PyObject *diagonalise(PyObject *self, PyObject *args) {

    // Define input args
    PyArrayObject *py_evals;
    PyArrayObject *py_dmats;
    int n_threads = 1;

    // Define pointers to Python array data
    double *evals;
    double *dmats;

    // Other vars
    int mat_order;
    int n_mats;
    int q;
    int dmat_elems;

    // Parse inputs
    if (!PyArg_ParseTuple(args, "O!O!i",
                          &PyArray_Type, &py_evals,
                          &PyArray_Type, &py_dmats,
                          &n_threads)) {
        return NULL;
    }

    // Load library functions
    ZheevdFunc zheevd;
    zheevd = get_zheevd();
    if (zheevd == NULL) {
        PyErr_Format(PyExc_RuntimeError, "Could not load zheevd function\n");
        return NULL;
    }

    // Point to Python array data
    n_mats = PyArray_DIMS(py_evals)[0];
    mat_order = PyArray_DIMS(py_dmats)[1];
    evals = (double*) PyArray_DATA(py_evals);
    dmats = (double*) PyArray_DATA(py_dmats);
    dmat_elems = 2*mat_order*mat_order;

    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        #pragma omp for
        for (q = 0; q < n_mats; q++) {
            double *dmat, *eval;
            eval = (evals + q*mat_order);
            dmat = (dmats + q*dmat_elems);
            printf("Diagonalising qpt %i\n", q);
            diagonalise_dyn_mat_zheevd(mat_order, dmat, eval, zheevd);
        }
    }

    return Py_None;
}

static PyMethodDef _segfault_methods[] = {
    {"diagonalise", diagonalise, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef _segfault_module_def = {
    PyModuleDef_HEAD_INIT,
    "_segfault",
    NULL,
    -1,
    _segfault_methods
};

PyMODINIT_FUNC PyInit__segfault(void) {
    import_array();
    return PyModule_Create(&_segfault_module_def);
}
