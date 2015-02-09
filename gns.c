#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "queue.h"
#include "coefficient.c"


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
int* zero_vector;
char** symbols;

struct GNS* create_genus_zero ();
void print_gns (struct GNS* to_print);
long n_choose_k (long N, long K);
int calculate_index(int* array, int n);
int* increase_by_one(int* starting);
int calculate_degree(int* element);
int* add_together (int* first, int* second);
int** create_potential_generators (struct GNS* gns, int missing_index, int* includes_three_v);
void print_vector (int* vector);
char** create_symbols ();
char* int_to_str (int integer);
int** get_min_gen (int** potential_generators, int num_gens, int where_start_new, int* num_min_gen);
int compare2 (int* first, int* second);
int** sorted_generators (int** min_gen, int num_old_gen, int num_total);




int main (int argc, char** argv)
{

	init_and_openlink (argc, argv);

	char** symbols2 = malloc(2*sizeof(char*));
	symbols2[0] = "x1";
	symbols2[1] = "x2";


	int list_length = 3;
	int** exponent_list = malloc(list_length*sizeof(int*));
	
	int* zeroeth = malloc (2*sizeof(int));
	zeroeth[0] = 0;
	zeroeth[1] = 1;
	exponent_list[0] = zeroeth;

	int* first = malloc (2*sizeof(int));
	first[0] = 1;
	first[1] = 0;
	exponent_list[1] = first;

	int* second = malloc (2*sizeof(int));
	second[0] = 1;
	second[1] = 1;
	exponent_list[2] = second;


	d = 10;
	symbols = create_symbols ();
	for (int i = 0; i < d; i++)
	{
		printf ("%s\n", symbols[i]);
	}


	zero_vector = malloc (d*sizeof (int));
	for (int i = 0; i < d; i++)
	{
		zero_vector[i] = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		//printf("coefficient = %d\n", calculate_coefficient (exponent_list, list_length, i, d, symbols2));
	}

	
	// quit Mathematica
	//print_gns(create_genus_zero());
	//initQueue();
	int includes_three;
	int** min_gens;
	int** sorted_gens;
	int** potential_generators;


	struct GNS* genus_zero = create_genus_zero();
	for (int i = genus_zero->index_of_effective; i < genus_zero->num_generators; i++)
	{
		potential_generators = create_potential_generators (genus_zero, i, &includes_three);
		int how_many = 2*(genus_zero->num_generators)-1;
		if (includes_three)
		{
			how_many += 1;
		}
		int num_min_gen = 0;
		min_gens = get_min_gen (potential_generators, how_many, genus_zero->num_generators-1, &num_min_gen);
		//printf ("num min gen: %d\n", num_min_gen);
		sorted_gens = sorted_generators (min_gens, genus_zero->num_generators-1, num_min_gen);

		for (int j = 0; j < num_min_gen; j++)
		{
			//print_vector (min_gens[j]);

		}
		printf ("\n");

	}
	WSPutFunction( lp, "Exit", 0);



	return 0;
}

void print_vector (int* vector)
{
	for (int i = 0; i < d; i++)
	{
		printf("%d ", vector[i]);
	}
	printf("\n");
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


int* add_together (int* first, int* second)
{
	int* to_return = malloc (d*sizeof(int));
	for (int i = 0; i < d; i++)
	{
		to_return[i] = first[i]+second[i];
	}
	return to_return;
}

int** create_potential_generators (struct GNS* gns, int missing_index, int* includes_three_v)
{
	int num_generators = gns->num_generators;
	int** old_gen = gns->generators;
	int frobenius_degree = gns->frobenius_degree;
	int* missing_generator = old_gen[missing_index];
	int degree_missing_gen = calculate_degree (missing_generator);
	int** to_return;
	if (degree_missing_gen == 1)
	{
		*includes_three_v = TRUE;
		to_return = malloc ((2*num_generators)*sizeof (int*));
		// make the last entry 3v
		to_return[2*num_generators-1] = add_together (add_together (missing_generator, missing_generator), missing_generator);

	}
	else
	{
		*includes_three_v = FALSE;
		to_return = malloc ((2*num_generators-1)*sizeof (int*));
	}

	int index = 0;
	for (int i = 0; i < num_generators; i++)
	{
		int* generator = old_gen[i];
		if (i != missing_index)
		{
			to_return[index] = add_together (generator, zero_vector);
			print_vector (to_return[index]);
			index += 1;
		}
		to_return[num_generators-1+i] = add_together (generator, missing_generator);
	}
	
	return to_return;
}


int** get_min_gen (int** potential_generators, int num_gens, int where_start_new, int* num_min_gen)
{
	int** to_return = malloc (num_gens*sizeof (int*));
	for (int i = 0; i < where_start_new; i++)
	{
		to_return[i] = potential_generators[i];
	}

	int index = where_start_new;
	for (int i = where_start_new; i < num_gens; i++)
	{
		//printf ("vector checked: ");
		//print_vector (potential_generators[i]);
		int coefficient = calculate_coefficient (potential_generators, num_gens, i, d, symbols);
		if (coefficient == 0)
		{
			//printf ("ok: ");
			//print_vector (potential_generators[i]);
			to_return[index] = potential_generators[i];
			index++;
		}

	}
	*num_min_gen = index;
	//free (to_return + index);
	return to_return;
}


char** create_symbols ()
{
	char** to_return = malloc (d*sizeof (char*));
	for (int i = 1; i <= d; i++)
	{
		to_return[i-1] = int_to_str (i);
	}
	return to_return;
}

char* int_to_str (int integer)
{
	int size = 1;
	int copy = integer;
	while (copy != 0)
	{
		size += 1;
		copy /= 10;
	}

	int length = size-1;
	char* to_return = malloc ((length+1)*sizeof (char));
	to_return[length] = '\0';
	for (int i = 0; i < length; i++)
	{
		to_return[i] = (char) 97+(integer % 10);
		integer /= 10;
	}
	return to_return;
}

int** sorted_generators (int** min_gen, int num_old_gen, int num_total)
{
	int** to_return = malloc (sizeof ((num_total)*sizeof (int*)));
	int index = 0;
	int old_index = 0;
	int new_index = num_old_gen;
	while (old_index < num_old_gen || new_index < num_total)
	{
		printf ("index: %d %d %d\n", index, old_index, new_index);
		if (old_index == num_old_gen)
		{
			to_return[index] = min_gen[new_index];
			index += 1;
			new_index += 1;
		}
		else if (new_index == num_total)
		{
			to_return[index] = min_gen[old_index];
			index += 1;
			old_index += 1;
		}
		else
		{
			if (compare2 (min_gen[old_index], min_gen[new_index]) == 0)
			{
				to_return[index] = min_gen[old_index];
				old_index += 1;
				index += 1;
			}
			else
			{
				to_return[index] = min_gen[new_index];
				new_index += 1;
				index += 1;
			}
		}
		
	}
	return to_return;
}

int compare2 (int* first, int* second)
{
	// return 0 if first is smaller, 1 if second is smaller
	int first_deg = calculate_degree (first);
	int second_deg = calculate_degree (second);
	if (first_deg < second_deg)
	{
		return 0;
	}
	if (second_deg < first_deg)
	{
		return 1;
	}
	for (int i = 0; i < d; i++)
	{
		if (first[i] < second[i])
		{
			return 0;
		}
		if (second[i] < first[i])
		{
			return 1;
		}
	}
	return -1;
}
