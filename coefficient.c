/* To run this program use the command-line below:
 *	Unix:           quotient -linkname "math -mathlink"
 *	Mac or Windows: quotient -linkmode launch
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "wstp.h"

#define TRUE 1
#define FALSE 0

#define SPECIAL_INCREMENT(a, b) ((a == b+1) ? b+=2 : b++)
#define SPECIAL_START(a) ((a == 0) ? 1 : 0)

static void init_and_openlink( int argc, char* argv[]);
int calculate_coefficient (int** exponent_list, int list_length, int missing_index, int d, char** symbols);


WSENV ep = (WSENV)0;
WSLINK lp = (WSLINK)0;

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
	WSPutFunction( lp, "Exit", 0);

	return 0;
}
*/


int calculate_coefficient (int** exponent_list, int list_length, int missing_index, int d, char** symbols)
{
	int coefficient, pkt;
	// need to make the symbols list

	int* exponent = exponent_list[missing_index];

	WSPutFunction (lp, "EvaluatePacket", 1L);
		WSPutFunction (lp, "SeriesCoefficient", d+1);
			WSPutFunction (lp, "Times", list_length-1);
			for (int i = SPECIAL_START(missing_index); i < list_length; SPECIAL_INCREMENT(missing_index, i))
			{
				WSPutFunction (lp, "Divide", 2);
					WSPutInteger (lp, 1);
					WSPutFunction (lp, "Subtract", 2);
						WSPutInteger (lp, 1);
						WSPutFunction (lp, "Times", d);
							for (int j = 0; j < d; j++)
							{
								WSPutFunction (lp, "Power", 2);
									WSPutSymbol (lp, symbols[j]);
									WSPutInteger (lp, exponent_list[i][j]);
							}
			}
			for (int i = 0; i < d; i++)
			{
				WSPutFunction (lp, "List", 3);
					WSPutSymbol (lp, symbols[i]);
					WSPutInteger (lp, 0);
					WSPutInteger (lp, exponent[i]);
			}
	WSEndPacket (lp);

	/* skip any packets before the first ReturnPacket */
	while( (pkt = WSNextPacket (lp), pkt) && pkt != RETURNPKT)
		WSNewPacket (lp);

	/* inside the ReturnPacket we expect an integer */
	WSGetInteger (lp, &coefficient);

	return coefficient;
}


static void deinit( void)
{
	if( ep) WSDeinitialize( ep);
}


static void closelink( void)
{
	if( lp) WSClose( lp);
}


static void init_and_openlink( int argc, char* argv[])
{
#if WSINTERFACE >= 3
	int err;
#else
	long err;
#endif /* WSINTERFACE >= 3 */

	ep =  WSInitialize( (WSParametersPointer)0);
	if( ep == (WSENV)0) exit(1);
	atexit( deinit);

#if WSINTERFACE < 3
	lp = WSOpenArgv( ep, argv, argv + argc, &err);
#else
	lp = WSOpenArgcArgv( ep, argc, argv, &err);
#endif
	if(lp == (WSLINK)0) exit(2);
	atexit( closelink);
}
