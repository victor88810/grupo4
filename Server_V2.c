#include <string.h>
#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9250);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind\n");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	MYSQL * conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	int i;
	// bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		int terminar = 0;
		while(terminar==0)
		{
			// Ahora recibimos la petici?n
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			printf ("Peticion: %s\n", peticion);
			
			// vamos a ver que quieren
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			// Ya tenemos el c?digo de petici?n
			printf("%d \n",codigo);
			p = strtok( NULL, "/");
			char consulta[500]; //Donde guardaremos la consulta
			
			if(codigo==0) 	
				terminar=1;
			
			else if(codigo==1){	//Registro
				
				char username[20];
				char password[20];
				
				strcpy (username, p);
				printf("%s \n", username);
				p = strtok(NULL, "/");
				strcpy (password, p);
				printf("%s \n", password);
				
				int numero;
				int id;
				id =  rand() % 10000;
				numero = atoi(respuesta);
				char agregar[80];
				if(numero == 0){
					sprintf(agregar, "INSERT INTO Jugador VALUES ('%d','%s','%s',0,0);",id,username,password);					
					err=mysql_query (conn, agregar);
					if (err!=0) {
						printf ("Error al consultar datos de la base %u %s\n",
								mysql_errno(conn), mysql_error(conn));
						sprintf (respuesta,"1\n");
						exit (1);
					}
					printf("%s\n", agregar);
					sprintf (respuesta,"0\n");
					int numero;
					numero = atoi(respuesta);
					if(numero == 1){
						printf("Los datos introducidos no son correctos \n");
					}
					else	
					   printf("Usuario creado correctamente \n");
				}
			}
			
			else if(codigo==2){	//Iniciar sesion
				
				char username[20];
				char password[20];
				
				strcpy (username, p);
				printf("%s \n", username);
				p = strtok(NULL, "/");
				strcpy (password, p);
				printf("%s \n", password);
				
				strcpy(consulta, "SELECT COUNT(Nombre) FROM Jugador WHERE Jugador.Nombre ='");
				strcat(consulta, username);
				strcat(consulta, "' AND Jugador.Contraseña = '");
				strcat(consulta, password);
				strcat(consulta, "';");
				
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row == NULL)
					sprintf (respuesta,"No se han obtenido datos en la consulta\n");
				else
					while (row !=NULL) {
						sprintf (respuesta,"%s \n", row[0]);
						row = mysql_fetch_row (resultado);
				}
					int numero;
					numero = atoi(respuesta);
					if(numero == 1){
						printf("Usuario iniciado correctamente \n");
					}
					else	
					   printf("Los datos introducidos no son correctos \n");
			}
			
			else if (codigo==3){ //Partidas ganadas por un jugador
				char nombre[20];
				printf ("Codigo: %d\n", codigo);
				
				strcpy (nombre, p);
				
				strcpy(consulta, "SELECT Jugador.Fondos FROM Jugador WHERE Jugador.Nombre ='");
				strcat(consulta, nombre);
				strcat(consulta, "';");
				
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL)
					sprintf (respuesta,"No se han obtenido datos en la consulta\n");
				else
					while (row !=NULL) {
						sprintf (respuesta,"%s\n", row[0]);
						row = mysql_fetch_row (resultado);
				}
			}
			
			else if (codigo ==4){ //Saber Fondos
				
				char nombre[20];
				printf("%s \n", nombre);
				strcpy (nombre, p);
				strcpy (consulta, "SELECT Jugador.Fondos FROM Jugador WHERE Jugador.Nombre ='");
				strcat(consulta, nombre);
				strcat(consulta, "';");
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL)
					sprintf (respuesta,"No se han obtenido datos en la consulta\n");
				else
					while (row !=NULL) {
						sprintf (respuesta,"%s", row[0]);
						row = mysql_fetch_row (resultado);
				}
			}
			
			else if(codigo==5){ //Partidas en común
				printf ("Codigo: %d\n", codigo);
				char nombre1[20];
				char nombre2[20];
				strcpy (nombre1, p);
				p = strtok(NULL, "/");
				strcpy (nombre2, p);
				
				strcpy(consulta, "SELECT * FROM Jugador ORDER BY Fondos DESC");
				strcat(consulta, "');");
				printf("%s\n", consulta);
				err=mysql_query (conn, consulta);
				printf("%d\n", err);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL)
					sprintf (respuesta,"No se han obtenido datos en la consulta\n");
				else
					while (row !=NULL) {
						sprintf (respuesta,"%s", row[0]);
						row = mysql_fetch_row (resultado);
						//sprintf (respuesta,"%s, %s\n", respuesta, row[0]);
						//row = mysql_fetch_row (resultado);
				}
					
			}
			
			printf ("Respuesta: %s\n", respuesta);
			// Enviamos la respuesta
			write (sock_conn,respuesta, strlen(respuesta));
		}
		// Se acabo el servicio para este cliente
		close(sock_conn); 
	}
}
