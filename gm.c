#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include "vm.h"

int status;
unsigned addr;
int segmento;
char rw;
int *pageFault;
int *pidFilho;
int *indicePaginaVM;
char *bitM;
int *ini;
int *escrita;

int shmid, shmid2, shmid3, shmid4, shmid5, shmid6, shmid7, shmid8, shmid9;

clock_t Time[2];

struct	memoriaFisica{
		int numProc;
		int frequencia;
		char bitM;
	};typedef struct memoriaFisica MemoriaFisica;
	MemoriaFisica * memFisica;

unsigned BitWiseOffset(unsigned addr){
	unsigned offset;
	offset = addr;
	offset = offset << 12;
	offset = offset >> 12;
	return offset;
}

unsigned BitWiseNumpage(unsigned addr){
	unsigned numPage;
	numPage = addr;
	numPage = numPage >> 20;
	return numPage;
}

void CriaPageTable1();
void CriaPageTable2();
void CriaPageTable3();
void CriaPageTable4();

void PageFaultHandler(int sinal, siginfo_t *si, void * conteudo);
void RetiraPagina(int indice, int proc);
void ColocaPagina(int indice, int indiceVM, int proc);
void TabelaAtualizada(int sinal);
int ProcuraTabelaReverso(int indiceFrame, int proc);
void inicializa(int proc);
void TrocaPagina(int proc);
void SharedMemory();

int main (int argc, char * argv[]){
	fd = fopen("desempenho.log", "w");
	int shmid, shmid2, shmid3, shmid4, shmid5,shmid6, shmid7,shmid8, i;
	pid_t pid1, pid2, pid3, pid4;
	pid_t pidGM[4];
	
	//signal//////////////////////////////////////////////////
	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = PageFaultHandler;
	
	sigaction(SIGUSR1, &sig, NULL);
	signal(SIGUSR2, TabelaAtualizada);
	
	//Cria tabela de pagina para os processos
	CriaSM();
	
	//Memoria Compartilhada/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SharedMemory();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Inicializa todas as pages frames com frequencia 0;
	for(i=0;i<256;i++){
		memFisica[i].frequencia = 0;
		memFisica[i].bitM = 'r';
		memFisica[i].numProc = -1;
	}
	
	*ini = 0;
	*escrita = 0;
	//user process 1
	if((pid1 = fork())==0){
		pidFilho[0] = getpid();
		//PageTable
		CriaPageTable1();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		int i=0, j=0, erro; int flag=1;unsigned pageFrame, offset, numPage;
		FILE *fp = fopen("matriz.log", "r");
			while(fscanf(fp, "%x %c", &addr, &rw)==2){
				if(memFisica[255].numProc != -1){
					numPage = BitWiseNumpage(addr);
					offset = BitWiseOffset(addr);
					indicePaginaVM[0] = (addr>>20);
					bitM[0] = rw;
					pageFrame = trans(0,numPage, offset, rw);
					//Numero do processo que esta acessando a memoria fisica em uma determinada page frame
					if(pageFrame != -1){
						memFisica[(pageFrame>>20)].numProc = 0;
						memFisica[(pageFrame>>20)].bitM = rw;
						memFisica[(pageFrame>>20)].frequencia += 1;
					}
				}else{
					
					inicializa(0);
				}
			}

	//user process 2
	}else if((pid2 = fork()) == 0){
		pidFilho[1] = getpid();
		//PageTable
		CriaPageTable2();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		int i=0, j=0, erro; int flag=1;unsigned numPage;
		unsigned offset;
		unsigned pageFrame;
		FILE *fp = fopen("compressor.log", "r");
		while(fscanf(fp, "%x %c", &addr, &rw)==2){
			if(memFisica[255].numProc != -1){
				numPage = BitWiseNumpage(addr);
				offset = BitWiseOffset(addr);
				indicePaginaVM[1] = (addr>>24);
				bitM[1] = rw;
				pageFrame = trans(1,numPage, offset, rw);
				//Numero do processo que esta acessando a memoria fisica em uma determinada page frame
				if(pageFrame != -1){
					memFisica[(pageFrame>>20)].numProc = 1;
					memFisica[(pageFrame>>20)].bitM = rw;
					memFisica[(pageFrame>>20)].frequencia += 1;
				}
			}else{
				inicializa(1);
			}
		}
		
	//user process 3
	}else if((pid3 = fork()) == 0){
		pidFilho[2] = getpid();
		//PageTable
		CriaPageTable3();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		int i=0, j, erro; int flag=1;unsigned numPage;
		unsigned offset;
		unsigned pageFrame;
		FILE *fp = fopen("compilador.log", "r");
		while(fscanf(fp, "%x %c", &addr, &rw)==2){
			if(memFisica[255].numProc != -1){
				numPage = BitWiseNumpage(addr);
				offset = BitWiseOffset(addr);
				indicePaginaVM[2] = (addr>>20);
				bitM[2] = rw;
				pageFrame = trans(2,numPage, offset, rw);
				//Numero do processo que esta acessando a memoria fisica em uma determinada page frame
				if(pageFrame != -1){
					memFisica[(pageFrame>>20)].numProc = 2;
					memFisica[(pageFrame>>20)].bitM = rw;
					memFisica[(pageFrame>>20)].frequencia += 1;
				}
			}else{	
				inicializa(2);
			}
		}

	//user process 4
	
	}else if((pid4 = fork()) == 0){
			pidFilho[3] = getpid();
		//PageTable
		CriaPageTable4();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		int i=0, j, erro; int flag=1;unsigned numPage;
		unsigned offset;
		unsigned pageFrame;
		FILE *fp = fopen("simulador.log", "r");
		while(fscanf(fp, "%x %c", &addr, &rw)==2){
			if(memFisica[255].numProc != -1){
				numPage = BitWiseNumpage(addr);
				offset = BitWiseOffset(addr);
				indicePaginaVM[3] = (addr>>20);
				bitM[3] = rw;
				pageFrame = trans(3,numPage, offset, rw);
				//Numero do processo que esta acessando a memoria fisica em uma determinada page frame
				if(pageFrame != -1){
					memFisica[(pageFrame>>20)].numProc = 3;
					memFisica[(pageFrame>>20)].bitM = rw;
					memFisica[(pageFrame>>20)].frequencia += 1;
				}
			}else{
				inicializa(3);
			}
		}

/////Desempenho//////////////////////////////////////////////////////////////////////////////////////////////////
	}else if(fork()==0){
		Time[0] = clock();
		double tempo;
		while(1){
			usleep(30);
			Time[1] = clock();
			tempo = (Time[1] - Time[0])* 1000.0 / CLOCKS_PER_SEC;
			fprintf(fd, "Tempo de execucao: %f\n", tempo);
		}	
	}else if(fork()==0){
		while(1){
			usleep(90);
			fprintf(fd, "Numero de Page Fault: %d\n", *numPagefault);
		}
	}else if(fork()==0){
		while(1){
			usleep(120);
			fprintf(fd, "Numero de escritas: %d\n", *escrita);
		}
////////Gerenciador de Memória///////////////////////////////////////////////////////////////////////////////////
	
}else{
	//GM
	while(1){
		//Troca de paginas do Processo 1
		if(pageFault[0] == 0){
			pageFault[0] = -1;
			if((pidGM[0]=fork())==0){
				TrocaPagina(0);
			}
		}

	
		//Troca de paginas do Processo 2
		if(pageFault[1] == 1){
			pageFault[1] = -1;
			if((pidGM[1]=fork())==0){
				TrocaPagina(1);
			}
		}
		
		//Troca de paginas do Processo 3
		if(pageFault[2] == 2){
			pageFault[2] = -1;
			if((pidGM[2]=fork())==0){
				TrocaPagina(2);
			}
		}

		//Troca de paginas do Processo 4
		if(pageFault[3] == 3){
			pageFault[3] = -1;
			if((pidGM[3]=fork())==0){
				TrocaPagina(3);
			}				
		}
	
	}

}
return 0;
}


