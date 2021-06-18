#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"dataset.h"

// Finding the datasize dynamically
int datasize = 0;

// Gets the data size for a more dynamic usage
int getsize()
{
	return datasize;
}

// Hash function for string type datas
unsigned int hashstring(unsigned char *str)
{
   unsigned long hash = 0;
   while (*str)
    {
        hash= hash*31+ *str ;
        str ++;
    }
    return hash % N;
}

// Hash function for integer type datas
unsigned int hashint(int nbr)
    {
        return nbr % N;
    }	

// The function that turns kitchenqual data to indexes
unsigned int kitchentoint(char *kitchen)
{
	int qual; 
	if(strcmp(kitchen, "Ex")==0)
		qual = Ex;
	else if(strcmp(kitchen, "Gd")==0)
		qual = Gd;
	else if(strcmp(kitchen, "TA")==0)
		qual = TA;
	else if(strcmp(kitchen, "Fa")==0)
		qual = Fa;
	else if(strcmp(kitchen, "Po")==0)
		qual = Po;
	else 
		printf("KITCHENTOINT ERROR!");
	
	return qual;
}

// Printing a given houses data
void print_house(House *house)
{
    printf("ID : %d\n", house->id);
    printf("Lot Area : %d\n", house->lotarea);
    printf("Street : %s\n", house->street);
    printf("Sale Price : %d\n", house->saleprice);
    printf("Neighborhood: %s\n", house->neighborhood);
    printf("Built year : %d\n", house->yearbuilt);
    printf("Overall Quality : %d\n", house->overallqual);
    printf("Overall Condition : %d\n", house->overallcond);
    printf("Kitchen Quality : %s\n", house->kitchenqual);
}

// Finding the house from its id
House* get_house_byid(int id, House **houses[])
{
    unsigned int i = hashint(id);
	
    // Iterating through the linked list
    for(House *tmp = houses[Nid][i]; tmp != NULL; tmp = tmp->next[Nid])
    {
        if (tmp->id == id)
            return tmp;
    }
    return 0;
}

// Finding the houses that are in the same neighborhood as a given house
House* get_neighborhoods(House **houses[], House *house)
{  
    // returning the whole node
    unsigned int i = hashstring(house->neighborhood);
    // houses is hashed with the neighborhood criterion	
    return houses[Nneighborhood][i];
}

// The function that reads data from the csv file to memory
// mod "0" for house_price_data.csv
// mod "1" for house_price_test.csv
void read_house_data(House **houses[], int mod, char *locatin)
{
    // Opening the file
    FILE *f = fopen(locatin , "r");
    if (!f) exit(1);
	
	datasize = 0;
    char line[LINE];
	int k;
	if(mod == 0)
		k = 0;
	else
		k = 1;

    // Skipping the first line
    char* skip = fgets(line, sizeof(line), f);
    while(fgets(line, sizeof(line), f))
    {
		
		House *house = malloc(sizeof(House));
		if (!house) exit(2);
		
        // Segregation of the values
        char *token = strtok(line, ",");
        for(int i = 0; i < 10 - k; i++)
        {	
            if (i == 1)
                house->id = atoi(token);
            // If mod is '0' saleprice is skipped
            else if (i == 2 && mod == 0)
                house->saleprice = atoi(token);
			else if (i == 3 - k)
				house->lotarea = atoi(token);
			else if (i == 4 - k)
                house->street = strdup(token) ;
			else if (i == 5 - k)
				house->overallqual = atoi(token);
			else if (i == 6 - k)
				house->overallcond = atoi(token);  
			else if (i == 7 - k)
                house->kitchenqual = strdup(token) ;
            else if (i == 8 - k)
                house->yearbuilt = atoi(token);
            else if (i == 9 - k)
			{
				token = strtok(token, "\n");
				house->neighborhood = strdup(token);
			}
            token = strtok(NULL, ",");
				
        }
		
		// Hashing for id		
		unsigned int idh = hashint(house->id);
		house->next[Nid] = houses[Nid][idh];
		houses[Nid][idh] = house;

        // Hashing for overallqual		
		house->next[Noverallqual] = houses[Noverallqual][house->overallqual - 1];
		houses[Noverallqual][house->overallqual - 1] = house;			

        // Hashing for overallcond
		house->next[Noverallcond] = houses[Noverallcond][house->overallcond - 1];
		houses[Noverallcond][house->overallcond - 1] = house;
		
		// Hashing for kitchenqual
		house->next[Nkitchenqual] = houses[Nkitchenqual][kitchentoint(house->kitchenqual)];
		houses[Nkitchenqual][kitchentoint(house->kitchenqual)] = house;
		
		// Hashing for yearbuilt
		idh = hashint(house->yearbuilt);
		house->next[Nyearbuilt] = houses[Nyearbuilt][idh];
		houses[Nyearbuilt][idh] = house;
		
		// Hashing for neighborhood
		idh = hashstring(house->neighborhood);
		house->next[Nneighborhood] = houses[Nneighborhood][idh];
		houses[Nneighborhood][idh] = house;
		
		// Hashing for lotarea
		idh = hashint(house->lotarea);
		house->next[Nlotarea] = houses[Nlotarea][idh];
		houses[Nlotarea][idh] = house;
		
        // Calculating the datasize
		datasize++; 
    }
    fclose(f);
}

