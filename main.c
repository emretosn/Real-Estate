#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include"dataset.h"
#include"models.h"
#include <inttypes.h>
#include <assert.h>

// The funcitons that dictates some operations in main
void readcheck(House **housesdata[], int mod, int *check, char *location, int *datasize);
void hash_free(House **houses[]);
void linked_free (Means *node); 

int main(int argc, char* argv[])
{	
	if(argc != 2)
	{
		printf("The right usage is shown bellow:\n");
		printf("./main <file_destination>\n\n");
		return 1;
	}
	
	// Main data hash table
	House **houses[Nsize] ;
	
    House *id[N] = {0};
    House *yearbuilt[N] = {0};
    House *neighborhood[N] = {0};
    House *overallqual[overallsize] = {0};
    House *overallcond[overallsize] = {0};
    House *kitchenqual[kitchensize] = {0};
    House *lotarea[N] = {0};
	House *saleprice[N] = {0};
	
	houses[Nid] = id;
    houses[Noverallqual] = overallqual;
    houses[Noverallcond] = overallcond;
    houses[Nkitchenqual] = kitchenqual;
    houses[Nneighborhood] = neighborhood;
    houses[Nyearbuilt] = yearbuilt;
    houses[Nlotarea] = lotarea;
	
	// Test data hash table
	House **test[Nsize];

	House *idddd[N] = {0};
	House *yrblt[N] = {0};
	House *nbhd[N] = {0};
	House *oq[overallsize] = {0};
	House *oc[overallsize] = {0};
	House *kq[kitchensize] = {0};
	House *ltarea[N] = {0};
	House *slprice[N] = {0};
			 
	test[Nid] = idddd;
	test[Noverallqual] = oq;
	test[Noverallcond] = oc;
	test[Nkitchenqual] = kq;
	test[Nneighborhood] = nbhd;
	test[Nyearbuilt] = yrblt;
	test[Nlotarea] = ltarea;
	
	// Variable declarations
	int value = 8;
	int read = 0;
	int read_test = 0;
	char* criter;
	int datasize = 0;
	int datatestsize = 0;
	int getid;
	
	printf("\nWelcome to the REAL ESTATE PROGRAM!\n\n");
	
	do{
		// The menu
		printf("THE POSSIBLE OPERATIONS :\n");
		printf("1 - Read the dataset\n");
		printf("2 - Find a house with its ID\n");
		printf("3 - Find the neighbors of a house with its ID\n");
		printf("4 - Mean sale prices according to a criterion\n");
		printf("5 - List the 10 most expensive houses\n");
		printf("6 - Sort the houses and save as csv\n");
		printf("7 - Make price prediction\n");
		printf("Press '0' to quit.\n\n");
		
		printf("Please choose an operation : ");
		assert(scanf("%d", &value) > 0);
		while((getchar())!='\n');
		printf("\n");
		
		// Check if the input is valid
		while(value>8 || value<0)
		{
			printf("Please enter a valid operation : ");
			assert(scanf("%d", &value) > 0);
			while((getchar())!='\n');
			printf("\n");
		}
		
		switch(value)
		{
			// Reading the dataset
			case 1:
				readcheck(houses, 0, &read, argv[1], &datasize);
				printf("The dataset has been read.\n\n");
				break;
			// Finding a house with its ID
			case 2:
				readcheck(houses, 0, &read, argv[1], &datasize);
				
				getid = 0;
				printf("Please enter the ID : ");
				do{
					assert(scanf("%d", &getid) > 0);
					while(getchar()!= '\n');
					printf("\n");
					if(!(100< getid && getid <= datasize + 100))
						printf("Invalid ID. Please enter an ID between 101-%i\n", datasize + 100);
					
				}while(!(100< getid && getid <= datasize + 100));

				print_house(get_house_byid(getid,houses));
				printf("\n");
				break;
			// Finding the neighbors of a house with its ID
			case 3:
				readcheck(houses, 0, &read, argv[1], &datasize);

				House *neighborhoods_of_house;
				House *house;
				getid = 0;
				
				printf("Please enter an ID : ");
				do{
					assert(scanf("%d", &getid) > 0);
					while(getchar()!='\n');
					printf("\n");
					if(!(100< getid && getid <= datasize + 100))
						printf("Invalid ID. Please enter an ID between 101-%i\n", datasize + 100);
				}while(!(100< getid && getid <= datasize + 100));
				
				house = get_house_byid(getid,houses);
				
				neighborhoods_of_house= get_neighborhoods(houses,house);
				
				for (House *tmp = neighborhoods_of_house; tmp != NULL; tmp = tmp->next[Nneighborhood])
				{
					print_house(tmp);
					printf("\n");
				}
				break;
			// Calculating the mean sale prices according to a criterion
			case 4:
				readcheck(houses, 0, &read, argv[1], &datasize);
				int value3;
				do{
					// Criterion menu
					printf("Please choose a criterion : \n");
					printf("1 - lotarea\n");
					printf("2 - neighborhood\n");
					printf("3 - yearbuilt\n");
					printf("4 - overallqual\n");
					printf("5 - overallcond\n");
					printf("6 - kitchenqual\n");

					printf("Please enter the criterion's index : ");
					assert(scanf("%d", &value3) > 0);
					
					// Check if the input is valid
					if(!(value3 < 7 && value3 > 0))
						printf("Please enter a valid index : \n");

				}while(!(value3 < 7 && value3 > 0));
				
				Means *overall;
				
				// Criterion : lotarea
				if(value3 == 1)
				{
					overall = mean_sale_prices(houses,"lotarea");
					for (Means *tmp = overall; tmp != NULL; tmp = tmp->next)
					{
						printf("Lotarea : %d\n",tmp->number);
						printf("Average price : %f\n\n",tmp->mean);
					}
				}
				// Criterion : neighborhood
				else if(value3 == 2)
				{
					overall = mean_sale_prices(houses,"neighborhood");
					for (Means *tmp = overall; tmp != NULL; tmp = tmp->next)
					{
						printf("Neighborhood : %s\n",tmp->string);
						printf("Average price : %f\n\n",tmp->mean);
					}
				}
				// Criterion : yearbuilt
				else if(value3 == 3)
				{
					overall = mean_sale_prices(houses,"yearbuilt");
					for (Means *tmp = overall; tmp != NULL; tmp = tmp->next)
					{
						printf("Yearbuilt : %d\n",tmp->number);
						printf("Average price : %f\n\n",tmp->mean);
					}
				}
				// Criterion overallqual
				else if(value3 == 4)
				{
					overall = mean_sale_prices(houses,"overallqual");
					for (int i = 0; i < 10; i++)
					{
						printf("Overallqual : %d\n",i + 1);
						printf("Average price : %f\n\n",overall->okmean[i]);
					}
				}
				// Criterion : overallcond
				else if(value3 == 5)
				{
					overall = mean_sale_prices(houses,"overallcond");
					for (int i = 0; i < 10; i++)
					{
						printf("Overallcond : %d\n",i + 1);
						printf("Average price : %f\n\n",overall->okmean[i]);
					}
				}
				// Criterion : kitchenqual
				else if(value3 == 6)
				{
					overall = mean_sale_prices(houses,"kitchenqual");
					char *kq[5] = {"Ex", "Gd", "TA", "Fa", "Po"};
					for (int i = 0; i < 5; i++)
					{
						printf("Kitchenqual : %s\n", kq[i]);
						printf("Average price : %f\n\n",overall->okmean[i]);
					}
				}
				linked_free(overall);
				break;
			// Listing the 10 most expensive houses
			case 5:
				readcheck(houses, 0, &read, argv[1], &datasize);

				House *arr1 = malloc(datasize*sizeof(House));
				arr1 = hash_to_arr(houses, Nid, N, datasize);
				sort_houses(arr1, "saleprice", datasize);
				for(int i = datasize - 1; i >= datasize - 10; i--)
				{
					print_house(&(arr1[i]));
					printf("\n");
				}
				free(arr1);
				break;
			// Sorting the houses and saving them as csv
			case 6:
				readcheck(houses, 0, &read, argv[1], &datasize);

				char criter[50] = {0};
				int value2;
				do{
					printf("Please choose a criterion : \n");
					printf("1 - lotarea\n");
					printf("2 - neighborhood\n");
					printf("3 - yearbuilt\n");
					printf("4 - overallqual\n");
					printf("5 - overallcond\n");
					printf("6 - kitchenqual\n");
					printf("7 - saleprice\n");
					
					printf("Please enter the criterion's index : ");
					assert(scanf("%d", &value2) > 0);
					
					if(!(value2 < 8 && value2 > 0))
						printf("Please enter a valid index : \n");

				}while(!(value2 < 8 && value2 > 0));
				
				House *arr;

				// Criterion : lotarea
				if(value2 == 1)
				{
					arr = hash_to_arr(houses, Nlotarea, N, datasize);
					sort_houses(arr, "lotarea", datasize);
				}
				// Criterion : neighborhood
				else if(value2 == 2)
				{
					arr = hash_to_arr(houses, Nneighborhood, N, datasize);
					sort_houses(arr, "neighborhood", datasize);	
				}
				// Criterion : yearbuilt
				else if(value2 == 3)
				{
					arr = hash_to_arr(houses, Nyearbuilt, N, datasize);
					sort_houses(arr, "yearbuilt", datasize);	
				}
				// Criterion : overallqual
				else if(value2 == 4)
				{
					arr = hash_to_arr(houses, Noverallqual, overallsize, datasize);
					sort_houses(arr, "overallqual", datasize);		
				}
				// Criterion : overallcond
				else if(value2 == 5)
				{
					arr = hash_to_arr(houses, Noverallcond, overallsize, datasize);
					sort_houses(arr, "overallcond", datasize);
				}
				// Criterion : kitchenqual
				else if(value2 == 6)
				{
					arr = hash_to_arr(houses, Nkitchenqual, kitchensize, datasize);
					sort_houses(arr, "kitchenqual", kitchensize);
				}
				// Criterion : saleprice
				else if(value2 == 7)
				{
					arr = hash_to_arr(houses, Nid, N, datasize);
					sort_houses(arr, "saleprice", datasize);
				}
				
				saveas_csv(arr,datasize,0);
				free(arr);
				printf("Sorting done according to the criterion and a csv file has been saved.\n\n");
				break;
			// Making price prediction with make_prediction()
			case 7:
				readcheck(houses, 0, &read, argv[1], &datasize);
				readcheck(test, 1, &read_test, "dataset/house_price_test.csv", &datatestsize);

				// Mallocing the matrixes
				double **areas = malloc(datasize*sizeof(double*));
				if (!areas) exit(2);
				for(int i = 0; i < datasize; i++) 
				{
					areas[i] = malloc(2*sizeof(double*));
					if (!areas[i]) exit(2);
				}
				
				double **prices = malloc(datasize*sizeof(double*));
				if (!prices) exit(2);
				for(int i = 0; i < datasize; i++) 
				{
					prices[i] = malloc(sizeof(double*));
					if (!prices[i]) exit(2);
				}

				double **X = malloc(datatestsize*sizeof(double*));
				if (!X) exit(2);
				for(int i=0; i<datatestsize; i++) 
				{
					X[i] = malloc(2*sizeof(double*));
					if (!X[i]) exit(2);
				}
				for(int j=0; j<datatestsize; j++)
					X[j][0] = 1;
				
				double **Y = malloc(datatestsize*sizeof(double*));
				if (!Y) exit(2);
				for(int i=0; i<datatestsize; i++) 
				{
					Y[i] = malloc(sizeof(double*));
					if (!Y[i]) exit(2);
				}
				
				// Creating data matrice
				create_data_matrice(houses,areas,prices);
				
				double **W;
				// Finding the linear prediction equation
				W = calculate_parameter(areas,prices,datasize,2,datasize,1); 

				// Making the prediction
				make_prediction(test,X,Y,W,datatestsize);
				
				printf("Price prediction made.\n\n");	

				// Free the hash
				array_free(areas,datasize);		
				array_free(prices,datasize);			
				array_free(X,datatestsize);			
				array_free(Y,datatestsize);			
				array_free(W,2);
				break;
			// Exit
			case 0:
				printf("Thank you for choosing us!\n");
				break;
		}

	}while(value != 0);
	
	// Making price prediction with model_by similarity and saving it to a txt file
	readcheck(houses, 0, &read, argv[1], &datasize);
	readcheck(test, 1, &read_test, "dataset/house_price_test.csv", &datatestsize);

	House *arr = malloc(datatestsize*sizeof(House));
	arr = hash_to_arr(test, Nid, N, datatestsize);
	// Opening the file
	FILE *t = fopen("dataset/prices_by_similarity.txt","w");
	fprintf(t,",Id,SalePrice\n");
	
	for(int i=0; i<datatestsize; i++)
	{
		// Calculating the saleprice
		int salep = model_by_similarity(houses, &(arr[i]));
		// Writing on the file
		if(salep == 0)
			fprintf(t,"%i,%i,%s\n",i,arr[i].id,"NOT ENOUGH DATA");
		else
			fprintf(t,"%i,%i,%i\n",i,arr[i].id,salep);
	}
	free(arr);
	fclose(t);

	hash_free(houses);
	hash_free(test);
	
	return 0;
}

// Checks if the database had been read. If not reads it.
void readcheck(House **housesdata[], int mod, int *check, char *location, int *datasize)
{
	if(*check==0)
	{
		// Reading the data
		read_house_data(housesdata,mod,location);
		*datasize = getsize();
		*check = 1;
	}
}

// Frees hash table
void hash_free(House **houses[])
{
	House *wp = NULL;
	House *p = NULL;
	for(int i=0; i<N; i++)
	{
		wp = houses[0][i];
		while(wp)
		{
			p = wp;
			wp = wp->next[0];
			free(p->street);
			free(p->neighborhood);
			free(p->kitchenqual);
			free(p);
		}
	}
}

// Frees linked list
void linked_free (Means *node)
{
	Means *wp = node, *p = NULL;
	while(wp)
	{
		p = wp;
		wp = wp->next;
		if(p->string != NULL)
			free(p->string);
		free(p);
	}
}