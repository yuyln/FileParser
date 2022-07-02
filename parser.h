#ifndef __PARSER
#define __PARSER
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#ifdef _MSC_VER
#define strtok_r strtok_s
#endif

typedef enum
{
    P_DOUBLE,
    P_FLOAT,
    P_INT,
    P_UINT
} P_TYPES;

static const char *separetors = " =\n\t:";
char **Parse(char *str, int *qnt);
char *ReadFileP(FILE *stream);
void *GetValue(const char *value, char **strs, int qnt, P_TYPES type);
int GetValueInt(const char *value, char **strs, int qnt);
unsigned int GetValueUInt(const char *value, char **strs, int qnt);
double GetValueDouble(const char *value, char **strs, int qnt);
float GetValueFloat(const char *value, char **strs, int qnt);
int GetIndexOfTag(const char *tag, char **strs, int qnt);

#ifdef PARSER_IMPLEMENTATION

static int FindSize(char *str)
{
    char *auxptr;
    char *sep = strtok_r(str, separetors, &auxptr);
    int i = 0;
    while (sep != NULL)
    {
        sep = strtok_r(NULL, separetors, &auxptr);
        i++;
    }
    return i;
}

char **Parse(char *str, int *qnt)
{
    char *backup = strdup(str);
    char *backup2 = strdup(str);

    int size = FindSize(backup);
    *qnt = size;

    char **out = (char **)malloc(sizeof(char *) * size);
    char *auxptr;

    out[0] = strtok_r(backup2, separetors, &auxptr);
    for (int i = 1; i < size; i++)
    {
        out[i] = strtok_r(NULL, separetors, &auxptr);
    }
    return out;
}

char *ReadFileP(FILE *stream)
{
    if (stream == NULL)
    {
        fprintf(stderr, "FILE IS NULL: %s", strerror(errno));
    }
    fseek(stream, 0, SEEK_SET);
    fseek(stream, 0, SEEK_END);
    size_t filesize = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    char *out = (char *)malloc(filesize + 1);
    size_t readsize = fread(out, 1, filesize, stream);
    if (readsize != filesize)
    {
        fprintf(stderr, "FILE SIZE(%zu) AND READ FILE(%zu) NOT EQUAL\n", filesize, readsize);
    }
    out[filesize] = '\0';
    return out;
}

void *GetValue(const char *value, char **strs, int qnt, P_TYPES type)
{
    uintptr_t *ret = (uintptr_t*)malloc(sizeof(uintptr_t));
    *ret = 0;
    int i = 0;
    int r = 0;
    for (i = 0; i < qnt; i++)
    {
        r = strcmp(value, strs[i]);
        if (r == 0)
        {
            break;
        }
    }

    if (r != 0)
    {
        fprintf(stderr, "%s NOT FOUND ON STRINGS\n", value);
        exit(-1);
    }

    switch (type)
    {
    case P_DOUBLE:
    {
        *((double*)ret) = strtod(strs[i + 1], NULL);
        return ret;
        break;
    }
    case P_FLOAT:
    {
        *((float*)ret) = strtof(strs[i + 1], NULL);
        return ret;
        break;
    }
    case P_INT:
    {
        *((int*)ret) = strtol(strs[i + 1], NULL, 10);
        return ret;
        break;
    }
    case P_UINT:
    {
        *((unsigned int*)ret) = strtoul(strs[i + 1], NULL, 10);
        return ret;
        break;
    }
    default:
    {
        assert(0 && "how?");
        break;
    }
    }
    return NULL;
}

int GetValueInt(const char *value, char **strs, int qnt)
{
    return *(int*)GetValue(value, strs, qnt, P_INT);
}

unsigned int GetValueUInt(const char *value, char **strs, int qnt)
{
    return *(unsigned int*)GetValue(value, strs, qnt, P_UINT);
}

double GetValueDouble(const char *value, char **strs, int qnt)
{
    return *(double*)GetValue(value, strs, qnt, P_DOUBLE);
}

float GetValueFloat(const char *value, char **strs, int qnt)
{
    return *(float*)GetValue(value, strs, qnt, P_FLOAT);
}

int GetIndexOfTag(const char *tag, char **strs, int qnt)
{
    int i = 0;
    int r = 0;
    for (i = 0; i < qnt; i++)
    {
        r = strcmp(tag, strs[i]);
        if (r == 0)
        {
            break;
        }
    }

    if (r != 0)
    {
        fprintf(stderr, "%s NOT FOUND ON STRINGS\n", tag);
        exit(-1);
    }
    return i;
}

#endif

#endif