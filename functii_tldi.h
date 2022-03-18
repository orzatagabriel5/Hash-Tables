#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


/* ORZATA Gabriel-Nicusor - 313CB */
#ifndef _LISTA_DUBLU_INLANTUITA_
#define _LISTA_DUBLU_INLANTUITA_

typedef struct celula2
{
  void *info;              /* adresa element extern */
  struct celula2 *pre, *urm;  /* legaturi spre celulele vecine */
} TCelula2, *TLDI;

typedef struct
{
  char *nume, *ip;
}DNS;


TLDI InitLDI();
int InsDupa(TLDI p, void* x);


#endif