// Finding the mean sale prices for overallqual, overallcond and kitchenqual parameters
Means* mean_of_qual(House **houses[], int Ncriter)
{
    unsigned int cnt = 0;
	Means *node = calloc(1,sizeof(Means));
    if (!node) exit(3);
	int size;
	if(Ncriter == Noverallqual || Ncriter == Noverallcond)
		size = 10;
	else 
        // size for kitchenqual
		size = 5;
	
    for(int i = 0; i < size; i++)
    {
        for(House* tmp = houses[Ncriter][i]; tmp != NULL; tmp = tmp->next[Ncriter])
            node->okmean[i] = (node->okmean[i]*(cnt) + tmp->saleprice)/(++cnt);							
        cnt = 0;
    }
    return node;
}

// Finding the mean sale prices for yearbuilt, lotarea and neighborhood parameters
Means* mean_of_data(House **houses[], int Ncriter, int mptr)
{
    unsigned int cnt = 0;
    Means *node = NULL; 
    int present;
    for(int i = 0; i < N; i++)
    {			
        for(House *tmp = houses[Ncriter][i]; tmp != NULL; tmp = tmp->next[Ncriter])
        {
            present = 0;
            for(Means *temp = node; temp != NULL; temp = temp->next)
            {
                if(mptr == NBH)
                {
                    // The comparison for neighborhood
                    if(strcmp(temp->string, tmp->neighborhood) == 0)
					{
						temp->mean = (temp->mean*temp->cnt + tmp->saleprice)/(++(temp->cnt));	
						present = 1;
						break;
					}
                }
                else
                {
                    // The comparison for lotarea and yearbuilt with mptr
                    if(temp->number == *(&(tmp->id) + mptr))
                    {
                        temp->mean = (temp->mean*temp->cnt + tmp->saleprice)/(++(temp->cnt));	
                        present = 1;
                    }
                }
            } 
            if(present == 0) 
            {
                Means *new = calloc(1, sizeof(Means));
                if (!new) exit(3);
                if(mptr == NBH)
                    new->string = strdup(tmp->neighborhood);
                else
					new->number = *(&(tmp->id) + mptr);
                new->mean = (new->mean*new->cnt + tmp->saleprice)/(++(new->cnt));	
                new->next = node;
                node = new;
            }		
        }	
    }
    return node;
}

// The function that finds the mean sale prices of a given data type
Means* mean_sale_prices(House** houses[] , char* criter_name)
{	
	if(strcmp(criter_name, "overallqual") == 0)
        mean_of_qual(houses, Noverallqual);
	else if(strcmp(criter_name, "overallcond") == 0)
        mean_of_qual(houses, Noverallcond);
	else if(strcmp(criter_name, "kitchenqual") == 0)
        mean_of_qual(houses, Nkitchenqual);
	else if(strcmp(criter_name, "yearbuilt") == 0)
        mean_of_data(houses, Nyearbuilt, YB);
	else if(strcmp(criter_name, "lotarea") == 0)
        mean_of_data(houses, Nlotarea, LOT);
	else if(strcmp(criter_name, "neighborhood") == 0)
        mean_of_data(houses, Nneighborhood, NBH);
}

