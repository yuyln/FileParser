#ifndef __PARSER_H
#define __PARSER_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

#ifdef _MSC_VER
#define strtok_r strtok_s
#endif

typedef struct ParserContext ParserContext;

struct ParserContext
{
    char* seps;
    char* file_str;
    char* file_name;
    char** state;
    size_t n;
    size_t file_size;
};

static ParserContext global_parser_context = {" \r\n:=", NULL, NULL, NULL, 0, 0};

ParserContext InitContext(char* separators);
void EndParse(ParserContext* context);
void StartParse(const char* file_path, ParserContext* context);
long int FindIndexOfTag(const char* tag, ParserContext* context);
double GetValueDouble(const char* tag, double def, ParserContext* context);
float GetValueFloat(const char* tag, float def, ParserContext* context);
long int GetValueInt(const char* tag, int base, long int def, ParserContext* context);
unsigned long int GetValueUInt(const char* tag, int base, unsigned long int def, ParserContext* context);
unsigned long long int GetValueULLInt(const char* tag, int base, unsigned long long int def, ParserContext* context);

#endif //PARSER_H

#ifdef _PARSER_IMPLEMENTATION

static char* CopyString(char *from)
{
    size_t from_size = strlen(from);
    char lchar = from[from_size - 1];
    if (lchar != '\0') from_size++;

    char* ret = (char*)calloc(1, from_size);

    if (lchar != '\0') from_size--;
    memcpy((void*)ret, (void*)from, from_size);

    return ret;
}

static FILE* pfopen(const char* path, const char* mode, int exit_)
{
    FILE* file = fopen(path, mode);
    if (!file)
    {
        fprintf(stderr, "Couldn't open file %s: %s\n", path, strerror(errno));
        if (exit_) exit(exit_);
    }
    return file;
}

ParserContext InitContext(char* separators)
{
    ParserContext ret;
    memset(&ret, 0, sizeof(ParserContext));
    ret.seps = CopyString(separators);
    return ret;
}

void EndParse(ParserContext* context)
{
    if (!context)
    {
        free(global_parser_context.file_str);
        free(global_parser_context.file_name);
        free(global_parser_context.state);
        global_parser_context.n = global_parser_context.file_size = 0;
        global_parser_context.file_str = global_parser_context.file_name = NULL;
        global_parser_context.state = NULL;
        return;
    }

    free(context->seps);
    free(context->file_str);
    free(context->file_name);
    free(context->state);
    memset((void*)context, 0, sizeof(ParserContext));
}

void StartParse(const char* file_path, ParserContext* context)
{
    if (!context)
    {
        StartParse(file_path, &global_parser_context);
        return;
    }

    if (context->file_str || context->state || context->n || context->file_name) EndParse(context);

    FILE *file = pfopen(file_path, "rb", 0);
    fseek(file, 0, SEEK_SET);
    fseek(file, 0, SEEK_END);

    context->file_size = (size_t)ftell(file);
    context->file_str = (char*)malloc(context->file_size + 1);
    context->file_name = CopyString((char*)file_path);

    fseek(file, 0, SEEK_SET);
    fread(context->file_str, 1, context->file_size, file);
    context->file_str[context->file_size] = '\0';
    fclose(file);

    char* local_parser_file_str = strdup(context->file_str);
    char* token = strtok(local_parser_file_str, context->seps);

    while (token)
    {
        ++(context->n);
        token = strtok(NULL, context->seps);
    }

    free(local_parser_file_str);

    context->state = (char**)malloc(sizeof(char*) * context->n);

    token = strtok(context->file_str, context->seps);
    size_t i = 0;
    while (token)
    {
        context->state[i++] = token;
        token = strtok(NULL, context->seps);
    }
}

long int FindIndexOfTag(const char* tag, ParserContext* context)
{
    if (!context) return FindIndexOfTag(tag, &global_parser_context);

    for (size_t i = 0; i < context->n; ++i)
        if (!strcmp(tag, context->state[i]))
            return i;
            
    return -1;
}

double GetValueDouble(const char* tag, double def, ParserContext* context)
{
    if (!context) return GetValueDouble(tag, def, &global_parser_context);

    long int i_tag = FindIndexOfTag(tag, context);
    if (i_tag < 0)
    {
        fprintf(stderr, "Could not find tag %s using default value %f\n", tag, def);
        return def;
    }
    return strtod(context->state[i_tag + 1], NULL);
}

float GetValueFloat(const char* tag, float def, ParserContext* context)
{
    if (!context) return GetValueFloat(tag, def, &global_parser_context);
    long int i_tag = FindIndexOfTag(tag, context);
    if (i_tag < 0)
    {
        fprintf(stderr, "Could not find tag %s using default value %f\n", tag, def);
        return def;
    }
    return strtof(context->state[i_tag + 1], NULL);
}

long int GetValueInt(const char* tag, int base, long int def, ParserContext* context)
{
    if (!context) return GetValueInt(tag, base, def, &global_parser_context);
    long int i_tag = FindIndexOfTag(tag, context);
    if (i_tag < 0)
    {
        fprintf(stderr, "Could not find tag %s using default value %ld\n", tag, def);
        return def;
    }
    return strtol(context->state[i_tag + 1], NULL, base);
}

unsigned long int GetValueUInt(const char* tag, int base, unsigned long int def, ParserContext* context)
{
    if (!context) return GetValueUInt(tag, base, def, &global_parser_context);
    long int i_tag = FindIndexOfTag(tag, context);
    if (i_tag < 0)
    {
        fprintf(stderr, "Could not find tag %s using default value %lu\n", tag, def);
        return def;
    }
    return strtoul(context->state[i_tag + 1], NULL, base);
}

unsigned long long int GetValueULLInt(const char* tag, int base, unsigned long long int def, ParserContext* context)
{
    if (!context) return GetValueULLInt(tag, base, def, &global_parser_context);
    long int i_tag = FindIndexOfTag(tag, context);
    if (i_tag < 0)
    {
        fprintf(stderr, "Could not find tag %s using default value %llu\n", tag, def);
        return def;
    }
    return strtoull(context->state[i_tag + 1], NULL, base);
}
#endif //_PARSER_IMPLEMENTATION