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

#define MAX_PATH 300
#define VTAB_MAX 100
#define CRC_GENERATOR 0x04a179c

struct nodLista{
	char* text;
	nodLista* urm;
};
struct nodAVL{
	char* cale;
	int cheie, ech;
	nodAVL *st, *dr;
};
struct nodTD{
	nodTD *urm;
	char nume[50];
	char semn[31];
};
struct nodI{
	char *path;
	char *ext;
	char *categ;
	nodI *urm;
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
void adauga_in_bd(FILE *bd, FILE *idx, char *cale, nodAVL *&p);
int crc(const int* data, int size, int generator);
bool extensie_vulnerabila(char* den);
void cautaAVLpath(nodAVL * p, char *s, bool &gasit);
int CRC(char *s);
char* getBiti(int x);
bool cautaSemnaturaDupaCat(nodTD **td, char *semnn, char *cat);
char* cautaSemnatura(nodTD **td, char *semnn);
void cautaCat(char* cat, nodTD **td);
bool insereazaInCat(char *cat, char *s, nodTD **td,char *semnn);
bool insereazaCat(char *cat, nodTD **td);
void injecteazaTD(nodTD **td, FILE *vir);
void afiseazaTD(nodTD **td);
void scrieTD(nodTD **td, FILE *v);
int afiseazaFI(nodI *p, char *pr);
void grupare_fis_infectate_dupa_ext(FILE *ext, FILE *out, nodI *prim, bool extra);
void grupare_fis_infectate_dupa_vir(FILE *out, nodI *prim, bool extra);
void grupare_fis_infectate_dupa_ext(FILE *ext, FILE *out, nodI *prim, bool extra, char *p);
void grupare_fis_infectate_dupa_vir(FILE *out, nodI *prim, bool extra, char *p);
nodI* insertNodI(nodI *prim, char* path, char *ext, char *categ);
nodI* creazaNodI(char *path, char *ext, char *categ);
void scan(FILE *bd, FILE *idx, char *cale, nodAVL *&p, nodTD **td, nodI *&iprim);

int main(int argc, char** argv)
{
	cout << "Citesc din baza de date si incarc in arbore ... (52)\n";
	
	FILE *bd, *idx, *virus;
	bd = fopen("/home/dinu/Documents/test/files.txt", "r+");
	idx = fopen("/home/dinu/Documents/test/index.idx", "r+");
	
	nodTD **td;
	td = new nodTD*[VTAB_MAX];
	for (int i=0; i<VTAB_MAX; i++)
		td[i] = NULL;
	
	printf("Initializez dictionarul de semnaturi...\n");
	virus = fopen("/home/dinu/Documents/test/viruses.dat", "r+b");
	if (virus){
		injecteazaTD(td, virus);
		//afiseazaTD(td);
		/*
		if (insereazaCat("Parazitii", td)){
			insereazaInCat("Parazitii", "sss", td, "2324");
			insereazaInCat("Parazitii", "sssss", td, "23224");
			insereazaInCat("Parazitii", "sssssss", td, "12324");
		}*/
		afiseazaTD(td);
		printf("Initializare completa!\n");
		fclose(virus);
		
		FILE *viruss;
		viruss = fopen("/home/dinu/Documents/test/viruses.dat", "wb");
		if (viruss){
			scrieTD(td, virus);
			fclose(viruss);
		}
		//fclose(virus);
		
	}
	
	nodAVL *p = 0;
	if (bd && idx){
		p = initializeazaAVL(bd, idx, p);
		afiseazaAVL(p,0);
		cout << "Am terminat de citit din baza de date.(60)\n";
	}
	else
		cout << "Fisierul baza de date nu a putut fi deschis.(63)\n";
	
	FILE *f;
    //if (argv[0]) 
	f = fopen("/home/dinu/Documents/test/parametru.txt", "r");
	char* bff;
	bff = new char[MAX_PATH];
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
			cout << "Am updatat baza de date.(95), Afisez noul arbore:\n";
			afiseazaAVL(p,0);
		}
		else
			cout << "Oups.Lista este goala.(98)\n";
		fclose(f);
	}
		
	nodI *iprim = NULL;
	char *bff1 = new char[MAX_PATH];
	cout << "Introdu comanda\n>";
	//scanf("%[^\n]s", bff);
	cin.getline(bff, MAX_PATH);
	while( strcmp(bff, "exit"))
		{
		strcpy(bff1, bff);
		char* cuv = toLowerCase(strtok(bff, " "));
		if (!strcmp(toLowerCase(cuv), "protect"))
		{
			char* temp;
			temp = new char[MAX_PATH];
			cuv = strtok(NULL, " ");
			if (cuv == NULL)
			{
				getcwd(temp, MAX_PATH);
			}
			else
			{
				strcpy(temp, cuv);
			}
			char* extensie = strrchr(temp, '.');
			FILE *ff = fopen(temp, "r");
			if (ff==NULL || strcmp(extensie, ".txt"))
				adauga_in_bd(bd, idx, temp, p);
			else{
				while (!feof(ff)){
					fscanf(ff, "%s", bff);
					if (bff) adauga_in_bd(bd, idx, bff, p);
				}
				fclose(ff);
			}
			//log(bff1);
		}
		else if (!strcmp(toLowerCase(cuv), "scan"))
		{
			char* temp;
			temp = new char[MAX_PATH];
			cuv = strtok(NULL, " ");
			if (cuv == NULL)
			{
				getcwd(temp, MAX_PATH);
			}
			else
			{
				strcpy(temp, cuv);
			}
			char* extensie = strrchr(temp, '.');
			FILE *fff = fopen(temp, "r");
			if (fff==NULL || strcmp(extensie, ".txt"))
				scan(bd, idx, temp, p, td, iprim);
			else{
				while (!feof(fff)){
					fscanf(fff, "%s", bff);
					if (bff) 
						scan(bd, idx, bff, p, td, iprim);
				}
				fclose(fff);
			}
			//log(bff1);
		}
		else if (!strcmp(toLowerCase(cuv), "update"))
		{
			char *nume = new char[50];
			int x, count = sscanf(bff1, "update %s -c %s -v %s -b %x", bff, nume, cuv, &x);
			if (count == 4){
				char *bitii; bitii = new char[31];
				for (int i=0;i<32; bitii[i++]='0');
				bitii = getBiti(x);
				printf ("semnatura:%s\n", bitii);
				if (insereazaInCat(nume, cuv, td, bitii)){
					printf("Inserarea a fost un succes total!\n");
					FILE *viruss;
					//viruss = fopen(bff, "wb");
					if (viruss){
						scrieTD(td, virus);
						fclose(viruss);
					}
				}
				else
					printf("Inserarea a esuat lamentabil.\n");
			}
			//log(bff1);
		}
		else if (!strcmp(toLowerCase(cuv), "report"))
		{
			FILE *ext;
			ext = fopen("/home/dinu/Documents/test/ext.txt", "r");
			char *home=0;
			getcwd(home, MAX_PATH);
			// report-n [-d] [-t] [-c] [path | drive | file]  <fileout>
			char d;
			int count = sscanf(bff1, "report -n -d -%c %s %s", &d, bff, cuv);
			if (count == 3 && ext){
				FILE *out = fopen(cuv, "w");
				if (d == 't' && out)
					grupare_fis_infectate_dupa_ext(ext, out, iprim, true, cuv);
				else if (d == 'c' && out)
					grupare_fis_infectate_dupa_vir(out, iprim, true, cuv);
				else
					printf("Comanda incorecta sau fisierul de output nu a putut fi deschis. (258)\n");
				}else{
					count = sscanf(bff1, "report -n -d -%c %s", &d, bff);
					if (count == 2){
						FILE *out = fopen(cuv, "w");
						if (d == 't' && out)
							grupare_fis_infectate_dupa_ext(ext, out, iprim, true, home);
						else if (d == 'c' && out)
							grupare_fis_infectate_dupa_vir(out, iprim, true, home);
						else
							printf("Comanda incorecta sau fisierul de output nu a putut fi deschis. (258)\n");
					}
					else{
						count = sscanf(bff1, "report -n -%c %s %s", &d, bff, cuv);
						if (count == 3 && ext){
							FILE *out = fopen(cuv, "w");
							if (d == 't' && out)
								grupare_fis_infectate_dupa_ext(ext, out, iprim, false, cuv);
							else if (d == 'c' && out)
								grupare_fis_infectate_dupa_vir(out, iprim, false, cuv);
							else
								printf("Comanda incorecta sau fisierul de output nu a putut fi deschis. (258)\n");
						}else {
							count = sscanf(bff1, "report -n -%c %s", &d, bff);
							if (count == 2){
								FILE *out = fopen(cuv, "w");
								if (d == 't' && out)
									grupare_fis_infectate_dupa_ext(ext, out, iprim, false, home);
								else if (d == 'c' && out)
									grupare_fis_infectate_dupa_vir(out, iprim, false, home);
								else
									printf("Comanda incorecta sau fisierul de output nu a putut fi deschis. (258)\n");
								}
								else{
									count = sscanf(bff1, "report -n %s %s", bff, cuv);
									if (count == 2 && ext){
										FILE *out = fopen(cuv, "w");
										if (out){
											fprintf(out, "Total:%i", afiseazaFI(iprim, cuv));
											fclose(out);
										}else
											printf("Comanda incorecta sau fisierul de output nu a putut fi deschis. (258)\n");
									}else {
										count = sscanf(bff1, "report -n %s", &d, bff);
										if (count == 2){
											FILE *out = fopen(cuv, "w");
											if (out){
												fprintf(out, "Total:%i", afiseazaFI(iprim, home));
												fclose(out);
											}
										}
									}
								}
							}
						}
					}

			/*
			cuv = strtok(NULL, " ");
			if (!strcmp(cuv, "-n")){
				cuv = strtok(NULL, " ");
				if (cuv){
					if (!strcmp(cuv, "-d"))
					{
						cuv = strtok(NULL, " ");
						if (cuv){
							if (!strcmp(cuv, "-t"))
						}
					}else if (!strcmp(cuv, "-t")){
						
					}else if (!strcmp(cuv, "-c"))
				}
			}
			 * */
			//log(bff1);
		}
		else
			printf("Comanda invalida!\n");
		cout << ">";
		fflush(stdin);
		cin.getline(bff, MAX_PATH);
		//scanf("%[^\n]s", bff);                
    }

	if (bd) fclose(bd);
	if (idx) fclose(idx);
    return 0;
}

