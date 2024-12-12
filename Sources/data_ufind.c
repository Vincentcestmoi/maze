#include "data_ufind.h"
#include <stdint.h>
#include <limits.h>


ufind* create_ufind(int i)
{
    ufind *uf = malloc(sizeof(ufind));
    if (uf == NULL)
    {
        perror("Failed to allocate memory for ufind");
        exit(EXIT_FAILURE);
    }
    uf->size_tab = i;
    return uf;
}


void delete_ufind(ufind* uf)
{
    free(uf -> parent);
    free(uf -> rank);
    free(uf -> sizec);
    free(uf);
}

int sizeset_ufind(ufind*)
{
    return 0;
}

int sizepar_ufind(ufind*)
{
    return 0;
}

void makeset_ufind(ufind*)
{
    return;
}

int find_ufind(int, ufind*)
{
    return 0;
}


int sizeclass_ufind(int, ufind*)
{
    return 0;
}

void union_ufind(int, int, ufind*)
{
    return;
}