/////////FUNCOES//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PageFaultHandler(int sinal, siginfo_t *si, void * conteudo){
	if(si->si_pid == pidFilho[0]){
		pageFault[0] = 0;
	}else if(si->si_pid == pidFilho[1]){
		pageFault[1] = 1;
	}else if(si->si_pid == pidFilho[2]){
		pageFault[2] = 2;
	}else{
		pageFault[3] = 3;
	}
	kill(si->si_pid, SIGSTOP);
}

void TabelaAtualizada(int sinal){
}

void TrocaPagina(int proc){
	int i, indice, indiceTabela;
				indice = 0;
				int tempoSuspencao = 1;
				int menorValor = memFisica[0].frequencia;
				//Acha o frame coma menor frequencia que nao
				for(i=0;i<256;i++){
					if(menorValor > memFisica[i].frequencia){
						menorValor = memFisica[i].frequencia;
						indice = i;
					}
				}
				
				//Caso seja tenha a mesma frequencia troca o que nao tem bit M modificado
				for(i=0;i<256;i++){
					if((memFisica[indice].frequencia == memFisica[i].frequencia) && memFisica[i].bitM != 'W'){
						indice = i;
						break;
					}
				}
				
					if(memFisica[indice].bitM == 'R'){
						tempoSuspencao = 1;
					}else{
						tempoSuspencao = 2;
						(*escrita)++;
					}
					sleep(tempoSuspencao);
			
				
				indiceTabela = ProcuraTabelaReverso(indice, memFisica[indice].numProc);
				RetiraPagina(indiceTabela, memFisica[indice].numProc);
				ColocaPagina(indice, indicePaginaVM[proc], proc);
				
				//Envia sinal para o processo que perdeu o page frame
				for(i=0;i<4;i++){
					if(memFisica[indice].numProc == i){
						kill(pidFilho[i], SIGUSR2);
						break; 
					}
				}
				memFisica[indice].numProc = proc;
				memFisica[indice].frequencia = 0;
				memFisica[indice].bitM = bitM[proc];
				
				kill(pidFilho[proc], SIGCONT);
}

