#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "queue.h"

struct GNS
{
	int** generators;
	int num_generators;
	int index_of_effective;
	int* frobenius;
	int** hole_set;
	int num_holes;
	int frobenius_degree;
	struct GNS** children;
};

int d;

struct GNS* create_genus_zero ();
void print_gns (struct GNS* to_print);
long n_choose_k (long N, long K);
int calculate_index(int* array, int n);
int* increase_by_one(int* starting);
int calculate_degree(int* element);
void add_together(int* first, int* second, int** sum_ptr);

int main (int argc, char** argv)
{
	d = 4;
	//print_gns(create_genus_zero());
	initQueue();
	int array[4] = {0,1,0,0};
	d = 4;
	int array1[4] = {1,0,2,3};
	int sum1 = 6;
	int array2[4] = {4,2,2,1};
	int sum2 = 9;
	int array3[4] = {5,2,4,4};
	int sum3 = sum1+sum2;
	//printf("%d %d %d\n", calculate_index(array1, sum1, d), calculate_index(array2, sum2, d), calculate_index(array3, sum3, d));
	int start[4] = {0,4,0,0};
	int* returned = increase_by_one(start, 4);
	for (int i = 0; i < 4; i++)
	{
		printf("%d ", returned[i]);
	}
	printf("\n");
	return 0;
}

struct GNS* create_genus_zero ()
{
	// obviously there will be d generators
	struct GNS* to_return = malloc (sizeof (struct GNS));
	int** generators = malloc (d*sizeof (int*));
	for (int i = d-1; i >= 0; i--)
	{
		int* new_generator = calloc(d, sizeof (int));
		new_generator[i] = 1;
		generators[d-1-i] = new_generator;
	}
	to_return->generators = generators;
	to_return->num_generators = d;
	to_return->index_of_effective = 0;
	to_return->frobenius = NULL;
	to_return->hole_set = NULL;
	to_return->num_holes = 0;
	to_return->frobenius_degree = 0;
	to_return->children = malloc (d*sizeof (struct GNS*));
	return to_return;
}

void print_gns (struct GNS* to_print)
{
	int num_generators = to_print->num_generators;
	int** generators = to_print->generators;
	int index_of_effective = to_print->index_of_effective;
	int* frobenius = to_print->frobenius;
	int frobenius_degree = to_print->frobenius_degree;
	int num_holes = to_print->num_holes;
	int** hole_set = to_print->hole_set;

	// first print the number of generators
	printf ("%d\n", num_generators);
	
	// then print each generator
	for (int i = 0; i < num_generators; i++)
	{
		for (int j = 0; j < d; j++)
		{
			printf ("%d ", generators[i][j]);
		}
		printf ("\n");
	}
	
	// then print index of effective generator
	printf ("%d\n", index_of_effective);

	// frobenius element
	if (frobenius == NULL)
	{
		printf ("N\n");
	}
	else 
	{
		for (int i = 0; i < d; i++)
		{
			printf ("%d ", frobenius[i]);
		}
		printf ("\n");
	}

	// frobenius order
	printf ("%d\n", frobenius_degree);

	// number of holes
	printf ("%d\n", num_holes);

	// the holes
	for (int i = 0; i < num_holes; i++)
	{
		for (int j = 0; j < d; j++)
		{
			printf ("%d ", hole_set[i][j]);
		}
		printf ("\n");
	}
}


long n_choose_k (long N, long K)
{
    // This function gets the total number of unique combinations based upon N and K.
    // N is the total number of items.
    // K is the size of the group.
    // Total number of unique combinations = N! / ( K! (N - K)! ).
    // This function is less efficient, but is more likely to not overflow when N and K are large.
    // Taken from:  http://blog.plover.com/math/choose.html
    //
    long r = 1;
    long d;
    if (K > N || K < -1) return 0;
    for (d = 1; d <= K; d++)
    {
        r *= N--;
        r /= d;
    }
    return r;
}

int calculate_index(int* array, int n)
{
	int total = 0;
	int subtotal = 0;
	for (int i = 0; i < d; i++)
	{
		int entry = array[i];
		for (int j = 0; j < entry; j++)
		{
			total += n_choose_k(n-subtotal-j+d-i-1-1, n-subtotal-j);
		}
		subtotal += entry;
	}
	return total;
}

int* increase_by_one(int* starting)
{
	int* to_return = malloc(d*sizeof(int));
	for (int i = 0; i < d; i++)
	{
		to_return[i] = starting[i];
	}
	for (int i = d-1; i >= 0; i--)
	{
		if (starting[i] != 0)
		{
			to_return[i-1] += 1;
			to_return[d-1] = to_return[i]-1;
			to_return[i] = 0;
			return to_return;
		}
	}
}

int calculate_degree(int* element)
{
	total = 0;
	for (int i = 0; i < d; i++)
	{
		total += element[i];
	}
	return total;
}


void add_together(int* first, int* second, int** sum_ptr)
{
	int* sum = *sum_ptr;
	for (int i = 0; i < d; i++)
	{
		sum[i] = first[i]+second[i];
	}
}




