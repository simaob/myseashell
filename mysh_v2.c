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


/************************************************
 *********** Ponto Numero 2 *********************
 *********Execucao de comandos Simples **********
 ***********************************************/

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

//Funcao responsavel pela execucao dos comandos
int executa_comando(Comandos comando)
{
	int st;
	int saiu;

	int out = 1;
	
	pid_t pid;

	pid = fork();
	
	if(pid ==0)
	{
		saiu = execvp(comando.argv[0], comando.argv);
		if(saiu <0)
		{
			out = saiu;
			exit(1);
		}
	}
	wait(&st);
	return out;
}

int main(int argc, char * argv[], char *envp[]){


	char buscar[MAX_LINHA_COMANDOS];
	Comandos comandos[NR_COMANDOS];

	int controlo =0;

	int continuar = 1; //manter-se no ciclo principal
	int k=1;
	int comando_valido = 1;


	clear_screen();
	fflush(stdout);
	
	printf("******************************************************************************\n");
	printf("*                            My Shell - Versao 2                             *\n");
	printf("*============================ Comandos Simples ==============================*\n");
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


	while(continuar==1) //Ciclo Principal
	{
		k = 1;
		printf("mysh> ");
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
				comando_valido = executa_comando(comandos[k-1]);
				//printf("[%d] %s %s\n",k, comandos[k-1].argv[0], (comando_valido ==1 ? " " : "- Comando invalido"));
				k++; controlo--;
			}
		}

	}

	
	return 0;
}

