/* To run this program use the command-line below:
 *	Unix:           quotient -linkname "math -mathlink"
 *	Mac or Windows: quotient -linkmode launch
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mathlink.h"

#define TRUE 1
#define FALSE 0

#define SPECIAL_INCREMENT(a, b) ((a == b+1) ? b+=2 : b++)
#define SPECIAL_START(a) ((a == 0) ? 1 : 0)

static void init_and_openlink( int argc, char* argv[]);
static void closelink( void);
static void deinit( void);
int calculate_coefficient (int** exponent_list, int list_length, int missing_index, int e, char** symbols);

MLENV ep = (MLENV)0;
MLINK lp = (MLINK)0;

/*
int main( int argc, char* argv[])
{
	int pkt, m, n, q;

	init_and_openlink (argc, argv);

	char** symbols = malloc(2*sizeof(char*));
	symbols[0] = "x1";
	symbols[1] = "x2";

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



	int d = 2;
	for (int i = 0; i < 3; i++)
	{
		printf("coefficient = %d\n", calculate_coefficient (exponent_list, list_length, i, d, symbols));
	}

	
	// quit Mathematica
	MLPutFunction( lp, "Exit", 0);

	return 0;
}
*/


int calculate_coefficient (int** exponent_list, int list_length, int missing_index, int e, char** symbols)
{
	int coefficient, pkt;
	// need to make the symbols list

	int* exponent = exponent_list[missing_index];

	MLPutFunction (lp, "EvaluatePacket", 1L);
		MLPutFunction (lp, "SeriesCoefficient", e+1);
			MLPutFunction (lp, "Times", list_length-1);
			for (int i = SPECIAL_START(missing_index); i < list_length; SPECIAL_INCREMENT(missing_index, i))
			{
				MLPutFunction (lp, "Divide", 2);
					MLPutInteger (lp, 1);
					MLPutFunction (lp, "Subtract", 2);
						MLPutInteger (lp, 1);
						MLPutFunction (lp, "Times", e);
							for (int j = 0; j < e; j++)
							{
								MLPutFunction (lp, "Power", 2);
									MLPutSymbol (lp, symbols[j]);
									MLPutInteger (lp, exponent_list[i][j]);
							}
			}
			for (int i = 0; i < e; i++)
			{
				MLPutFunction (lp, "List", 3);
					MLPutSymbol (lp, symbols[i]);
					MLPutInteger (lp, 0);
					MLPutInteger (lp, exponent[i]);
			}
	MLEndPacket (lp);

	/* skip any packets before the first ReturnPacket */
	while( (pkt = MLNextPacket (lp), pkt) && pkt != RETURNPKT)
		MLNewPacket (lp);

	/* inside the ReturnPacket we expect an integer */
	MLGetInteger (lp, &coefficient);

	return coefficient;
}


static void deinit( void)
{
	if( ep) MLDeinitialize( ep);
}


static void closelink( void)
{
	if( lp) MLClose( lp);
}


static void init_and_openlink( int argc, char* argv[])
{
#if MLINTERFACE >= 3
	int err;
#else
	long err;
#endif /* MLINTERFACE >= 3 */

	ep =  MLInitialize( (MLParametersPointer)0);
	if( ep == (MLENV)0) exit(1);
	atexit( deinit);

#if MLINTERFACE < 3
	lp = MLOpenArgv( ep, argv, argv + argc, &err);
#else
	lp = MLOpenArgcArgv( ep, argc, argv, &err);
#endif
	if(lp == (MLINK)0) exit(2);
	atexit( closelink);
}
