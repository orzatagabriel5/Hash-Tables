/* ORZATA Gabriel-Nicusor - 313CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "functii_tldi.h"
 
#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef int (*TFElem)(void*);     /* functie prelucrare element */
typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*);     /* functie afisare/eliberare un element */
typedef int (*TFHash)(void*, int);
typedef char* (*Conv)(void*);

typedef struct
{
    size_t M;
    TFHash fh;
    TLDI *v;
} TH;

/* functii tabela hash */
void free_dns(void* x);
TH* IniTH(size_t M, TFHash fh);
void DistrTH(TH**aa, TF fe);
int put(TH*a, void*ae, TFCmp f);
int compara_key(void* x, void* y);
int compara_dns(void* x, void* y);
int generare_hash(void* key, int M);
void* get(TH *a, void* key, TFCmp f, FILE* out);
int find(TH *a, void* key, TFCmp f, FILE* out);
void print_bucket(TH* a, Conv ConvertEl, int cod, FILE* out);
void print(TH* ah, Conv ConvertEl, FILE *out);
int Remove(TH *a, void* key, TFCmp f);
char* convert_ip(void* x);

#endif