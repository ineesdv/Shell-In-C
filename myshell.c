#include <stdio.h>

#include "parser.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "TElemento.h"
#include "simplelinkedlist.h"


//Colores
#define boldText "\033[1m"
#define italicText "\033[3m"
#define underlinedText "<\033[4m"
#define blinkText "\033[5m"
#define greenColor "\e[0;32m"
#define redColor "\e[0;31m\033[1m"
#define blueColor "\e[0;34m\033[1m"
#define yellowColor "\e[0;33m\033[1m"
#define purpleColor "\e[0;35m\033[1m"
#define turquoiseColor "\e[0;36m\033[1m"
#define grayColor "\e[0;37m\033[1m"
#define normalText "\033[0m"
#define endColor "\033[0m\e[0m"

void exit_msh();
void cd(tline* line);
void stop_signals();
void continue_signals();
void redirect_output(tline line);
int redirect_input(tline line);
void print_msh();
void redirect_error(tline line);
//void handlerSignalCtrlZ(int sigsum, TLinkedList* jobs);
//void imprimirJobs(TLinkedList jobs);
//void ampersand(tline line, TLinkedList *jobs, TElemento new);
//void bg(tline line, TLinkedList* jobs);
//void fg(tline line, TLinkedList* jobs);




int main(void) {

	int in = dup(0);	
	int out = dup(1);
	int error = dup(2);

	char buf[1024];
	tline * line;
	int i,j;
	
	//TLinkedList jobs;
	//crearVacia(&jobs);

    print_msh();

	while (fgets(buf, 1024, stdin)) {
		
		stop_signals();

		line = tokenize(buf);
		fflush(stdin);
		fflush(stdout);

		//Línea vacía
		if (line==NULL) {
			continue;
		}

		//Un solo comando
		if (line->ncommands == 1){ 
					
		
			if (strcmp(line->commands[0].argv[0], "exit") == 0){
				exit_msh();

			}else if (strcmp(line->commands[0].argv[0], "cd") == 0){ 
				cd(line);

			}else if(strcmp(line->commands[0].argv[0], "jobs") == 0){ 
				
				if (line->commands[0].argc > 1){

					printf("Error de sintaxis. Uso: jobs.\n");
					continue;

				}else {
					//imprimirJobs(jobs);
				}


			}else if(strcmp(line->commands[0].argv[0], "bg") == 0){ 
				
				if (line->commands[0].argc != 2){

					printf("Error. Uso: bg <id>\n");
					continue;

				}else {

					//bg(*line, &jobs);
				}


			}else if(strcmp(line->commands[0].argv[0], "fg") == 0){ 
				
				if (line->commands[0].argc != 2){

					printf("Error. Uso: fg <id>\n");
					continue;

				}else {

					//fg(*line, &jobs);
				}


			}else if(!line->commands[0].filename){ //No existe el filename 
				printf("No existe el comando introducido\n");
				continue;

			}else{ 
				//printf("Orden 0 (%s):\n", line->commands[0].filename);
				pid_t pidHijo;
				pidHijo = fork();
				if (pidHijo < 0){
					printf("Error al crear el hijo\n");
					return -1;
				}
				
				if(pidHijo == 0){
					//signal(SIGTSTP, handlerSignalCtrlZ);
					continue_signals();

					if (line->redirect_output != NULL && i == (line->ncommands-1)) { 
						// Redirección output (>)
						//printf("redirección de salida: %s\n", line->redirect_output);
						
						redirect_output(*line);
                        execvp(line->commands[0].filename, line->commands[0].argv);

					}else if (line->background) {
						printf("comando a ejecutarse en background\n"); // No terminado, abajo del todo están las funciones
						/*
						TElemento newJob;
						newJob.pid = pidHijo;
						newJob.status = 'B';
						strcpy(newJob.command, buf);
						ampersand(*line, &jobs, newJob); //comando &
                        */

					}else if (line->redirect_input != NULL && i==0) { 
						// Redirección input (<)
						//printf("redirección de entrada: %s\n", line->redirect_input);
						fflush(stdin);
						fflush(stdout);
						if(redirect_input(*line)){
							fflush(stdin);
							fflush(stdout);
                            execvp(line->commands[0].filename, line->commands[0].argv);
						}else{
							exit(1);
						}

					}else if (line->redirect_error != NULL && i == (line->ncommands-1)) { 
						// Redirección output (>&)

						//printf("redirección de error: %s\n", line->redirect_error);
						//crear | modo apertura (escribir+crear) | permisos
						fflush(stdin);
						fflush(stdout);
                        redirect_error(*line);
						fflush(stdin);
						fflush(stdout);
						execvp(line->commands[0].filename, line->commands[0].argv);

					}else{
                        //Sin redirección
						fflush(stdin);
						fflush(stdout);
                        execvp(line->commands[0].filename, line->commands[0].argv);
                        fprintf(stderr, "Se ha producido un error en la ejecución del comando.\n");
                        exit(1);
                    }
				}
				wait(NULL);
				printf("\n");
			}

		} else{ //Varios comandos
			
			//Creación de pipes (READ/WRITE)
			int pipes[line->ncommands-1][2];
			
			for (int i = 0; i < line->ncommands; i++){
				pipe(pipes[i]);
			}

			for (i=0; i<line->ncommands; i++) { 
				//printf("Orden %d (%s):\n", i, line->commands[i].filename);
				//printf("Estamos en una iteración del for\n");
				pid_t pidHijo;
				pidHijo = fork();
				if (pidHijo < 0){
					printf("Error al crear el hijo\n");
					return -1;
				}
				
				
				if(pidHijo == 0){
					continue_signals();
					//signal(SIGTSTP, handlerSignalCtrlZ);
					//printf("Soy el hijo\n");

					//Primer proceso
					if (i==0){

						if (line->redirect_input != NULL) {
							//Consideramos redirección de entrada (<)
							//printf("redirección de entrada: %s\n", line->redirect_input);

							if(!redirect_input(*line)){
								exit(1);
							}
								
						}

						//printf("Soy el primero\n");

						close(pipes[0][0]); 
						dup2(pipes[0][1], 1); 

						execvp(line->commands[i].filename, line->commands[i].argv);
						fprintf(stderr, "%s: No se encuentra el mandato.\n", line->commands[0].argv[0]);
						exit(1);

					}

					//Proceso nésimo
					else if(i != 0 && i != (line->ncommands-1) ){
						
						close(pipes[i-1][1]);
						close(pipes[i][0]);
						for (int j = 0; j < line->ncommands-1; j++)
						{
							if ( j < i-1 && j > i){ 
								close(pipes[j][0]);
								close(pipes[j][1]);
							}
						}

						dup2(pipes[i-1][0], 0); 
						dup2(pipes[i][1], 1); 
						execvp(line->commands[i].filename, line->commands[i].argv);
						fprintf(stderr, "%s: No se encuentra el mandato.\n", line->commands[0].argv[0]);
						exit(1);

					}

					//Último proceso
					else{
						
						if (line->redirect_output != NULL && i == (line->ncommands-1)) { 
							//Consideramos redirección de salida (>)
							//printf("redirección de salida: %s\n", line->redirect_output);

							redirect_output(*line);

						
						}else if (line->redirect_error != NULL && i == (line->ncommands-1)) { 
							//Consideramos redirección de error (>&)
							//printf("redirección de error: %s\n", line->redirect_error);
							
							redirect_error(*line);

						}

						close(pipes[i-1][1]);
						for (int j = 0; j < line->ncommands-2; j++)
						{
							close(pipes[j][0]);
							close(pipes[j][1]);

						}
					
						dup2(pipes[i-1][0], 0); // Leemos del array de pipe[actual][read]
						execvp(line->commands[i].filename, line->commands[i].argv);
						fprintf(stderr, "%s: No se encuentra el mandato.\n", line->commands[0].argv[0]);
						exit(1);

					}


					
				}
			}

			//printf("Voy a cerrar las tuberias\n");
			for (int j=0; j<(line->ncommands-1); j++) {
				close(pipes[j][1]);
				close(pipes[j][0]);
			}		
		
			for(int i=0; i<line->ncommands; i++) {
				wait(NULL);
				//printf("Soy el padre y ha terminado un hijo\n");
			}
		
		}

		//Restaurar descriptores estándar
		dup2(in, 0);
		dup2(out, 1);
		dup2(error, 2);
	
		print_msh();
	}
	return 0;
}


