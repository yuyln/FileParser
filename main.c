#include <stdio.h>
#include <stdlib.h>
#define PARSER_IMPLEMENTATION
#include "parser.h"

int main(int argc, char **argv)
{
    int qnt;
    FILE *f = fopen("test.in", "rb");
    char *o = ReadFile(f);
    fclose(f);
    char **parsed = Parse(o, &qnt);
    unsigned int A = GetValueUInt("A", parsed, qnt);
    int B = GetValueInt("B", parsed, qnt);
    double D = GetValueDouble("D", parsed, qnt);
    float E = GetValueFloat("E", parsed, qnt);
    printf("%u\n", A);
    printf("%d\n", B);
    printf("%f\n", D);
    printf("%f\n", E);
    float T = GetValueFloat("T", parsed, qnt);
    printf("%f\n", T);
    return 0;
}