char* toLowerCase(char* str){	
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

void log(char *x){
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

nodLista* adaugaLaInceput(nodLista *prim, char * s){
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
	nodAVL* temp = new nodAVL;
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
		bool gasit = false;
		nodAVL *temp = p;
		cautaAVLpath(temp, cale, gasit);
		if (!gasit){
			if (x < p->cheie)
				p->st = insereazaAVL(p->st, x, cale);
			else
				if (x > p->cheie)
					p->dr = insereazaAVL(p->dr, x, cale);
				else
					cout << "Nodul cu cheia dorita exista deja";
		}
		else
			cout << "Fisierul exista deja in arbore. (417)\n";
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
	char* den = new char[MAX_PATH/2];
	char* path = new char[MAX_PATH]; 
	char ext[5];
	int dim=0, crc=0;
	long int datc=0, datm=0;
	while (!feof(f))
		{
			long int poz = ftell(f);
			//denumire, path, ext, dimensiune, data creare, data mod, crc
			fscanf(f, "%s %s %s %i %li %li %i", den, path, ext, &dim, &datc, &datm, &crc);
			//frumos ar fi sa rescriu index file
			if (den && path && dim && crc){
				fprintf(idx, "%s %li\n", path, poz);
				p = insereazaAVL(p, poz, path);
			}
			//p = echilibreaza(p);
		}
	afiseazaAVL(p,0);
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
	return h%VTAB_MAX;
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

void adauga_in_bd_si_idx(FILE *bd, FILE *idx, nodAVL *&p, char* den, char*path, char* ext, long int dim, long int datc, long int datm, int crc){
	////denumire, path, ext, dimensiune, data creare, data mod, crc
	//denumre, poz in fisier	
	if(bd && idx){
		long int x = ftell(bd);
		fprintf(bd, "%s %s %s %li %li %li %i\n", den, path, ext, dim, datc, datm, crc);
		fprintf(idx, "%s %li\n", den, x);
		p = insereazaAVL(p, x, path);
	}
	else
		cout << "Fisierul bazei de date sau a indexului e invalid (410);\n";
}

//adauga in bd
void adauga_in_bd(FILE *bd, FILE *idx, char *cale, nodAVL *&p){
	DIR* dirr;
	struct dirent* curent;
	char path[MAX_PATH];
	char* ext = 0;
	
	if ((dirr=opendir(cale)) != NULL)
		while ((curent=readdir(dirr)) != NULL) {
			if (strncmp(curent->d_name, "..", 2) != 0 && strncmp(curent->d_name, ".", 1) != 0) {
				strncpy(path, cale, MAX_PATH);
				strncat(path, "/", 2);
				strncat(path, curent->d_name, MAX_PATH/2);
				if (curent->d_type == DT_DIR)
					adauga_in_bd(bd, idx, path, p);
				else {
					bool gasit = false;
					cautaAVLpath(p, path, gasit);
					if (!gasit){
						ext = strrchr(curent->d_name,'.');
						if (ext && extensie_vulnerabila(ext)){
							struct stat st;
							if (stat(path, &st)>=0){
								char *biti = new char[31];
								biti = getBiti(st.st_size);
								int crc = CRC(biti);
								adauga_in_bd_si_idx(bd, idx, p, curent->d_name, path, ext, st.st_size, st.st_ctime, st.st_mtime, crc);
							}
						}
					}
				}
			}
		}
	closedir(dirr);
	return;
}

void scaneaza_in_detaliu(FILE *bd, FILE *idx, char *cale, nodAVL *&p, nodTD **td, nodI *&iprim){
	bool gasit = false;
	cautaAVLpath(p, cale, gasit);
	if (gasit){
		//citesc despre
		rewind(idx);
		if (idx){
			char *bff = new char[MAX_PATH];
			long x;
			while (!feof(idx)){
				fscanf(idx, "%s %li", bff, &x);
				if (!strcmp(bff, cale)){
					fseek(bd, x, SEEK_SET);
					////denumire, path, ext, dimensiune, data creare, data mod, crc
					char* pathh = new char[MAX_PATH];
					char* extt = new char[MAX_PATH];
					int crc=0;
					long datc=0, datm=0, dim=0;
					fscanf(bd, "%s %s %s %li %li %li %i", bff, pathh, extt, &dim, &datc, &datm, &crc);
					if (pathh && dim && crc){
						struct stat st;
						if (stat(pathh, &st)>-1){
							char *biti = new char[31];
							biti = getBiti(st.st_size);
							if (crc != CRC(biti))
							{
								//verific semnatura
								char * semnttura = cautaSemnatura(td, biti);
								if (semnttura)
									iprim = insertNodI(iprim, pathh, extt, semnttura);
								else
								{
									//modific inregistrarea din bd folosind noul crc
									crc = CRC(biti);
									fseek(bd, x, SEEK_SET);
									fprintf(bd, "%s %s %s %li %li %li %i\n", bff, pathh, extt, dim, datc, datm, crc);
								}
							}
						}
					}
					break;
				}
			}
		}
	}
	else
	{
		//verific semnatura
		char *biti = new char[31];
		struct stat st;
		biti = getBiti(st.st_size);
		char * semnttura = cautaSemnatura(td, biti);
		char *ext = strrchr(cale, '.');
		if (semnttura && ext)
			iprim = insertNodI(iprim, cale, ext, semnttura);
		else
			adauga_in_bd(bd, idx, cale, p);
	}
}

void scan(FILE *bd, FILE *idx, char *cale, nodAVL *&p, nodTD **td, nodI *&iprim){
	DIR* dirr;
	struct dirent* curent;
	char path[MAX_PATH];
	
	if ((dirr=opendir(cale)) != NULL)
		while ((curent=readdir(dirr)) != NULL) {
			if (strncmp(curent->d_name, "..", 2) != 0 && strncmp(curent->d_name, ".", 1) != 0) {
				strncpy(path, cale, MAX_PATH);
				strncat(path, "/", 2);
				strncat(path, curent->d_name, MAX_PATH/2);
				if (curent->d_type == DT_DIR)
					scan(bd, idx, path, p, td, iprim);
				else {
					struct stat st;
					if (stat(path, &st)>=0)
						scaneaza_in_detaliu(bd, idx, path, p, td, iprim);
					}
			}
		}
	closedir(dirr);
	return;
}

bool insereazaCat(char *cat, nodTD **td){
	if (td!=NULL){
		int poz = hash(cat);
		if (td[poz] == NULL){
			nodTD *nou = new nodTD;
			nou->urm = NULL;
			strcpy(nou->semn, "categorie");
			strcpy(nou->nume, cat);
			td[poz] = nou;
		}
		return true;
	}
	return false;
}

bool insereazaInCat(char *cat, char *s, nodTD **td,char *semnn){
	int poz = hash(cat);
	nodTD *nou = new nodTD;
	nou->urm = NULL;
	strcpy(nou->semn, semnn);
	strcpy(nou->nume, s);
	nodTD *temp = td[poz];
	if (td[poz]!=NULL){
		bool gasit = false;
		while (temp->urm!=NULL){
			if (!strcmp(temp->urm->nume, s) || !strcmp(temp->urm->semn, semnn))
				gasit = true;
			temp = temp->urm;
		}
		if (!gasit) temp->urm = nou;
			return true;
	}
	else
		if (insereazaCat(cat, td)){
			td[poz]->urm = nou;
			return true;
		}
	return false;
}

void cautaCat(char* cat, nodTD **td){
	int poz = hash(cat);
	if(td[poz]!=NULL){
		nodTD *temp = td[poz];
		while (temp!=NULL){
			printf("Cat:%s Numele:%s Semnatura:%s\n", cat, temp->nume, temp->semn);
			temp = temp->urm;
		}
	}
	else
		printf("Categoria nu a putut fi gasita\n");
}

char* cautaSemnatura(nodTD **td, char *semnn){
	for (int i=0; i<VTAB_MAX; i++){
		if (td[i]!=NULL){
			nodTD *temp = td[i];
			while (temp!=NULL)
				if (!strcmp(temp->semn, semnn))
					return temp->nume;
				else
					temp = temp->urm;
		}
	}
	return NULL;
}

bool cautaSemnaturaDupaCat(nodTD **td, char *semnn, char *cat){
	int poz = hash(cat);
		if (td[poz]!=NULL){
			nodTD *temp = td[poz];
			while (temp!=NULL)
				if (!strcmp(temp->semn, semnn))
					return true;
				else
					temp = temp->urm;
	}
	return false;
}

char* getBiti(int x){
	char* biti = new char[31];
	for (int i=0;i<32;biti[i++]='0');
	for (int i=0; (1<<i)<=x && i<32; i++)
		if (x&(1<<i))
			biti[31-i]='1';
	return biti;
}

int CRC(char *s){
	int rez=0;
	
	for (int i=0; i<32; i++){
		rez ^= (s[i]-48) << (24);
		for (int j=0; j<8; j++)
			rez = (rez & (1<<31) ? (rez<<1) ^ CRC_GENERATOR : rez<<1);
	}
	return rez;
}

void injecteazaTD(nodTD **td, FILE *vir){
	char *nume = new char[50];
	char *cat = new char[31];
	char *semn = new char[31];
	while (!feof(vir)){
		fread(cat, sizeof(char), 50, vir);
		if (feof(vir))
			break;
		fread(nume, sizeof(char), 50, vir);
		fread(semn, sizeof(char), 31, vir);
		insereazaInCat(cat, nume, td, semn);
	}
}

void afiseazaTD(nodTD **td){
	for(int i=0; i<VTAB_MAX; i++)
		if (td[i]!=NULL){
			nodTD *temp = td[i];
			while (temp){
				printf("Nume:%s Semnatura:%s\n", temp->nume, temp->semn);
				temp = temp->urm;
			}
		}
}

void scrieTD(nodTD **td, FILE *v){
	nodTD *categ;
	for(int i=0; i<VTAB_MAX && v; i++)
		if (td[i]!=NULL){
			nodTD *temp = td[i];
			if (!strcmp(temp->semn, "categorie")){
				categ = td[i];
				while (temp){
					fwrite(categ->nume, sizeof(char), 50, v);
					fwrite(temp->nume, sizeof(char), 50, v);
					fwrite(temp->semn, sizeof(char), 31, v);
					temp = temp->urm;
				}
			}
		}
}

bool extensie_vulnerabila(char* den){
	FILE *ext;
	ext = fopen("/home/dinu/Documents/test/ext.txt", "r+");
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

nodI* creazaNodI(char *path, char *ext, char *categ){
	nodI* temp = new nodI;
	temp->path = new char[strlen(path)+1];
	temp->categ = new char[strlen(categ)+1];
	temp->ext = new char[strlen(ext)+1];
	temp->urm = NULL;
	strcpy(temp->path, path);
	strcpy(temp->categ, categ);
	strcpy(temp->ext, ext);
	return temp;
}

nodI* insertNodI(nodI *prim, char* path, char *ext, char *categ){
	nodI *nou = creazaNodI(path, ext, categ);
	if (prim == NULL)
		prim = nou;
	else{
		nodI *temp;
		temp = prim;
		bool ok = false;
		while (temp->urm!= NULL && !ok){
			if (!strcmp(temp->categ, categ)){
				nou->urm = temp->urm;
				temp->urm = nou;
				ok = true;
			}else
				temp = temp->urm;
		}
		if (!ok) temp->urm = nou;
	}
	return prim;
}

void grupare_fis_infectate_dupa_vir(FILE *out, nodI *prim, bool extra){
	int contor=0;
	nodI *temp = prim;

	if (out){
		while (prim!=NULL){
			fprintf(out, "Virus - %s:\n", temp->categ);
			temp = prim;
			if (temp)
			while (!strcmp(prim->categ, temp->categ)){
				contor++;
				if (extra)
					fprintf(out, "%s\n", temp->path);
				temp = temp->urm;
				if (temp==NULL)
					break;
			}
			prim = temp;
		}
		fprintf(out, "\nTotal:%i", contor);
	}
}

void grupare_fis_infectate_dupa_vir(FILE *out, nodI *prim, bool extra, char *p){
	int contor=0;
	nodI *temp = prim;

	if (out){
		while (prim!=NULL){
			fprintf(out, "Virus - %s:\n", temp->categ);
			temp = prim;
			if (temp)
			while (!strcmp(prim->categ, temp->categ)){
				if (!strncmp(temp->path, p, strlen(p)<strlen(temp->path)?strlen(p):strlen(temp->path))){
				contor++;
				if (extra)
					fprintf(out, "%s\n", temp->path);
				}
				temp = temp->urm;
				if (temp==NULL)
					break;
			}
			prim = temp;
		}
		fprintf(out, "\nTotal:%i", contor);
	}
}

//grupare fişiere infectate pe tipuri de fişiere target, conform ext.txt
void grupare_fis_infectate_dupa_ext(FILE *ext, FILE *out, nodI *prim, bool extra){
	int contor=0;
	rewind(ext);
	char *bf = new char[MAX_PATH];
	if (ext && out){
		while (!feof(ext)){
			fscanf(ext, "%s", bf);
			fprintf(out, "Extensie: %s\n", bf);
			nodI *temp = new nodI[1];
			temp = prim;
			bool g = false;
			while (temp!=NULL){
				if (!strcmp(temp->ext, bf)){
					g = true;
					contor++;
					if (extra)
						fprintf(out, "%s\n", temp->path);
				}
				temp = temp->urm;
			}
			if (!g)
				fprintf(out, "(none)\n");
		}
		fprintf(out, "\nTotal:%i", contor);
	}
}

void grupare_fis_infectate_dupa_ext(FILE *ext, FILE *out, nodI *prim, bool extra, char *p){
	int contor=0;
	rewind(ext);
	char *bf = new char[MAX_PATH];
	if (ext && out){
		while (!feof(ext)){
			fscanf(ext, "%s", bf);
			fprintf(out, "Extensie: %s\n", bf);
			nodI *temp = new nodI[1];
			temp = prim;
			bool g = false;
			while (temp!=NULL){
				if (!strcmp(temp->ext, bf) && !strncmp(temp->path, p, strlen(p)<strlen(temp->path)?strlen(p):strlen(temp->path))){
					g = true;
					contor++;
					if (extra)
						fprintf(out, "%s\n", temp->path);
				}
				temp = temp->urm;
			}
			if (!g)
				fprintf(out, "(none)\n");
		}
		fprintf(out, "\nTotal:%i", contor);
	}
}

int afiseazaFI(nodI *p, char *pr){
	int contor =0;
	while (p){
		printf("path:%s, ext:%s, categ:%s\n", p->path, p->ext, p->categ);
		if (!strncmp(p->path, pr, strlen(pr)<strlen(p->path)?strlen(pr):strlen(p->path)))
			contor++;
		p = p->urm;
	}
	return contor;
}


