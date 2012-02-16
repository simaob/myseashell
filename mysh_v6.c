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

//Tratamento de sinais
#include <signal.h>


#define MAX_LINHA_COMANDOS 150
#define NR_COMANDOS 5


/************************************************
 *********** Ponto Numero 5 *********************
 *********Execucao de comandos Simples **********
 ***********************************************/

//Permitir a utilizacao do tipo sighandler
typedef void (*sighandler_t)(int);

//pid_t pidfilho;

//void cc_handler(int signo)
//{
//	if(pidfilho != -1)
//	{
//		kill(-pidfilho,SIGKILL); //Kill todos os processos filhos em execucao
//		pidfilho = -1;
//	}
//}


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
	sighandler_t old_handler;
	pid = fork();
	
	if(pid > 0)
		old_handler = signal(SIGINT, SIG_IGN); //O pai Ignora o Ctrl+C
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
	signal(SIGINT, old_handler); //Repor o handler original
	return;
}


void executa_bg_comando(Comandos comando)
{

	pid_t filho_bg_pid;
	filho_bg_pid = fork();
	if(filho_bg_pid == 0)
	{	

		printf("[%s: PID = %d]\n", comando.argv[0], getpid());
		dup2(comando.input, STDIN_FILENO);
		execvp(comando.argv[0], comando.argv);
		perror(comando.argv[0]);
		exit(1);
	}
	waitpid(filho_bg_pid, NULL , WNOHANG);
	return;
}



int main(int argc, char * argv[], char *envp[]){


	char buscar[MAX_LINHA_COMANDOS];
	Comandos comandos[NR_COMANDOS];
	//sighandler_t old_handler;
	int controlo =0;

	int continuar = 1; //manter-se no ciclo principal
	int k=1;

	clear_screen();

	/***************************************************************
	 ******************* INTRO SCREEN ******************************
	 **************************************************************/

	printf("******************************************************************************\n");
	printf("*                            My Shell - Versao 6                             *\n");
	printf("*==========================Tratamento de Control + C=========================*\n");
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

	while(continuar==1) //Ciclo Principal
	{
		k = 1;
		fflush(stdout);
		fgets(buscar, MAX_LINHA_COMANDOS, stdin);
		strtok(buscar, "\n"); //remover o \n inserido pelo fgets.

		controlo = separa_comandos(buscar, comandos);

		//Executar os comandos
		while (controlo >0)
		{	
			//old_handler = signal(SIGINT, cc_handler);
			if(!strcmp(comandos[k-1].argv[0] ,"exit"))
			{
				continuar = 2;
				printf("Au revoir!\n");	
				break;
			}
			else{
				if(comandos[k-1].output ==-1 || comandos[k-1].input == -1) break; //Se der erro com o
								// ficheiro de output
								//ou input comando nao é executado
					
				else{
					if(comandos[k-1].forma == BACKGROUND)
					{
						executa_bg_comando(comandos[k-1]);
					}
					else if(comandos[k-1].forma == FOREGROUND)
					{
						executa_comando(comandos[k-1]);
					}
					k++; controlo--;
				}
			}
			//signal(SIGINT, old_handler);
		}
		if(continuar ==1)
		{
			printf("mysh> ");
		}
	}

	
	return 0;
}

