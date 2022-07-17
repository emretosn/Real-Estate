#include<stdio.h>
#include<stdlib.h>
#include"models.h"
#include"dataset.h"

// The funciton that makes a prediction by averaging the similary houses' saleprices
int model_by_similarity(House **houses[], House *new_house)
{
    // Getting the same neighborhood
    House *list = get_neighborhoods(houses, new_house);

    int j = 0;
    House *arr = malloc(sizeof(House));
    if(!arr) exit(2);
    for(House *tmp = list; tmp != NULL; tmp = tmp->next[Nneighborhood])
    {
        arr = realloc(arr, (j+1)*sizeof(House));
        arr[j++] = *tmp;
    }
    // Sort for lotarea
	sort_houses(arr,"lotarea",j);
	// Get similar for lotarea
    int k = 0;
    House *lot = malloc(sizeof(House));
    if (!lot) exit(2);
    for(int i = 0; i < j; i++)
    {										
        if((arr[i].lotarea <= (new_house->lotarea + 2000)) && (arr[i].lotarea >= (new_house->lotarea - 2000))) 
        {
            lot = realloc(lot, (k+1)*sizeof(House));
            lot[k] = arr[i];
            k++;
        }
    }
    // Sort for yearbuilt
	sort_houses(lot,"yearbuilt",k);
    // Get similar for yearbuilt
    int l = 0;
    House *yb = malloc(sizeof(House));
    for(int i = 0; i < k; i++)
    {
        if((lot[i].yearbuilt <= (new_house->yearbuilt + 5)) && (lot[i].yearbuilt >= (new_house->yearbuilt - 5)))
        {
            yb = realloc(yb, (l+1)*sizeof(House));
            yb[l] = lot[i];
            l++;
        }
    }
	
    int avg = 0;
	
    // Return conditions for similarity

    // Return if no similar lot area
	if(k == 0)
	{
		free(lot);
		free(yb);
		free(arr);
		return avg;
	}
    // return if no similar yearbuilt
	else if(l == 0)
	{
		for(int i = 0; i < k; i++)
			avg += lot[i].saleprice;

		free(lot);
		free(yb);
		free(arr);
		return avg/k;
	}
    // Return with all the similar houses
	else
    	{
		for(int i = 0; i < l; i++)
			avg += yb[i].saleprice;
	
		free(lot);
		free(yb);
		free(arr);
		return avg/l;
	}
}

// Creating the matrices from house_price_data.csv to find the linear equation
void create_data_matrice(House **houses[], double **areas, double **prices)
{
	int j=0;
    	for(int i=0; i<N; i++)
    	{
        	for (House *tmp = houses[Nid][i]; tmp != NULL; tmp = tmp->next[Nid])
        	{
            		if(tmp!=NULL && tmp->saleprice != 0 && tmp->lotarea != 0)
            		{
				prices[j][0] = tmp->saleprice;
				areas[j][0] = 1;
				areas[j][1] = tmp->lotarea;
				j++;
            		}
        	}
    	}
}

// Getting the transpose of a matrix
double** get_transpose(double **A, int r, int c)
{	
    double **R = malloc(c*sizeof(double*));
    if (!R) exit(2);
    for(int i = 0; i < c; i++) 
    {
        R[i] = malloc(r*sizeof(double));
        if (!R[i]) exit(2); 
    }
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            R[j][i] = A[i][j];
        }
    }
    return R;
}

// Finding the inverse of a two by two matrix
double** get_inverse(double **A)
{
    double **R = malloc(2*sizeof(double*));
    if (!R) exit(2);
    for(int i = 0; i < 2; i++)
    {
        R[i] = malloc(2*sizeof(double));
        if (!R[i]) exit(2); 
    }
    // Finding the determinant
    double det = (A[0][0]*A[1][1])-(A[0][1]*A[1][0]);
    if(det == 0) exit(3);
    // Writing the determinant
    R[0][0] = (A[1][1])/det;
    R[0][1] = -(A[0][1])/det;
    R[1][0] = -(A[1][0])/det;
    R[1][1] = (A[0][0])/det;
    return R;
}

// Getting the result of the multiplication of two matrices
double** get_multiplication(double **A, double **B, int r1, int c1, int r2, int c2)
{
    double **R = malloc(r1*sizeof(double*));
    if (!R) exit(2);
    for(int i = 0; i < r1; i++) 
    {
        R[i] = malloc(c2*sizeof(double));
        if (!R[i]) exit(2); 
    }
	int j=0;
    for(int i=0; i<r1; i++)
    {
        for(int j=0; j<c2; j++)
            R[i][j] = 0;
    }
    // Multiplying the two marrices
    for (int i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            for (int k = 0; k < c1; k++)
            {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return R;
}

// Getting the linear equation to make the prediction
// The formula : (Xt.X)^-1.Xt.Y = W
double** calculate_parameter(double **A, double **B, int r1, int c1, int r2, int c2)
{
	double** tmp1;
	double** tmp2;
	double** tmp3;
	tmp1 = get_transpose(A,r1,c1);
	tmp2 = get_multiplication(tmp1 ,A,c1,r1,r1,c1);
	array_free(tmp1,c1);
	tmp1 = get_inverse(tmp2);
	array_free(tmp2,c1);
	tmp2 = get_transpose(A,r1,c1);
	tmp3 = get_multiplication(tmp1,tmp2,c1,c1,c1,r1);
	array_free(tmp1,2);
	array_free(tmp2,c1);
	tmp1 = get_multiplication(tmp3,B,c1,r1,r2,c2);
	array_free(tmp3,c1);

   return tmp1 ;
}

// Making the prediction and saving it to a csv file
void make_prediction(House **houses[], double **X, double **Y, double **W, int datasize)
{
	House *houses_pre = malloc(datasize*sizeof(House));
    if (!houses_pre) exit(2);
	int j=0;
    for(int i=0; i<N; i++)
    {
        for(House *tmp = houses[Nid][i]; tmp != NULL; tmp = tmp->next[Nid])
        {
		if(tmp!=NULL && tmp->lotarea != 0 && tmp->id != 0)
            	{	
			X[j][1] = tmp->lotarea;
			houses_pre[j].id = tmp->id;
                	// The prediction equation
			Y[j][0] = W[1][0] * X[j][1] + W[0][0];
			houses_pre[j].saleprice = Y[j][0];
			houses_pre[j].lotarea = tmp->lotarea;
			j++;
		}
        }
    }
    // Saving as a csv file
    saveas_csv(houses_pre,100,1);
	free(houses_pre);
}

// Freeing a 2D array
void array_free(double **array, int datasize)
{
	double* cridx;
	for (int i = 0; i < datasize; i++)
	{
		cridx = array[i];
		free(cridx);
	}	
	free(array);
}
