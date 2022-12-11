#include <stdio.h>
#include <stdlib.h>
#define _PARSER_IMPLEMENTATION
#include "parser.h"

int main()
{
    ParserContext ctx = InitContext("= \r\n:");
    StartParse("./test.in", &ctx);
    printf("%llu\n", GetValueULLInt("A", 10, 0, &ctx));
    printf("%ld\n", GetValueInt("B", 10, 0, &ctx));
    printf("%lu\n", GetValueUInt("C", 10, 0, &ctx));
    printf("%f\n", GetValueFloat("D", 0.0, &ctx));
    printf("%f\n", GetValueDouble("E", 0.0f, &ctx));
    printf("%s\n", ctx.file_name);
    EndParse(&ctx);
    return 0;
}