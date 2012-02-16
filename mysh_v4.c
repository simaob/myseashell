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
#define NR_COMANDOS 5


/********************************************************
 *********** Ponto Numero 4 *****************************
 *Com Redireccionamento de Output e Input para ficheiros*
 ********************************************************/

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


void executa_comando(Comandos comando)
{
	int st;
	
	pid_t pid;

	pid = fork();
	
	if(pid ==0)
	{
		if(comando.output > 0)
		{
			dup2(comando.output, STDOUT_FILENO); //redireccionando o output!=)
		}
		if(comando.input > 0)
			dup2(comando.input, STDIN_FILENO);
		execvp(comando.argv[0], comando.argv);
			perror(comando.argv[0]);
		exit(1);
	}
	wait(&st);
	return;
}

int main(int argc, char * argv[], char *envp[]){


	char buscar[MAX_LINHA_COMANDOS];
	Comandos comandos[NR_COMANDOS];

	int controlo =0;

	int continuar = 1; //manter-se no ciclo principal
	int k=1;


	clear_screen();

	/***************************************************************
	 ******************* INTRO SCREEN ******************************
	 **************************************************************/

	printf("******************************************************************************\n");
	printf("*                            My Shell - Versao 4                             *\n");
	printf("*============Com Redireccionamento de Output e Input para ficheiros==========*\n");
	printf("* Desenvolvido no ambito de Sistemas Operativos - Disciplina do Terceiro Ano *\n");
	printf("* do Mestrado em Engenharia Informatica e de Computacao da Faculdade de      *\n");
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
	
	while(continuar==1) //Ciclo Principal
	{
		k = 1;
		printf("\nmysh> ");
		fgets(buscar, MAX_LINHA_COMANDOS, stdin);
		strtok(buscar, "\n"); //remover o \n inserido pelo fgets.
		controlo = separa_comandos(buscar, comandos);

		//Executar os comandos
		while (controlo >0)
		{	
			
			if(!strcmp(comandos[k-1].argv[0] ,"exit"))
			{
				continuar = 2;
				printf("Au revoir!\n");	
				break;
			}
			else{
				if(comandos[k-1].output ==-1 || comandos[k-1].input == -1) break; //Se der erro com o ficheiro de output
								//ou input comando nao é executado
					
				else{
				executa_comando(comandos[k-1]);
				k++; controlo--;
				}
			}
		}

	}

	
	return 0;
}

