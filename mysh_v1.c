/**********************************************************
 **************Trabalho 1 de Sistemas Operativos***********
 *********************************************************/

//Includes
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Tratamento dos comandos
#include "comandos.h"

//Processos Filhos e wait
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINHA_COMANDOS 150

void clear_screen()
{
	int saiu = 0;
	int st;
	pid_t pid;
	pid = fork();		
	if(pid ==0)
	{
		saiu = execvp("clear",NULL);
		if(saiu <0)
			exit(1);
	}
	wait(&st);
	return;
}


int main(void){


	char buscar[MAX_LINHA_COMANDOS];
	Comandos comandos[5];
	int controlo =0;
    int i ;
	clear_screen();

	/***************************************************************
	 ******************* INTRO SCREEN ******************************
	 **************************************************************/

	printf("******************************************************************************\n");
	printf("*                            My Shell - Versao 1                             *\n");
	printf("*============================ Parse de Comandos =============================*\n");
	printf("* Desenvolvido no ambito de Sistemas Operativos - Disciplina do Terceiro Ano *\n");
	printf("* do Mestrado I. em Engenharia Informatica e de Computacao da Faculdade de   *\n");
	printf("* Engenharia da Universidade do Porto.                                       *\n");
	printf("*============================================================================*\n");
	printf("* Desenvolvida por:                                                          *\n");
	printf("*- Simao Belchior de Castro                                                  *\n");
	printf("*- 040509100                                                                 *\n");
	printf("*- Contactos                                                                 *\n");
	printf("*           - simao@fe.up.pt                                                 *\n");
	printf("*           - +351 91 790 17 60                                              *\n");
	printf("*                                                                            *\n");
	printf("*\"Handle with Care\"                                                          *\n");
	printf("******************************************************************************\n\n");

	fflush(stdout);
	printf("mysh> ");

	fgets(buscar, MAX_LINHA_COMANDOS, stdin);
	strtok(buscar, "\n"); //remover o \n inserido pelo fgets.
	controlo = separa_comandos(buscar, comandos);
	
    while(controlo > 0)
    {
         printf("Comando recebido:\n");
         printf("Nome do Comando: %s\n", comandos[controlo-1].argv[0]);
         printf("Argumentos:\n");
         for(i=0;i<comandos[controlo-1].argc;i++)
            printf("Arg[%d]: %s\n", i, comandos[controlo-1].argv[i]);
         printf("Output File Descriptor: %d\n", comandos[controlo-1].output);
         printf("Input File Descriptor: %d\n", comandos[controlo-1].input);
         printf("Forma de Execucao: %s\n", (comandos[controlo-1].forma == FOREGROUND) ? "Foreground" : "Background");
         printf("Pipe: %s\n", (comandos[controlo-1].forma == NOPIPE) ? "Sem Pipe" : ((comandos[controlo-1].forma == PIPEBOSS)? "Pipe Lado 1" : "Pipe Lado 2"));
          controlo--;              
    }	

	
	return 0;
}

