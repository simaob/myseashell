#include "comandos.h"

//Verifica se o utilizador quer apagar um ficheiro
int verifica_resposta(void){
	
	char resposta[MAX_RESPOSTA];
	int itsok = 0;

	printf("O ficheiro de destino ja existe, pretende destrui-lo? Sim ou N�?\n");
	
	fgets(resposta, MAX_RESPOSTA, stdin);
	strtok(resposta, "\n"); //remover o \n inserido pelo fgets.

	if(!strcmp(resposta, "Sim") || !strcmp(resposta, "sim") || !strcmp(resposta, "s") 
		|| !strcmp(resposta, "S"))
		itsok = 1;

	else if(!strcmp(resposta, "Nao") || !strcmp(resposta, "nao") || !strcmp(resposta, "n") 
		|| !strcmp(resposta, "N"))
		itsok = 2;
	while(itsok == 0)
	{
		printf("Por favor responda Sim ou Nao:\n");
		fgets(resposta, MAX_RESPOSTA, stdin);
		strtok(resposta, "\n"); //remover o \n inserido pelo fgets.

		if(!strcmp(resposta, "Sim") || !strcmp(resposta, "sim") || !strcmp(resposta, "s") || 
			!strcmp(resposta, "S"))
			itsok = 1;

		else if(!strcmp(resposta, "Nao") || !strcmp(resposta, "nao") || !strcmp(resposta, "n") 
			|| !strcmp(resposta, "N"))
			itsok = 2;
	}
	return itsok;
}

//Parse dos comandos
int separa_comandos(char * linha, Comandos comandos[])
{
	char *p;
	char *auxiliar=linha;
	char *resultado[90]; //Nr m�imo de elementos
	int i=0;
	int k =0;
	int resposta = 0;
	//int jahabackground = 0; //flag de controlo para nao haver multiplicidade de comandos com background
	
	int pipes[2];
	
	//Separar cada elemento da linha, por espa�s
	do
	{
    	 	resultado[i++]=auxiliar; /* save last startpoint */

    	 	p=strchr(auxiliar,' '); //procura um espa�

     		if(p!=0)     /* foi encontrado um espa� */
     		{
          		*p=0; /* substituir o espa� por zero */
          		auxiliar=p+1;  /* avan� o espa� */
     		}

	} while((p!=0)&&(i<49));

	resultado[i]=0;     /* Ultimo elemento da lista ser�um valor Nulo (zero)*/
	

	i=0;
	//defaults
	comandos[k].argc=0;
	comandos[k].argv = malloc(sizeof(char*)*90);
	comandos[k].forma = FOREGROUND;
	comandos[k].output = 0;
	comandos[k].input = 0;
	comandos[k].pipe = NOPIPE;
	while(resultado[i]!=0)
	{
		
   		if(strlen(resultado[i])==1) //se o elemento tiver tamanho 1
          		switch(*resultado[i]) 
          		{
				case '|': //Tratamento de pipes
					/* Definir o ficheiro de saida do primeiro comando
					 * Definir o ficheiro de entrada do proximo comando
					 */
					if(comandos[k].argc == 0) // Se nao houver comando antes da' erro...
					{
						printf("%s: Comando inv�ido\n", resultado[i]);
						return 0;
					}
					
					pipe(pipes); //get file descriptors
					//output vai para este comando
					comandos[k].output = pipes[1];
					comandos[k].pipe = PIPEBOSS;
					//proximo resultado eh o segundo comando q recebera a saida do pipe
					k++; i++; //avanca para o proximo comando e para o proximo comando
					//inicializacoes novo comando
					comandos[k].argc=0;
					comandos[k].argv = malloc(sizeof(char*)*50);
					comandos[k].forma = FOREGROUND;
					comandos[k].output = 0;
					comandos[k].input = 0;

					comandos[k].pipe = NOPIPE;
					//preencher o nome do comando em argv[0]
					comandos[k].argv[comandos[k].argc] = (char*) malloc(strlen(resultado[i]+1)*sizeof(char));
					strcpy(comandos[k].argv[comandos[k].argc],resultado[i]);
					comandos[k].argc++; //incrementar o nr de argumentos
					//abrir o ficheiro de input
					comandos[k].input = pipes[0];
					comandos[k].pipe = PIPESLAVE;

					break;
					
					
  			      case '&': //Tratamento de comandos em background
					if(comandos[k].argc == 0) // Se nao houver comando antes da' erro...
					{
						printf("%s: Comando inv�ido\n", resultado[i]);
						return 0;
					}
					if(comandos[k].input == 0){ //Se nao houver redireccionamento
						comandos[k].input = open("/dev/null", O_RDONLY);
						if(comandos[k].input ==-1)
							perror("/dev/null");
					}
					comandos[k].forma = BACKGROUND;
					//jahabackground++;
					break;
					
					
               			case '>': //vai haver redireccionamento de output
					if(comandos[k].argc == 0) // Se nao houver comando antes da' erro...
					{
						printf("%s: Comando inv�ido\n", resultado[i]);
						return 0;
					}
					comandos[k].output = open(resultado[++i], O_CREAT | O_WRONLY | O_EXCL , 0644);
					 //abrir o ficheiro
					if(comandos[k].output == -1) //Ficheiro j�existe, pretende apaga-lo?
					{
						//prompt na funcao
						resposta = verifica_resposta();
						if(resposta == 1)
						{
							comandos[k].output = open(resultado[i], 
								O_CREAT | O_WRONLY | O_TRUNC, 0644);
							
						}
						else
						{
							printf("%s - Comando nao executado!!\n", comandos[k].argv[0]);
							comandos[k].output = -1;
						}
					}
                  	        	break;
					
					
               			case '<': // redireccionamento de input
					if(comandos[k].argc == 0) // Se nao houver comando antes da' erro...
					{
						printf("%s: Comando inv�ido\n", resultado[i]);
						return 0;
					}
					comandos[k].input = open(resultado[++i], O_RDONLY);
					if(comandos[k].input == -1)
						perror(resultado[i]);
                    	        	break;
				case ';': //Multiplos comandos
					if(comandos[k].argc == 0) // Se nao houver comando antes da' erro...
					{
						printf("%s: Comando inv�ido\n", resultado[i]);
						return 0;
					}
					//if(jahabackground >0)
					//{
					//	printf("syntax error near unexpected token `;'\n");
					//	printf("ERRO: Por outras palavras nao podem ser executados dois comandos em background na mesma linha!!\n");
					//	return 0;
					//}
					else{
						k++; //"Completou" o comando e avanca para o proximo
						comandos[k].argc=0;
						comandos[k].argv = malloc(sizeof(char*)*90);
						comandos[k].forma = FOREGROUND;
						comandos[k].output = 0;
						comandos[k].input = 0;
						comandos[k].pipe = NOPIPE;
					}
					break;
               			default: //Caso seja um argumento normal
					comandos[k].argv[comandos[k].argc] = (char*) malloc(strlen(resultado[i]+1)*sizeof(char));
					strcpy(comandos[k].argv[comandos[k].argc],resultado[i]);
					comandos[k].argc++;
					break;
			}
		else{ //adicionar argumento ah estrutura
			comandos[k].argv[comandos[k].argc] = (char*) malloc(strlen(resultado[i]+1)*sizeof(char));
			strcpy(comandos[k].argv[comandos[k].argc],resultado[i]);
			comandos[k].argc++;
			

		}
   		i++; //avanca para o proximo resultado
	}
	return (k+1); //Retorna numero de comandos 
	
}

