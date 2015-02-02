/* mlcall.c */

#include <mathlink.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MLENV env;
    MLINK link;
    int errno;
    int packet;

    env = MLInitialize(0);

    /* consider using MLOpenString instead here: */    
    link = MLOpenArgcArgv(env, argc, argv, &errno);

    MLActivate(link);

    /* send 42+137 using the full form Plus[42, 137] */
    MLPutFunction(link, "Plus", 2);
    MLPutInteger(link, 42);
    MLPutInteger(link, 137);
    MLEndPacket(link);

    /* get packets until we find a ReturnPacket or error */
    while ((packet = MLNextPacket(link)) && packet != RETURNPKT)
        MLNewPacket(link);

    if (MLError(link))
        printf("error.\n");
    else {
        int result;
        MLGetInteger(link, &result); /* we know that the result is an integer in this case */
        printf("result: %d\n", result);
    }

    return 0;
}