#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main()
{
    StartParse("./test.in");
    printf("%llu\n", GetValueULLInt("A", 10));
    printf("%ld\n", GetValueInt("B", 10));
    printf("%lu\n", GetValueUInt("C", 10));
    printf("%f\n", GetValueFloat("D"));
    printf("%f\n", GetValueDouble("E"));
    EndParse();
    return 0;
}