CC = gcc


all: v7 v6 v5 v4 v3 v2 v1
	@echo All Done!


v7: mysh_v7.o comandos.o
	gcc -o mysh_v7.exe mysh_v7.o comandos.o
	@echo Versao 7 a bombar!

v6: mysh_v6.o comandos.o
	gcc -o mysh_v6.exe mysh_v6.o comandos.o
	@echo Versao 6 a bombar!

v5: mysh_v5.o comandos.o
	gcc -o mysh_v5.exe mysh_v5.o comandos.o
	@echo Versao 5 a bombar!

v4: mysh_v4.o comandos.o
	gcc -o mysh_v4.exe mysh_v4.o comandos.o
	@echo Versao 4 a bombar!

v3: mysh_v3.o comandos.o
	gcc -o mysh_v3.exe mysh_v3.o comandos.o
	@echo Versao 3 a bombar!

v2: mysh_v2.o comandos.o
	gcc -o mysh_v2.exe mysh_v2.o comandos.o
	@echo Versao 2 a bombar!

v1: mysh_v1.o comandos.o
	gcc -o mysh_v1.exe mysh_v1.o comandos.o
	@echo Versao 1 a bombar!

%.o: %.c
	$(CC) -c $< -Wall
		
clean:
	rm *.exe *.o 
	@echo Clean and Ready to Rumble!