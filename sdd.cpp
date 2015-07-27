#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <time.h>
using namespace std;
//#include <Windows.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX 300
#define CRC_GENERATOR 0x04C12DB7

struct nodLista{
	char* text;
	nodLista* urm;
};
struct nodAVL{
	char* cale;
	int cheie, ech;
	nodAVL *st, *dr;
};

char* afiseazaEroare(char *s);
char* toLowerCase(char* str);
void log(char *x);
nodLista* sterge(nodLista* prim);
void afiseaza(nodLista* prim);
nodLista* adaugaLaInceput(nodLista *prim, char * s);
nodLista* creazaNod(char* x);
void drumMaxim(nodAVL *p, int &max, int lung);
void calculEch(nodAVL *p);
nodAVL* creazaNodAVL(int c, int e);
nodAVL* rot_dr_s(nodAVL *p);
nodAVL* rot_st_s(nodAVL *p);
nodAVL* rot_dr_d(nodAVL *p);
nodAVL* rot_st_d(nodAVL *p);
nodAVL* insereazaAVL(nodAVL* p, long int x,char* cale);
nodAVL* echilibreaza(nodAVL *p);
nodAVL* cautaAVL(nodAVL * p, int key);
//nodAVL* insereaza_din_lista_in_AVL(nodLista* prim,nodAVL* p);
nodAVL* initializeazaAVL(FILE *f, FILE *idx, nodAVL *p);
int hash(const char* s);
void afiseazaAVL(nodAVL *p, int nivel);
void adauga_in_bd(FILE *bd, FILE *idx, char *cale, nodAVL*p);
int crc(const int* data, int size, int generator);
bool extensie_vulnerabila(char* den);
void cautaAVLpath(nodAVL * p, char *s, bool &gasit);

int main(int argc, char** argv)
{
	cout << "Citesc din baza de date si incarc in arbore ... (52)\n";
	
	FILE *bd, *idx, *virus;
	bd = fopen("/home/dinu/Documents/test/files.txt", "r+");
	idx = fopen("/home/dinu/Documents/test/index.idx", "w+");
	virus = fopen("/home/dinu/Documents/test/viruses.dat", "r+b");
	
	nodAVL *p = NULL;
	if (bd != NULL){
		p = initializeazaAVL(bd, idx, p);
		cout << "Am terminat de citit din baza de date.(60). ArboreleAVL constituit: \n";
		afiseazaAVL(p,0);
	}
	else
		cout << "Fisierul baza de date nu a putut fi deschis.(63)\n";
	
	FILE *f;
    //if (argv[0]) 
	f = fopen("/home/dinu/Documents/test/parametru.txt", "r");
	char* bff;
	bff = new char[MAX];
	nodLista* prim = 0;
	if (f == NULL) perror ("Fisierul dat ca parametru nu a putut fi descchis.\n");
	else{
		cout << "Citesc din fiesierul dat ca parametru.(74)\n";
		while (!feof(f))
		{
			fscanf(f, "%s", bff);
			prim = adaugaLaInceput(prim, bff);
		}
		cout << "Lista:\n";
		afiseaza(prim);
		//insereaza_din_lista_in_AVL(prim, p);
		if (prim != NULL){
			while (prim != NULL){
				///////////////////
				//////////////////
				bool gasit = false;
				cautaAVLpath(p, prim->text, gasit);
				if (!gasit)
					adauga_in_bd(bd, idx, prim->text, p);
				prim = prim->urm;
			}
			cout << "Am updatat baza de date.(95)\n";
		}
		else
			cout << "Oups.Lista este goala.(98)\n";
	}
	if (f) fclose(f);
	
	cout << "Introdu comanda\n> ";
	scanf("%[^\n]s", bff);
	while( strcmp(toLowerCase(bff), "exit"))
		{
		log(bff);
		char* cuv = toLowerCase(strtok(bff, " "));
		if (!strcmp(cuv, "protect"))
		{
			char* temp;
			temp = new char[MAX];
			cuv = strtok(NULL, " ");
			if (cuv == NULL)
			{
				getcwd(temp, MAX);
			}
			else
			{
				strcpy(temp, cuv);
			}
			//protejeaza(temp);
		}
		else if (!strcmp(bff, "scan"))
		{
			cout << "cuv";
		}
		else if (!strcmp(bff, "update"))
		{
			cout << "cuv";
		}
		else if (!strcmp(bff, "report"))
		{
			cout << "cuv";
		}
		else
			afiseazaEroare("");
		cout << "\n> ";
		fflush(stdin);
		scanf("%[^\n]s", bff);                
    }
   
	if (bd) fclose(bd);
	if (idx) fclose(idx);
	if (virus) fclose(virus);
    return 0;
}
char* toLowerCase(char* str)
{	
	int differ = 'A'-'a';
	char ch;
	int ii = strlen(str);
	for (int i=0; i <ii;i++)
	{
		strncpy(&ch,str+i,1);
		if (ch>='A' && ch<='Z')
		{
			ch = ch-differ;
			memcpy(str+i,&ch,1);
		}
	}
	return str;
}

