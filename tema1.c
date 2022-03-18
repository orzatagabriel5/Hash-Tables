/* ORZATA Gabriel-Nicusor - 313CB */
#include "functii_h.h"

int main (int argc, char* argv[])
{
	int M, i;
    M = atoi(argv[1]); // primul argument este M pe care il obtin cu atoi
    TH *ah = NULL;
    ah = (TH*)IniTH(M, generare_hash);
    if(!ah){
		return 0;
	}

    char str[50], *p, v[3][30];
	FILE *in = fopen(argv[2], "r");
	FILE *out = fopen(argv[3], "w");
    // deschid fiserele de input si de output
    while(fgets(str, 50, in)){
        i = 0;
        if(feof(in) || strlen(str) < 3){
            break;
        } // Cand este un enter in plus la finalul inputului
        if(str[strlen(str) - 1] == '\n'){
			str[strlen(str) - 1] = 0;
		} // elimin enter-ul de la finalul liniei
        p = strtok(str," ");
		while(p != NULL){
			strcpy(v[i], p);
			p = strtok(NULL, " ");
			i++;
		} // separ fiecare cuvant din linia citita
        if(strcmp(v[0], "put") == 0){
            DNS *dns = (DNS*)malloc(sizeof(DNS));
            dns->nume = malloc(30 * sizeof(char));
            dns->ip = malloc(30 * sizeof(char));
            strcpy(dns->nume, v[1]);
            strcpy(dns->ip, v[2]);
            // creez un dns pentru a da ca parametru functiei put structura
            // specifica DNS in care pun datele citite din fisierul de input
            if(put(ah, (void*)dns, compara_dns) == 0){
                free(dns->nume);
				free(dns->ip);
				free(dns);
                // daca dns-ul era deja in tabela H eliberez
                // memoria alocata mai sus
            }
        }
        if(strcmp(v[0], "get") == 0){
            void *x;
            if((x = get(ah, (void*)v[1], compara_key, out)) != NULL){
                fprintf(out, "%s\n", convert_ip(x));
                // Daca se gaseste key printez in fisier ip-ul acesteia
            } else {
                fprintf(out, "NULL\n");
            }
        }
        if(strcmp(v[0], "find") == 0){
            find(ah, (void*)v[1], compara_key, out);
        }

        if(strcmp(v[0], "print_bucket") == 0){
            print_bucket(ah, convert_ip, atoi(v[1]), out);
        }

        if(strcmp(v[0], "print") == 0){
            print(ah, convert_ip, out);
        }

        if(strcmp(v[0], "remove") == 0){
            Remove(ah, (void*)v[1], compara_key);
            
        }

    }
    
    
    fclose(in);
    fclose(out);
    DistrTH(&ah , free_dns);
    return 1;
}