// The function that saves data to a new csv file
void saveas_csv(House *house, int datas, int mod) 
{
    int i;
    FILE *f;
    // For house_price_data.csv
    if (mod == 0)
    {
        f = fopen("dataset/sorted_data.csv", "w");
        if (!f) exit(1);
        fprintf(f,",Id,SalePrice,LotArea,Street,OverallQual,OverallCond,KitchenQual,YearBuilt,Neighborhood\n");
        for(i=0; i<datas; i++)
            fprintf(f,"%i,%i,%i,%i,%s,%i,%i,%s,%i,%s\n",i,house[i].id,house[i].saleprice,
                    house[i].lotarea,house[i].street,house[i].overallqual,house[i].overallcond,
                    house[i].kitchenqual,house[i].yearbuilt,house[i].neighborhood);
    }
    // For house_price_test.csv
    else if (mod == 1)
    {
        f = fopen("dataset/price_predictions.csv", "w");
        if (!f) exit(1);
        fprintf(f,",Id,SalePrice\n");
        for(i=0; i<datas; i++)
            fprintf(f,"%i,%i,%i\n",i,house[i].id,house[i].saleprice);
    }
	fclose(f);
}

// The funciton that turns a hash table to an array
House* hash_to_arr(House **houses[], int n, int ctr, int datas)
{
    House *arr = malloc(datas*sizeof(House));
    if (!arr) exit(2);
    int j = 0;
    for(int i=0; i<ctr; i++)
    {
        for(House *tmp = houses[n][i]; tmp != NULL; tmp = tmp->next[n])
            arr[j++] = *tmp;		
    }
    return arr;
}

// Merging function
void merge(House* house, int low, int middle, int high, int mptr)
{
    int nL = middle - low + 1;
    int nR = high - middle;
    House left[nL], right[nR];

    for(int i=0; i<nL; i++) left[i] = house[low+i];
    for(int j=0; j<nR; j++) right[j] = house[middle+1+j];

    int i=0, j=0, k=low;
    while(i<nL && j<nR)
    {
        // Comparison for neighborhood
        if (mptr == NBH && strcmp(left[i].neighborhood, right[j].neighborhood) < 0)
            house[k++] = left[i++];
        // Comparison for all of the other parameters
        else if(mptr != NBH && *(&(left[i].id) + mptr) < *(&(right[j].id) + mptr))
            house[k++] = left[i++];
        else
            house[k++] = right[j++];
    }
    while(i<nL)
        house[k++] = left[i++];
    while(j<nR)
        house[k++] = right[j++];
}

// Merge sort function
void merge_sort(House* house, int low, int high, int mptr)
{
    if (low < high)
    {
        int mid = low + (high-low)/ 2;
        merge_sort(house, low, mid, mptr);
        merge_sort(house, mid+1, high, mptr);
        merge(house,low,mid,high,mptr);
    }
}

// The function that sorts the data according to a given parameter
// houses[] array is given with the hash_to_arr function in the main()
void sort_houses(House houses[], char *criter_name, int datas)
{
    if(strcmp(criter_name, "lotarea") == 0)
        merge_sort(houses, 0, datas-1, LOT);
    else if(strcmp(criter_name, "neighborhood") == 0)
        merge_sort(houses, 0, datas-1, NBH);
    else if(strcmp(criter_name, "yearbuilt") == 0)
        merge_sort(houses, 0, datas-1, YB);
    else if(strcmp(criter_name, "overallqual") == 0)
        merge_sort(houses, 0, overallsize-1, OQ);
    else if(strcmp(criter_name, "overallcond") == 0)
        merge_sort(houses, 0, overallsize-1, OC);
    else if(strcmp(criter_name, "kitchenqual") == 0)
        merge_sort(houses, 0, kitchensize-1, KQ);
    else if(strcmp(criter_name, "saleprice") == 0)
        merge_sort(houses, 0, datas-1, SP);
}

