#ifndef _DATASET_H
#define _DATASET_H
#define LINE 1000
#define N 1000

#define Nsize               9
#define Nid					0
#define Nlotarea			1
#define Nstreet				2
#define Nneighborhood		3
#define Nyearbuilt			4
#define Noverallqual		5		
#define Noverallcond		6	
#define Nkitchenqual		7
#define Nsaleprice          8

#define overallsize 		10
#define kitchensize 		5

#define Ex      0
#define Gd      1
#define TA      2
#define Fa      3
#define Po      4

// The operations on the right are being used to iterate over the datas in a struct
#define LOT     1   // *(&(arr[i].id) + 1)
#define YB      2   // *(&(arr[i].id) + 2)
#define OQ      3   // *(&(arr[i].id) + 3)
#define OC      4   // *(&(arr[i].id) + 4)
#define SP    	5	// *(&(arr[i].id) + 5)
#define KQ      6   // *(&(arr[i].id) + 6)
#define NBH     0

// House struct
typedef struct house {
    int id;
    int lotarea;
    int yearbuilt;
    int overallqual;
    int overallcond;
    int saleprice;
	char* kitchenqual;
    char* neighborhood;
    char* street;
    struct house *next[Nsize];
} House;

typedef struct means {
	char* string;
	int number;
    // Holds the (overalcond-overalcual-kitchencual) saleprice averages
	float okmean[10];
	float mean;
	int cnt;
	struct means * next;
} Means;

int getsize();
unsigned int hashstring(unsigned char *str);
unsigned int hashint(int nbr);
unsigned int kitchentoint(char *kitchen);
void read_house_data(House **houses[], int mod, char *locatin);
void print_house(House *house);
House* get_house_byid(int id, House **houses[]);
House* get_neighborhoods(House **houses[], House *house);
Means* mean_of_qual(House **houses[], int Ncriter);
Means* mean_of_data(House **houses[], int Ncriter, int mptr);
Means* mean_sale_prices(House** houses[] , char* criter_name);
void saveas_csv(House *house, int datas, int mod);
House* hash_to_arr(House **houses[], int n, int ctr, int datas);
void merge(House* house, int low, int middle, int high, int mptr);
void merge_sort(House* house, int low, int high, int mptr);
void sort_houses(House houses[], char *criter_name, int datas);

#endif // _DATASET_H