// FUNCIONES BÁSICAS SHELL

void print_msh(){
	char user[100];
	char cwd[100];
	strcpy(user, getenv("USER"));
	getcwd(cwd, sizeof(cwd));
	printf("%s%s%s@%s%s%s:%smsh> %s", blueColor, user, grayColor, turquoiseColor, cwd, grayColor, purpleColor, normalText);	

}

void exit_msh(){

	printf("Adiós :)\n");
	exit(0);

}

void cd(tline* line){
    char* ruta;
	if (line->commands[0].argc > 2){
		printf("Error de sintaxis. Uso: cd <ruta>\n");

	}else{
		if (line->commands[0].argc == 1 || strcmp(line->commands[0].argv[1], "~") == 0 ){ // HOME
			ruta = getenv("HOME"); //coger la variable que tiene el home del usuario actual

		}else{
			ruta = line->commands[0].argv[1];
			
		}
		chdir(ruta);
	} 
}

void stop_signals(){
	signal(SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
}

void continue_signals(){
	signal(SIGINT, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
}


// FUNCIONES DE REDIRECCIÓN

void redirect_output(tline line){
	//crear | modo apertura (escribir+crear) | permisos
    int newfile = open(line.redirect_output, O_WRONLY|O_CREAT, 0777); 
    dup2(newfile, 1);
}

int redirect_input(tline line){
	//crear | modo apertura (leer)
	int newfile = open(line.redirect_input, O_RDONLY); 
	if (newfile == -1){
		printf("No se puede abrir el archivo.\n");
		return 0;
	}else{
		dup2(newfile, 0); 
		return 1;
	}
}

void redirect_error(tline line){
	//crear | modo apertura (escribir+crear) | permisos
	int newfile = open(line.redirect_error, O_WRONLY|O_CREAT, 0777); //crear fichero vacío
    dup2(newfile, 2); 
}


// ####################################################
// 				FUNCIONES DEL BACKGROUND
// ###################################################
// En proceso, falta por implementar del todo.
/*
void handlerSignalCtrlZ(int sigsum, TLinkedList* jobs){
	signal(SIGTSTP, handlerSignalCtrlZ); //cuando hay un control z (señal de SIGTSTP) lo maneja la función
	pid_t pid_process = getpid();

	kill(pid_process, SIGTSTP);

	TElemento new;
	new.pid = pid_process;
	//new.id = ultimo de la lista +1
	//new.status='B';
	//strcpy del comando
	//insertarFinal(aux, jobs);

    printf("Job detenido y añadido a la lista.\n");
     
}



void imprimirJobs(TLinkedList jobs){

	if(!esVacia(jobs)){
		mostrar(jobs);

	}else{
		printf("No hay jobs.\n");
	}
}


void ampersand(tline line, TLinkedList *jobs, TElemento new){

	int id_requested = line.commands[0].argv[1]; //coger el numero: bg 1

	if(!contienePorID(id_requested, jobs)){ // si no está
		printf("El job seleccionado no existe.\n");
		
	}else{
		insertarFinal(new, jobs);
		kill(new.pid, SIGCONT);
		printf("Job enviado al background\n");
	}
	
}


void bg(tline line, TLinkedList* jobs){

	char c = line.commands[0].argv[1];
	int id_requested = int(c);

	if(contienePorID(id_requested, jobs)){
		int pid = devolverPID(id_requested, jobs);
		kill(pid, SIGCONT);
		cambiarStatus(id_requested, jobs, 'B');

	}else{
		printf("No se ha encontrado el job.\n");
	}
    return 0;
}

void fg(tline line, TLinkedList* jobs){

	int id_requested = line.commands[0].argv[1];
	pid_t pid_padre = getpid();


	if(contienePorID(id_requested, jobs)){
		int pid = devolverPID(id_requested, jobs);
		//establece job en foreground
		tcsetpgrp (pid_padre, pid);
		kill(pid, SIGCONT);
		//esperamos a que termine (ya que está en fg)
		wait_for_job(pid);
		//poner la shell normal en foreground
		tcsetpgrp(pid_padre, pid_padre);

	}else{
		printf("No se ha encontrado el job.\n");
	}
    return 0;
}

*/