struct GNS* next_gns (struct GNS* gns)
{
	// you need to figure out the Frobenius order, then go up to 2*F_o + 1
	// for a given order, a, you need
	int frobenius_degree = gns->frobernius_degree;
	int num_holes = gns->num_holes;
	int** holes = gns->holes;
	int* hole_set_degree_start = malloc ((frobenius_degree+1)*sizeof (int));
	int* hole_set_indicies = malloc(num_holes*sizeof(int));
	int where_last = 0;
	
	// you can put a bound on how many new generators you'll need
	int** new_generators = malloc (2*(gns->num_generators)*sizeof (int*));
	int new_gen_index = 0;
	for (int i = 0; i < num_holes; i++)
	{
		int of_interest = calculate_degree(holes[i]);
		hole_set_indices[i] = of_interest;
		for (int j = where_last; j < of_interest; j++)
		{
			hole_set_degree_start[j] = i;
		}
		where_last = of_interest;
	}
	for (int l = 1; l <= 2*frobenius_degree+1; l++)
	{
		int how_many_l = n_choose_k(l+d-1, l);
		int* list_l = calloc(how_many_l, sizeof(int));
		// only need to remove holes if l <= frobenius_degree


		for (int k = 1; k < (int) floor(k/2.0); k++)
		{
			int* one_before_k = malloc(d*sizeof(int));
			for (int i = 0; i < d-1; i++)
			{
				one_before_k[i] = 0;
			}
			one_before_k[d-1] = k;
			int how_many_k = n_choose_k(k+d-1, k);
			int** list_k = malloc(how_many_k*sizeof(int*));
			list_k[0] = one_before_k;
			for (int a = 1; a < how_many_k; a++)
			{
				int* returned = increase_by_one(one_before_k);
				list_k[a] = returned;
				one_before_k = returned;
			}

			int hole_start = hole_set_degree_start[k];
			int where_stop = (k != frobenius_degree) ? hole_set_degree_start[k+1] : num_holes-1;
			for (; hole_start < where_stop; hole_start++)
			{
				free(list_k[hole_set_indices[hole_start]]);
				list_k[hole_set_indices[hole_start]] = NULL;
			}

			int comp_k = l-k;
			int* one_before_comp_k = malloc(d*sizeof(int));
			for (int i = 0; i < d-1; i++)
			{
				one_before_comp_k[i] = 0;
			}
			one_before_comp_k[d-1] = comp_k;
			int how_many_comp_k = n_choose_k(comp_k+d-1, comp_k);
			int** list_comp_k = malloc(how_many_comp_k*sizeof(int*));
			list_comp_k[0] = one_before_comp_k;
			for (int a = 1; a < how_many_comp_k; a++)
			{
				int* returned = increase_by_one(one_before_comp_k);
				list_comp_k[a] = returned;
				one_before_comp_k = returned;
			}

			if (comp_k <= frobenius_degree)
			{
				int hole_start = hole_set_degree_start[comp_k];
				int where_stop = (comp_k != frobenius_degree) ? hole_set_degree_start[comp_k] : num_holes-1;
				for (; hole_start < where_stop; hole_start++)
				{
					free (list_comp_k[hole_set_indices[hole_start]]);
					list_comp_k[hole_set_indices[hole_start]] = NULL;
				}
			}

			// ok, so now we can go through the two lists
			int* catcher = malloc (d*sizeof(int));
			for (int m = 0; m < how_many_k; m++)
			{
				int* of_interest = list_k[m];
				if (of_interest != NULL)
				{
					for (int n = 0; n < how_many_comp_k; n++)
					{
						int* of_interest2 = list_comp_k[n];
						if (of_interest2 != NULL)
						{
							add_together(of_interest, of_interest2, &catcher);
							list_l[calculate_index(catcher, l)] = 1;
						}
					}
				}
			}

			// need to free
			for (int i = 0; i < how_many_k; i++)
			{
				free (list_k[i]);
			}
			free(list_k);

			for (int i = 0; i < how_many_comp_k; i++)
			{
				free (list_comp_k[i]);
			}
			free(list_comp_k);
		}
		
		if (l <= frobenius_order)
		{
			int hole_start = hole_set_degree_start[l];
			int where_stop = (l != frobenius_degree) ? hole_set_degree_start[l+1] : num_holes-1;
			for (; hole_start < where_stop; hole_start++)
			{
				list_l[hole_set_indices[hole_start]] = 1;
			}
		}


		int* first_l = malloc(d*sizeof(int));
		for (int i = 0; i < d-1; i++)
		{
			first_l[i] = 0
		}
		first_l[d-1] = l;
		if (list_l[0] == 0)
		{
			int* to_insert = malloc(d*sizeof(int));
			for (int i = 0; i < d; i++)
			{
				to_insert[i] = first_l[i];
			}
			new_generators[new_gen_index] = to_insert;
			new_gen_index += 1;
		}
		int* last_one = first_l;

		for (int i = 1; i < how_many_l; i++)
		{
			int* potential_generator = increase_by_one(last_one, l);
			if (list_l[i] == 0)
			{
				int* to_insert = malloc(d*sizeof(int));
				for (int j = 0; j < d; j++)
				{
					to_insert[j] = potential_generator[j];
				}
				new_generators[new_gen_index] = to_insert;
				new_gen_index += 1;
			}
			free (last_one);
			last_one = potential_generator;
		}

		// so now you have it for l
	}
	// you've now done it for all l

	// new_generators is complete. you did it!

	
}
