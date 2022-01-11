#ifndef __dyn_mat_H__
#define __dyn_mat_H__

int diagonalise_dyn_mat_zheevd(const int order,
    double *dyn_mat, double *eigenvalues, ZheevdFunc zheevdptr);

#endif
