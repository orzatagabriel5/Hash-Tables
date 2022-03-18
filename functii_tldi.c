/* ORZATA Gabriel-Nicusor - 313CB */
#include "functii_h.h"


TLDI InitLDI(void* x)
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
    if (!aux) {
        return NULL;
    }
	aux->pre = aux->urm = aux;
    aux->info = x;
	return aux;
}

int InsDupa(TLDI a, void* x)
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) {
        return 0;
    }
	aux->info = x;
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;
    return 1;
}