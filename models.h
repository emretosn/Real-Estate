#ifndef _MODELS_H
#define _MODELS_H

#include"dataset.h"

int model_by_similarity(House **houses[], House *new_house);
void create_data_matrice(House **houses[], double **areas, double **prices);
double** get_transpose(double **A, int r, int c);
double** get_inverse(double **A);
double** get_multiplication(double **A, double **B, int r1, int c1, int r2, int c2);
double** calculate_parameter(double **A, double **B, int r1, int c1, int r2, int c2);
void make_prediction(House **houses[], double **X, double **Y, double **W, int datasize);
void array_free(double **array, int datasize);

#endif //_MODELS_H