char* afiseazaEroare(char *s){
	char* temp = new char[60 + strlen(s) + 1];
	strcpy(temp, s);
	strcpy(temp, "Comanda introdusa nu este valida. Incearca din nou\n");
	return temp;
}

void log(char *x)
{
    FILE *f;
    f = fopen("logs.txt", "a");
    if (f == NULL)
        cout << "Fisierul de log nu a putut fi deschis";
    else{
		time_t raw;
		time(&raw);
        fputs(x, f);
		fprintf(f,  "%s", ctime(&raw));
	}
    fclose(f);
}


nodLista* creazaNod(char* x){
	nodLista* temp = new nodLista[1];
	temp->urm = NULL;
	temp->text = new char[strlen(x)+1];
	strcpy(temp->text, x);
	return temp;
}

nodLista* adaugaLaInceput(nodLista *prim, char * s)
{
	nodLista* x = creazaNod(s);
	if (prim == NULL){
		prim = x;
	}
	else
	{
		x->urm = prim;
		prim = x;
	}
	return prim;
	
}

void afiseaza(nodLista* prim){
	if (prim != NULL)
		while (prim != NULL){
			cout << prim->text << endl;
			prim = prim->urm;
		}
	else
		cout << "Oups.Lista este goala.\n";
}

nodLista* sterge(nodLista* prim){
	if (prim != NULL)
	{
		nodLista* temp = prim;
		prim = prim->urm;
		delete temp;
	}
	return prim;
}

void drumMaxim(nodAVL *p, int &max, int lung){
	if (p != NULL){
		drumMaxim(p->dr, max, lung+1);
		if ((p->st == NULL) && (p->dr == NULL) && (max<lung))
			max = lung;
		drumMaxim(p->st, max, lung+1);
	}
}

void calculEch(nodAVL *p){
	int iSt, iDr;
	iSt = 1;
	iDr = 1;
	if (p->st != NULL)
		drumMaxim(p->st, iSt, 1);
	else
		iSt = 0;
	if(p->dr != NULL)
		drumMaxim(p->dr, iDr, 1);
	else
		iDr = 0;
	p->ech = iDr - iSt;
}

nodAVL* creazaNodAVL(long int c, char* s){
	nodAVL* temp = new nodAVL[1];
	temp->cale = new char[strlen(s)+1];
	strcpy(temp->cale, s);
	temp->ech = 0;
	temp->cheie = c;
	temp->dr = NULL;
	temp->st = NULL;
	return temp;
}

nodAVL* rot_dr_s(nodAVL *p){
	nodAVL *q;
	q = p->st;
	p->st = q->dr;
	q->dr = p;
	calculEch(p);
	calculEch(q);
	p = q;
	return p;
}

nodAVL* rot_st_s(nodAVL *p){
	nodAVL *q;
	q = p->dr;
	p->dr = q->st;
	q->st = p;
	calculEch(p);
	calculEch(q);
	p = q;
	return p;
}

nodAVL* rot_dr_d(nodAVL *p){
	p->st = rot_st_s(p->st);
	p = rot_dr_s(p);
	return p;
}


nodAVL* rot_st_d(nodAVL *p){
	p->dr = rot_dr_s(p->dr);
	p = rot_st_s(p);
	return p;
}

nodAVL* insereazaAVL(nodAVL* p, long int x, char* cale){
	if (p == NULL){
		p = creazaNodAVL(x, cale);
	} else
	{
		if (x < p->cheie)
			p->st = insereazaAVL(p->st, x, cale);
		else
			if (x > p->cheie)
				p->dr = insereazaAVL(p->dr, x, cale);
			else
				cout << "Nodul cu cheia dorita exista deja";
	}
	//weird return p;
	return echilibreaza(p);
}

nodAVL* echilibreaza(nodAVL *p){
	nodAVL *temp;
	calculEch(p);
	if (p->ech == -2){
		temp = p->st;
		if (temp->ech == 1)
			p = rot_dr_d(p);
		else
			p = rot_dr_s(p);
	}
	else if (p->ech == 2){
		temp = p->dr;
		if (temp->ech == -1)
			p = rot_st_d(p);
		else
			p = rot_st_s(p);
	}
	return p;
}

nodAVL* cautaAVL(nodAVL * p, int key){
	while (p != NULL ){
		if (p->cheie == key)
			return p;
		else
			if (p->cheie > key)
				p = p->st;
			else
				p = p->dr;
	}
	return NULL;
}

void cautaAVLpath(nodAVL * p, char *s, bool &gasit){
	if (p!=NULL && !gasit){
		if (!strcmp(p->cale, s))
			gasit = true;
		else
		{
			cautaAVLpath(p->st, s, gasit);
			cautaAVLpath(p->dr, s, gasit);
		}
	}
}

