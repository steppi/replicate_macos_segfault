#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_libs.h"


int diagonalise_dyn_mat_zheevd(const int order,
    double* dyn_mat, double* eigenvalues, ZheevdFunc zheevdptr) {

    char jobz = 'V';
    char uplo = 'L';
    int mat_order = order;
    int lda = order;
    int lwork, lrwork, liwork = -1;
    double *work, *rwork;
    int *iwork;
    int info;

    // Query vars
    double lworkopt, lrworkopt;
    int liworkopt;

    // Workspace query
    (*zheevdptr)(&jobz, &uplo, &mat_order, dyn_mat, &lda, eigenvalues, &lworkopt, &lwork,
        &lrworkopt, &lrwork, &liworkopt, &liwork, &info);
    if (info != 0) {
        printf("INFO: Zheevd failed querying workspace with info %i", info);
        return info;
    }
    lwork = (int)lworkopt;
    lrwork = (int)lrworkopt;
    liwork = liworkopt;

    // Allocate work arrays
    work = (double*)malloc(2*lwork*sizeof(double));
    rwork = (double*)malloc(lrwork*sizeof(double));
    iwork = (int*)malloc(liwork*sizeof(int));

    (*zheevdptr)(&jobz, &uplo, &mat_order, dyn_mat, &lda, eigenvalues, work, &lwork,
        rwork, &lrwork, iwork, &liwork, &info);

    free((void*)work);
    free((void*)rwork);
    free((void*)iwork);

    if (info != 0) {
       printf("INFO: Zheevd diagonalisation failed with info %i", info);
    }

    return info;
}