void RetiraPagina(int indice, int proc){
	if(proc == 0){
		//faz o swap
		page1[indice].pageNumber = -1;
	}
	
	if(proc == 1){
		//faz o swap
		page2[indice].pageNumber = -1;
	}
	
	if(proc == 2){
		//faz o swap
		page3[indice].pageNumber = -1;
	}
	
	if(proc == 3){
		//faz o swap
		page4[indice].pageNumber = -1;
	}
}

void ColocaPagina(int indice, int indiceVM, int proc){
	if(proc == 0){
		page1[indiceVM].pageNumber = indice;
	}
	
	if(proc == 1){
		page2[indiceVM].pageNumber = indice;
	}
	
	if(proc == 2){
		page3[indiceVM].pageNumber = indice;
	}
	
	if(proc == 3){
		page4[indiceVM].pageNumber = indice;
	}
}

void inicializa(int proc){
	if(proc == 0){
		if(page1[(addr>>20)].pageNumber == -1){
			page1[(addr>>20)].pageNumber = *ini;
			memFisica[*ini].numProc = 0;
			memFisica[*ini].bitM = rw;
			(*ini)++;
		}
	}else if(proc == 1){
		
		if(page2[(addr>>20)].pageNumber == -1){
			page2[(addr>>20)].pageNumber = *ini;
			memFisica[*ini].numProc = 1;
			memFisica[*ini].bitM = rw;
			(*ini)++;
		}
	}else if(proc == 2){
		
		if(page3[(addr>>20)].pageNumber == -1){
			page3[(addr>>20)].pageNumber = *ini;
			memFisica[*ini].numProc = 2;
			memFisica[*ini].bitM = rw;
			(*ini)++;
		}	
	}else{
		if(page4[(addr>>20)].pageNumber == -1){
			page4[(addr>>20)].pageNumber = *ini;
			memFisica[*ini].numProc = 3;
			memFisica[*ini].bitM = rw;
			(*ini)++;
		}
	}
}

int ProcuraTabelaReverso(int indiceFrame, int proc){
		int i;
		if(proc == 0){
			for(i=0;i<NUMPAGES;i++){
				if(page1[i].pageNumber == indiceFrame){
					return i;
				}
			}

		}else if(proc == 1){
			for(i=0;i<NUMPAGES;i++){
				if(page2[i].pageNumber == indiceFrame){
					return i;
				}
			}

		}else if(proc == 2){
			for(i=0;i<NUMPAGES;i++){
				if(page3[i].pageNumber == indiceFrame){
					return i;
				}
			}

		}else{
			for(i=0;i<NUMPAGES;i++){
				if(page4[i].pageNumber == indiceFrame){
					return i;
				}
			}

		}
}
	
void CriaPageTable1(){
int i, j;

for(i=0;i<NUMPAGES;i++){
		page1[i].pageNumber = -1;
	}
	
}

void CriaPageTable2(){
int i,j;


for(i=0;i<NUMPAGES;i++){
		page2[i].pageNumber = -1;
	}

}

void CriaPageTable3(){
int i,j;

for(i=0;i<NUMPAGES;i++){
		page3[i].pageNumber = -1;
	}
	
}

void CriaPageTable4(){
int i,j;

for(i=0;i<NUMPAGES;i++){
		page4[i].pageNumber = -1;
	}
}

void SharedMemory(){
	shmid = shmget((key_t)10,256 * sizeof(MemoriaFisica) , IPC_CREAT | 0666);
	memFisica = shmat(shmid, 0, 0);
	if(shmid == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	shmid2 = shmget((key_t)11, 4 * sizeof(int) , IPC_CREAT | 0666);
	indicePaginaVM = shmat(shmid2, 0, 0);
	if(shmid2 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	shmid3 = shmget((key_t)12, 4 * sizeof(char) , IPC_CREAT | 0666);
	bitM = shmat(shmid3, 0, 0);
	if(shmid3 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	shmid4 = shmget((key_t)13, 4 * sizeof(int) , IPC_CREAT | 0666);
	pidFilho = shmat(shmid4, 0, 0);
	if(shmid4 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	shmid5 = shmget((key_t)15, 4* sizeof(int) , IPC_CREAT | 0666);
	pageFault = shmat(shmid5, 0, 0);
	if(shmid5 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	
	shmid6 = shmget((key_t)16, sizeof(int) , IPC_CREAT | 0666);
	numPagefault = shmat(shmid6, 0, 0);
	if(shmid6 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	*numPagefault = 0;
	
	shmid7 = shmget((key_t)17, sizeof(int) , IPC_CREAT | 0666);
	numEscrita = shmat(shmid7, 0, 0);
	if(shmid7 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	
	shmid8 = shmget((key_t)18, sizeof(int) , IPC_CREAT | 0666);
	ini = shmat(shmid8, 0, 0);
	if(shmid8 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	
	shmid9 = shmget((key_t)19, sizeof(int) , IPC_CREAT | 0666);
	escrita = shmat(shmid9, 0, 0);
	if(shmid9 == -1){
		printf("Erro ao criar memoria compartilhada\n");
		exit(0);
	}
	*escrita = 0;
	*ini = 0;
}


