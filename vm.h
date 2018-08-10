#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <signal.h>

#define NUMPAGES 4096

struct pageTable{

	int pageNumber;
	
};typedef struct pageTable PageTable;

int *numPagefault;
int *numEscrita;

FILE *fd;

//Page Table de cada processo
PageTable * page1;
PageTable * page2;
PageTable * page3;
PageTable * page4;

void CriaSM();

int ProcuraTabela(int indice, int proc);

unsigned trans(int proc, unsigned indice, unsigned offset, char rw);

#endif
