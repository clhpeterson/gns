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
static void closelink( void);
static void deinit( void);
int calculate_coefficient (int** exponent_list, int list_length, int missing_index, int e, char** symbols);
