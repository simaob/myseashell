#ifndef _COMANDOS_H_
#define _COMANDOS_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//tratamento de ficheiros
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define MAX_CARACTERES 10
#define MAX_COMANDO 50

#define BACKGROUND 1
#define FOREGROUND 0

#define MAX_RESPOSTA 10

//O Boss eh o first end do pipe
#define PIPEBOSS 1
//O slave eh o second end do pipe
#define PIPESLAVE 2
#define NOPIPE 0

typedef struct{

	char **argv;
	int argc;
	int output;
	int input;
	int forma;
	int pipe;

}Comandos;


int separa_comandos(char * linha, Comandos comandos[]);
int verifica_resposta(void);

#endif
