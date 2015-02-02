
/* To run this program use the command-line below:
 *	Unix:           quotient -linkname "math -mathlink"
 *	Mac or Windows: quotient -linkmode launch
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
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
	d = 2;
	//print_gns(create_genus_zero());
	initQueue();

	struct GNS* genus_zero = create_genus_zero();



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
	return to_return;
}

int calculate_degree(int* element)
{
	int total = 0;
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