//nodAVL* insereaza_din_lista_in_AVL(nodLista* prim,nodAVL* p){
//	nodAVL* temp = NULL;
//	int h;
//	if (prim != NULL){
//		while (prim != NULL){
//			h = hash(prim->text);
//			nodAVL* temp = cautaAVL(p, h);
//			if (temp == null)
//				p = insereazaAVL(p, h, prim->text);
//			prim = prim->urm;
//		}
//	}
//	else
//		cout << "Oups.Lista este goala.\n";
//	return p;
//}

nodAVL* initializeazaAVL(FILE *f, FILE *idx, nodAVL *p){
	char* den = new char[MAX/2];
	char* path = new char[MAX]; 
	char ext[15];
	int dim, crc;
	long int datc, datm;
	while (!feof(f))
		{
			long int poz = ftell(f);
			//denumire, path, ext, dimensiune, data creare, data mod, crc
			fscanf(f, "%s %s %s %i %li %li %i", den, path, ext, &dim, &datc, &datm, &crc);
			fprintf(idx, "%s %li\n", path, poz);
			//cuv = toLowerCase(strtok(bff, " "));
//	/	/	cuv = strtok(NULL, " ");
			p = insereazaAVL(p, poz, path);
			//p = echilibreaza(p);
		}
	return p;
}

int hash(const char* s){
	int h, len;
	h = 0;
	len = strlen(s);
	for (int i=0; i<len; i++){
		char c = s[i];
		h = (31*c) + (int)c;
	}
	return h;
}

void afiseazaAVL(nodAVL *p,int nivel){
	if (p!=NULL){
		cout << "Nivel:" << nivel << "\nHashkey:" << p->cheie << "\nCale:" << p->cale << endl << endl;
		if (p->dr != NULL)
			afiseazaAVL(p->dr, nivel+1);
		if (p->st != NULL)
			afiseazaAVL(p->st, nivel+1);
	}
}

void adauga_in_bd_si_idx(FILE *bd, FILE *idx, nodAVL *p, char* den, char*path, char* ext, int dim, long int datc, long int datm, int crc){
	////denumire, path, ext, dimensiune, data creare, data mod, crc
	//denumre, poz in fisier	
	if(bd && idx){
		long int x = ftell(bd);
		fprintf(bd, "%s %s %s %i %li %li %i", den, path, ext, dim, datc, datm, crc);
		fprintf(idx, "%s %li", den, x);
		p = insereazaAVL(p, x, path);
	}
	else
		cout << "Fisierul bazei de date sau a indexului e invalid (410);\n";
}

//adauga in bd
void adauga_in_bd(FILE *bd, FILE *idx, char *cale, nodAVL*p){
	DIR* dirr;
	struct dirent* curent;
	char path[MAX];
	char* ext = 0;
	
	if ((dirr=opendir(cale)) == NULL)
		while ((curent=readdir(dirr)) != NULL) {
			if (strncmp(curent->d_name, "..", 2) != 0 && strncmp(curent->d_name, ".", 1) != 0) {
				strncpy(path, cale, MAX);
				strncat(path, "/", 2);
				strncat(path, curent->d_name, MAX/2);
				if (curent->d_type == DT_DIR)
					adauga_in_bd(bd, idx, path, p);
				else {
					bool gasit = false;
					cautaAVLpath(p, path, gasit);
					if (!gasit){
						ext = strrchr(curent->d_name,'.');
						if (ext && extensie_vulnerabila(ext)){
							struct stat st;
							if (ext && stat(path, &st)!=0)
								adauga_in_bd_si_idx(bd, idx, p, curent->d_name, path, ext, st.st_size, st.st_ctime, st.st_mtime, 11);
							//printf("\n denumire:%s path:%s ext:%s creation date:%li last modification date:%li\n",
	//								curent->d_name,
	//								path,
	//								ext==NULL?"directory":ext,
	//								(stat(longest_name, &st))!=0?0:st.st_ctime,
	//								st.st_mtime);
						}
					}
				}
			}
		}
	closedir(dirr);
	return;
}

int crc(const int* data, int size, int generator){
	static const int BITS = 8 * sizeof(int);
	int r=0, j=0;
	
	for (int i=0; i<size; i++){
		r ^= data[i] << (BITS-8);
		for (j=0; j<8; j++)
			r = (r & (1<<(BITS-1)) ? (r<<1) ^ generator : r<<1);
	}
	
	return r;
}

bool extensie_vulnerabila(char* den){
	FILE *ext;
	ext = fopen("ext.txt", "r+");
	char* e;
    e = strrchr(den,'.');
	if (e){
		char* temp = new char[strlen(e)+3];
		while (!feof(ext)){
			fscanf(ext, "%s", temp);
			if (!strcmp(temp, e))
				return true;
		}
	}
	fclose(ext);
	return false;
}

void protejaza(char* param){
	
}
