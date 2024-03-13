#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"

int main(int argc, char *argv[])
{
    initSignalHandlers();
    init_clocks();
    exit(EXIT_SUCCESS);
}