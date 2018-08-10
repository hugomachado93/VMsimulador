#include "vm.h"

	unsigned addr;
	char rw;

	void CriaSM(){
		int segmento1, segmento2, segmento3, segmento4;
		segmento1 = shmget((key_t)1, 4096 * sizeof(PageTable) , IPC_CREAT | 0666);
		page1 = shmat(segmento1, 0, 0);
		if(segmento1 == -1){
			printf("Erro ao criar memoria compartilhada\n");
			exit(0);
		}
		
		segmento2 = shmget((key_t)2,4096 * sizeof(PageTable) , IPC_CREAT | 0666);
		page2 = shmat(segmento2, 0, 0);
		
		if(segmento2 == -1){
			printf("Erro ao criar memoria compartilhada\n");
			exit(0);
		}
		
		segmento3 = shmget((key_t)3,4096 * sizeof(PageTable) , IPC_CREAT | 0666);
		page3 = shmat(segmento3, 0, 0);
		
		if(segmento3 == -1){
			printf("Erro ao criar memoria compartilhada\n");
			exit(0);
		}
		
		segmento4 = shmget((key_t)4,4096 * sizeof(PageTable) , IPC_CREAT | 0666);
		page4 = shmat(segmento4, 0, 0);
		
		if(segmento4 == -1){
			printf("Erro ao criar memoria compartilhada\n");
			exit(0);
		}
		
	}

	int ProcuraTabela(int indice, int proc){
		int i;
		if(proc == 0){
			for(i=0;i<NUMPAGES;i++){
				if(indice == i){
					return page1[i].pageNumber;
				}
			}

		}else if(proc == 1){
			for(i=0;i<NUMPAGES;i++){
				if(indice == i){
					return page2[i].pageNumber;
				}
			}

		}else if(proc == 2){
			for(i=0;i<NUMPAGES;i++){
				if(indice == i){
					return page3[i].pageNumber;
				}
			}

		}else{
			for(i=0;i<NUMPAGES;i++){
				if(indice == i){
					return page4[i].pageNumber;
				}
			}

		}
	}

	unsigned trans(int proc, unsigned indice, unsigned offset, char rw){
		int numPage;
		int quant = 0;
		unsigned pageFrame;
		
		numPage = ProcuraTabela(indice, proc);
		//PAGE FAULT
		//-1 indica que o processo nao esta mapeado na memoria fisica
		if(numPage == -1){
			printf("PAGE FAULT\n");
			(*numPagefault)++;
			kill(getpid(), SIGUSR1);
			return -1;
		}else{
			numPage = numPage << 20;
			pageFrame = numPage | offset;
			printf("%d ----------> 0x%08x %c\n", getpid(), pageFrame, rw);
			return pageFrame;
		}

	}
