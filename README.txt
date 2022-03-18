Tema 1
Santinela:
	am inlocuit santinela profitand de faptul ca in bucket-urile corespunzatoare
	hash-urilor nu pot si doua elemente la fel, asa ca am pus ca iterarea prin
	lista sa se opreasca cand se ajunge la primul element ( el != p).
Citirea argumentelor am facut-o folosind argc si argv[].

Citirea din fisierul de input am facut-o folosind un vector de 3 string-uri
in care am pus comanda si key sau ip daca a fost nevoie.

PUt:
	Calculez hash-ul bucket-ului folosind functia specifica tipului de date.
	Daca nu mai exista alt element cu acelasi hash il introducsi asociez
	pointer-ului adresa acestuia.
	Apoi verific daca elemntul exista deja folosind functia de comparare.
	Daca elementul care trebuie introdus este mai mic alfabetic fata de 
	primul element il introduc inaintea sa si modific pointerul catre
	bucket a->v[cod].
	La final daca elementul este mai mare alfabetic decat toate elementele
	din bucket il pun la "sfarsitul" listei circulare.
GET:
	Verific daca elementul este in lista la fel ca la functia put si in caz
	afirmativ returnez pointerul catre zona de memorie a acestuia.
FIND:
	Acelasi lucru ca la functia get numai ca intorc 0 sau 1 si scriu
	in fisierul de output True sau False.
Remove:
	Cautarea elementului o fac ca la get si find.
	Daca elementul care trebuie scos este primul dupa ce eliberez memoria
	updatez pointerul catre inceputul listei (a->v[cod]) cu urmatorul element.
	Daca lista are un singur element acesta va devenii NULL.
	In caz contrar, doar eliberez memoria dupa ce refac legaturile.
DistrTH:
	Am facut o functie de eliberare specifica pentru structura de date folosita
	(free_dns). Trec prin toate bucket-urile si daca acestea nu sunt goale le
	eliberez. Verific daca bucket-ul are doar un element, caz in care eliberez
	memoria acestuia si trec la urmatorul bucket; daca lista are mai mult de
	un element tred la urmatorul element inainte de a elibera memoria din el,
	pentru a evita accesarea unei zone de memorie goala. Apoi eliberez memoria
	din restul elementelor si la final eliberez si tabela hash.