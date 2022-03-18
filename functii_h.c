/* ORZATA Gabriel-Nicusor - 313CB */
#include "functii_h.h"

TH* IniTH(size_t M, TFHash fh)
{
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h) {
        printf("eroare alocare hash\n");
        return NULL;
    } 

    h->v = (TLDI *) calloc(M, sizeof(TLDI));
    if(!h->v) {
        printf("eroare alocare vector de pointeri TLG in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;
    return h;
}

void free_dns(void* x)
{
    // Aceasta este functia de eliberare specifica
    // pentru tipul de date DNS
    DNS *d = (DNS*)x;
    free(d->nume);
    free(d->ip);
    free(d);
}

void DistrTH(TH** ah, TF fe)
{
    TLDI *p, el, aux;

    for (p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++) {
        // Verific daca lista este deja nula ca sa evit un double free()
        if((*p) != NULL){
            el = *p;
        } else{
            continue;
        }
        aux = el;
        // Verific mai intai daca lista are doar un element
        if(el->urm == el){
            fe(aux->info);
            free(aux);
            continue;
        } else {
            el = el->urm;
        }
        // Daca nu are doar un element trec la urmatorul inainte
        // de a elibera memoria din el
        fe(aux->info);
        free(aux);
        for(; el != *p; ) {
            // Cum intr-un bucket nu exista doua elemente la fel
            // am ales acesta conditie de oprire 
            aux = el;
            el = el->urm;
            fe(aux->info);
            free(aux);
        
        }
    }
    free((*ah)->v);
    free(*ah);
    *ah = NULL;
    // Dupa ce am eliberat toate bucket-urile eliberez si tabela H
}



int generare_hash(void* ae, int M)
{
    DNS *d = (DNS*)ae;
    int i = 0, sum = 0;
    // Parcurg key caracter cu caracter si adun valorile ascii in sum
    // pana cand ajung la termiantorul de sir
    while((d->nume)[i] != '\0'){
        sum += (int)((d->nume)[i]);
        i++;
    }
    return sum % M;
}

char* convert_ip(void* x)
{
    // Am creeat o functie care imi converteste orice tip de date in
    // tipul de date specific (in cazul asta DNS)
    DNS* dns = (DNS*)x;
    return dns->ip;
}

void print(TH* ah, Conv convertEl, FILE *out)
{
    TLDI p, el;
    int i;
    // Parcurg fiecare bucket si daca nu e gol printez ce e in el
    for(i = 0; i < ah->M; i++) {
        p = ah->v[i];
        if(p != NULL) {
            fprintf(out, "%d: ",i);
            fprintf(out, "%s ", convert_ip(p->info));
            for(el = p->urm; el != p; el = el->urm){
                fprintf(out, "%s ", convert_ip(el->info));
            }
            fprintf(out, "\n");
        }
    }
}

void print_bucket(TH* a, Conv convertEl, int cod, FILE* out)
{
    TLDI p, el;
    // Asa evit introducerea unui bucket inexistent
    if(cod >= a->M){
        return;
    }
    // Parcurg bucket-ul daca nu e gol si printez ce e in el
    if(a->v[cod] == NULL){
        fprintf(out, "VIDA\n");
        return;
    }
    // Pentru primul element printez in afara for-ului,
    // deoarece conditia de oprire din el sare de primul element
    p = a->v[cod];
    fprintf(out, "%s ", convert_ip(p->info));
    for(el = p->urm; el != p; el = el->urm){
        fprintf(out, "%s ", convert_ip(el->info));
    }
    fprintf(out, "\n");
}

int compara_dns(void* x, void* y)
{
    // Functie pentru orodnarea alfabetica a key-urilor cand ambii
    // parametrii sunt de tip DNS
    DNS *dx = (DNS*)x;
    DNS *dy = (DNS*)y;
    return strcmp(dx->nume, dy->nume);
}

int compara_key(void* x, void* y)
{
    // Functie pentru orodnarea alfabetica a key-urilor cand doar
    // primul parametru e de tip DNS
    DNS *dx = (DNS*)x;
    char *dy = (char*)y;
    return strcmp(dx->nume, dy);
}


int put(TH*a, void* ae, TFCmp f)
{
    int cod = a->fh(ae, a->M);
    // Calculez codul bucket-ului folosind functia specifica tipului de date 
    TLDI el, p, aux;
    if(a->v[cod] == NULL){
        // Daca nu mai exista alt element cu acelasi hash il introduc
        // si asociez pointer-ului adresa acestuia
        aux = malloc(sizeof(TCelula2));
        aux->info = ae;
        aux->pre = aux->urm = aux;
        a->v[cod] = aux;   
        return 1;    
    }
    p = a->v[cod];
    // Verific daca elementul exista deja
    if (f(p->info, ae) == 0){
        return 0;
    }
    for(el = p->urm; el != p; el = el->urm) {
        if (f(el->info, ae) == 0){
            return 0;
        }
    }
    // Daca aux este mai mic alfabetic fata de primul element
    // il introduc inaintea sa si modific pointerul catre bucket a->v[cod]
    if (f(p->info, ae) > 0){
        TLDI aux = (TLDI)malloc(sizeof(TCelula2));
        if(!aux) {
            return -1;
        }
        aux->info = ae;
        aux->pre = p->pre;
        aux->urm = p;
        p->pre = aux;
        aux->pre->urm = aux;
        a->v[cod] = aux;
        return 1;
    } else {
        for(el = p->urm; el != p; el = el->urm) {
            if (f(el->info, ae) > 0){
                // Daca aux este mai mic alfabetic decat urmatoarele elem
                // il introduc inaintea lor
                InsDupa(el->pre, ae);
                return 1;
            }
        }
    }
    // Daca programul ajunge aici inseamna ca aux este mai mare alfabetic decat
    // toate elementele din bucket si folosesc el->pre deoarece el ajunge sa fie
    // p cand iese din for
    InsDupa(el->pre, ae);
    return 1;

}

void* get(TH *a, void* key, TFCmp f, FILE* out)
{
    TLDI p, el;
    int i = 0, sum = 0, cod;
    char *nume = (char*)key;
    while((nume)[i] != '\0'){
        sum += (int)((nume)[i]);
        i++;
    }
    // Calculez hash-ul
    cod = sum % a->M;
    if(a->v[cod] != NULL){
        p = a->v[cod];
    } else {
        return NULL;
    }
    // Daca exista un bucket un acest hash verfic daca in el se afla key
    if (f(p->info, key) == 0){
        return (void*)p->info;
    }
    for(el = p->urm; el != p; el = el->urm) {
        if (f(el->info, key) == 0){
            return (void*)el->info;
        }
    }
    // Cu functia de comparatie verfic daca key este in bucket, daca nu
    // este reurnez NULL
    return NULL;
}

int find(TH *a, void* key, TFCmp f, FILE* out)
{
    TLDI p, el;
    int i = 0, sum = 0, cod;
    char *nume = (char*)key;
    while((nume)[i] != '\0'){
        sum += (int)((nume)[i]);
        i++;
    }
    cod = sum % a->M;
    // La fel ca la get caut in bucket, doar ca acum returnez 0/1 si scriu
    // in fisier False/True
    if(a->v[cod] != NULL){
        p = a->v[cod];
    } else {
        fprintf(out, "False\n");
        return 0;
    }
    if (f(p->info, key) == 0){
        fprintf(out, "True\n");
        return 1;
    }
    for(el = p->urm; el != p; el = el->urm) {
        if (f(el->info, key) == 0){
            fprintf(out, "True\n");
            return 1;
        }
    }
    fprintf(out, "False\n");
    return 0;
}

int Remove(TH *a, void* key, TFCmp f)
{
    TLDI p, el;
    int i = 0, sum = 0, cod;
    char *nume = (char*)key;
    while((nume)[i] != '\0'){
        sum += (int)((nume)[i]);
        i++;
    }
    cod = sum % a->M;
    if(a->v[cod] != NULL){
        p = a->v[cod];
    } else {
        return 0;
    }
    // Cautarea elemtului este facuta ca la get si find
    if (f(p->info, key) == 0){
        if(p->urm == p){
            // Daca bucket-ul are un singur element eliberez celula si
            // fac a->v[cod] NULL ca sa nu pointeze catre o zona de
            // memorie care a fost eliberata
            p->urm->pre = p->pre;
            p->pre->urm = p->urm;
            a->v[cod] = p->urm;
            free_dns(p->info);
            free(p);
            p = NULL;
            a->v[cod] = NULL;
        } else {
            // Daca functia are mai multe elemente fac acelasi lucru
            // numai ca setez a->v[cod] sa pointeze catre uramtorul 
            // element
            p->urm->pre = p->pre;
            p->pre->urm = p->urm;
            a->v[cod] = p->urm;
            free_dns(p->info);
            free(p);
            p = NULL;
        }
        return 1;
    }
    for(el = p->urm; el != p; el = el->urm) {
        if (f(el->info, key) == 0){
            // Daca elemetul nu este primul refac legaturile
            // si eliberez memoria
            el->urm->pre = el->pre;
            el->pre->urm = el->urm;
            free_dns(el->info);
            free(el);
            el = NULL;
            return 1;
        }
    }
    return 1;
}