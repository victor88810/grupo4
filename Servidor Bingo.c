#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>

int sockets[100];

typedef struct{
	char orden[20];
}Orden;
typedef struct{
	int num;
	Orden servicios[100];
}Servicios;
Servicios lista_servicios;

typedef struct{
	char jugador[20];
	int socket;
}Usuarios;
typedef struct{
	int num;
	Usuarios usuarios[100];
}Conectados;
Conectados lista_conectados;

pthread_mutex_t mutex /*= PTHREAD_MUTEX_INITIALIZER*/;

int PonLista(Conectados *l, char jugador[20],int socket)
{
	if (l->num<100)
	{
		strcpy(l->usuarios[l->num].jugador,jugador);
		l->usuarios[l->num].socket=socket;
		l->num++;
		return 0;
	}
	else
		return -1;
}
int EliminarLista(Conectados *l, int socket)
{
	int encontrado=0;
	int i=0;
	while((i<l->num) && (encontrado==0))
	{
		if(l->usuarios[i].socket==socket)
			encontrado=1;
		else
			i++;
	}
	if (!encontrado)
		return -1;
	else
	{
		while(i<l->num-1)
		{
			l->usuarios[i]=l->usuarios[i+1];
			i++;
		}
		l->num=l->num-1;
		return 0;
	}
}
int UsuarioExistente(char Usuario[20])
{
	MYSQL *conn;
	int err;
	//++++++++++++++++++++++++++++++++++Almacenar consulta
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//++++++++++++++++++++++++++++++++++Establecemos conexion
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//++++++++++++++++++++++++++++++++++++++++++CONSULTA
	sprintf(consulta, "SELECT Jugador.Username FROM Jugador WHERE Jugador.Username = '%s'\n", Usuario);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	if (row == NULL){ //no hay usuarios con ese nombre
		return 0;
		printf("No existe el usuario\n");
	}
	
	else            //nombre ya existe
		return -1;
	
}
int Registro(char usuario[20], char password[20])
{
	MYSQL *conn;
	int err;
	//++++++++++++++++++++++++++++++++++Almacenar consulta
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//++++++++++++++++++++++++++++++++++Establecemos conexion
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//++++++++++++++++++++++++++++++++++++++++++CONSULTA
	sprintf(consulta,"INSERT INTO Jugador VALUES ('%s', '%s', 'NuevoJugador', 0);",usuario, password);
	printf("%s\n",consulta);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	
	mysql_close (conn);
	return 0;
}
int IniciarSesion(char username[20], char password[20])
{
	MYSQL *conn;
	int err;
	//++++++++++++++++++++++++++++++++++Almacenar consulta
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [100];
	//++++++++++++++++++++++++++++++++++Establecemos conexion
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//++++++++++++++++++++++++++++++++++++++++++CONSULTA
	sprintf(consulta, "SELECT Jugador.Username FROM Jugador WHERE Jugador.Username='%s' AND Jugador.Password='%s' ",username, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	return 0;
	
}

int Fondos(char nombre[200], char respuesta[512])
{
	MYSQL *conn;
	int err;
	//++++++++++++++++++++++++++++++++++Almacenar consulta
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//++++++++++++++++++++++++++++++++++Establecemos conexion
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//++++++++++++++++++++++++++++++++++++++++++CONSULTA
	strcpy(consulta, "SELECT Jugador.Fondos FROM Jugador WHERE Jugador.Username ='");
	strcat(consulta, nombre);
	strcat(consulta, "';");
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	
	resultado = mysql_store_result (conn);
	
	/*char campeon[20] = mysql_store_result(resultado);*/
	row = mysql_fetch_row (resultado);
	//al obtener la consulta
	
	if (row == NULL)
		sprintf (respuesta,"No se han obtenido datos en la consulta\n");
	else
		while (row !=NULL) {
			printf("%s\n", row[0]);
			sprintf (respuesta,"%s\n", row[0]);
			printf("%s\n", respuesta);
			row = mysql_fetch_row (resultado);
	}
	return 0;
	
	}

int JugadorContrincante(char nombre[20], char respuesta [200])
{
	MYSQL *conn;
	int err;
	//++++++++++++++++++++++++++++++++++Almacenar consulta
	MYSQL_RES *resultado1;
	MYSQL_ROW row1;
	
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [300];
	//++++++++++++++++++++++++++++++++++Establecemos conexion
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//++++++++++++++++++++++++++++++++++++++++++CONSULTA
	sprintf(consulta, "SELECT Game.Identificador FROM (Jugador, Partida, Game) WHERE Game.Username = Jugador.Username AND Jugador.Username = '%s';", nombre);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	sprintf(consulta, "SELECT Jugador.Username FROM (Jugador, Partida, Game) WHERE Game.Username = Jugador.Username AND Game.Identificador = '%s';", row[0]);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	
	resultado1 = mysql_store_result (conn);
	row1 = mysql_fetch_row (resultado1);
	if (row == NULL)
		sprintf (respuesta,"No se han obtenido datos en la consulta\n");
	else
	{	while (row !=NULL) 
	{
		// la columna 0 contiene el nombre del jugador
		sprintf (respuesta,"%s\n", row[0]);
		// obtenemos la siguiente fila
		row = mysql_fetch_row (resultado1);
	}
	printf("%s\n",respuesta);
	return 0;
	}
	/*strcpy ( tiempo, respuesta);*/ 
}
int Ranking(char respuesta[200])
{
	MYSQL *conn;
	int err;
	//++++++++++++++++++++++++++++++++++Almacenar consulta
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//++++++++++++++++++++++++++++++++++Establecemos conexion
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Bingo",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//++++++++++++++++++++++++++++++++++++++++++CONSULTA
	strcpy(consulta, "SELECT * FROM Jugador ORDER BY Fondos DESC;");
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	printf("%s %s %s\n", row[0], row[1], row[2]);
	row = mysql_fetch_row (resultado);
	printf("%s %s %s\n", row[0], row[1], row[2]);
	if (row == NULL)
	{	
		sprintf (respuesta,"No se han obtenido datos en la consulta\n");
		return (0);
	}
	else
	{	while (row !=NULL) 
	{
		// la columna 0 contiene el nombre del jugador
		sprintf (respuesta,"%s\n", row[0]);
		// obtenemos la siguiente fila
		row = mysql_fetch_row (resultado);
	}
	printf("%s\n",respuesta);
	int ganadas= atoi (respuesta);
	return ganadas;
	}
}

void *AtenderCliente(void *socket)
{
	
	int sock_conn;
	int ret;
	int *s;
	s = (int*) socket;
	sock_conn = *s;
	char buff[512];
	char respuesta[512];
	
	char user[20];
	int terminar = 0;
	while (terminar == 0)
	{
		// Recibimos peticion de un usuario
		ret=read(sock_conn,buff, sizeof(buff));
		printf ("Recibido\n");
		
		buff[ret]='\0'; //fin de string
		
		//Escribimos en consola quien se ha conectado
		printf ("El código de peticion es: %s\n",buff);
		
		//Servicios
		
		char *servicios = strtok(buff, "/");
		int codigo = atoi (servicios);
		int i=0;
		while ((servicios = strtok(NULL,"/")) != NULL)
		{
			//mensaje troceado
			strcpy(lista_servicios.servicios[i].orden,servicios);
			i++;
		}
		
		//situaciones
		if (codigo==0)
		{
			int socket;
			printf("Desconectar socket %d\n", sock_conn);
			socket=sock_conn;
			pthread_mutex_lock(&mutex);
			int e= EliminarLista (&lista_conectados,socket);
			pthread_mutex_unlock(&mutex);
			int j;
			j=0;
			strcpy(respuesta, "6/0/");
			char numero[10];
			sprintf(numero, "%d", lista_conectados.num);
			strcat(respuesta, numero);
			
			while ( j < lista_conectados.num)
			{
				strcat(respuesta, "/");
				strcat (respuesta, lista_conectados.usuarios[j].jugador);
				j = j + 1;
			}
			printf("%s\n", respuesta);
			/*				write (sock_conn,respuesta, strlen(respuesta));*/
			
			//enviar por todos los sockets q tengo conectados en ese momento
			int k;
			for(k=0; k<lista_conectados.num; k++)
				write(lista_conectados.usuarios[k].socket,respuesta, strlen(respuesta));
			terminar=1;
		}
		
		if(codigo==1)
		{                       	//Registro
			
			char username[20];
			char password[20];
			
			strcpy (username, lista_servicios.servicios[0].orden);
			strcpy (password, lista_servicios.servicios[1].orden);
			int existe = UsuarioExistente(username);
			printf("%d\n",existe);
			if(existe == 0)
			{
				int r = Registro(username,password);
				strcpy(respuesta, "1/0");
				
			}
			else //usuario exitente
			{		
				strcpy(respuesta, "1/-1");
				printf("Error Registro");
			}
			write(sock_conn,respuesta,strlen(respuesta));
		}
		if (codigo == 2) //Inicio Sesion
		{
			
			char username [20];
			char password[20];
			strcpy (username, lista_servicios.servicios [0].orden);
			strcpy (password, lista_servicios.servicios[1].orden);
			int lerr= IniciarSesion(username, password);
			if (lerr==0)
			{
				strcpy ( respuesta, "2/0");
				char jugador[20];
				int socket;
				strcpy(jugador, username);
				socket=sock_conn;
				pthread_mutex_lock(&mutex);
				write (sock_conn, respuesta, strlen(respuesta));
				//cont=cont+1;
				
				pthread_mutex_unlock (&mutex); // Ya puedes interrumpirme
				
				int p= PonLista(&lista_conectados, jugador, socket);
				// notificar a todos los clientes conectados
				int j;
				j=0;
				strcpy(respuesta, "6/0/");
				char numero[10];
				sprintf(numero, "%d", lista_conectados.num);
				strcat(respuesta, numero);
				
				while ( j < lista_conectados.num)
				{
					strcat(respuesta, "/");
					strcat (respuesta, lista_conectados.usuarios[j].jugador);
					/*						strcat(respuesta, "/");*/
					
					j = j + 1;
				}
				strcat(respuesta, "/");
				printf("%s\n", respuesta);
				/*					respuesta[strlen(respuesta)-1]='\0';*/
				/*					printf("%s\n", respuesta);*/
				/*write (sock_conn,respuesta, strlen(respuesta));*/
				
				//enviar por todos los sockets q tengo conectados en ese momento
				int k;
				for(k=0; k<lista_conectados.num; k++)
					write(lista_conectados.usuarios[k].socket,respuesta, strlen(respuesta));
				
				/*pthread_mutex_unlock(&mutex);*/
				/*printf("re\n");*/
				
			}
			else
				strcpy (respuesta, "2");
			
			write (sock_conn, respuesta, strlen(respuesta));
			
		}
		
		if (codigo==3) //Fondos
		{
			char nombre[200];
			char prueba[20];
			strcpy (nombre, lista_servicios.servicios [0].orden);
			int Consulta = Fondos(nombre, prueba);
			
			
			if (Consulta==0)
			{
				sprintf (respuesta,"3/0/%s\n",prueba);
				printf("Hola: %s",respuesta);
				
			}
			else
			{
				strcpy(respuesta,"Error");
			}
			write (sock_conn,respuesta, strlen(respuesta));
		}
		if(codigo ==4) //Jugador contrincante
		{
			char nombre[20];
			char prueba[20];
			strcpy (nombre, lista_servicios.servicios[0].orden);
			int Consulta = JugadorContrincante(nombre, prueba);
			
			if (Consulta == 0)
			{
				sprintf (respuesta,"4/-1/%s\n", prueba);
				printf("%s\n", respuesta);
			}
			else
			{
				strcpy(respuesta,"Error");
			}
			//strcpy(respuesta,"4/Error");
			
			write (sock_conn, respuesta, strlen(respuesta));
			
		}
		if (codigo == 5) // Ranking
		{
			char prueba[500];
			int Consulta = Ranking(prueba);
			if (Consulta >= 0)
			{
				sprintf (respuesta,"5/-1/%s\n", prueba);
				
			}
			else
			{
				strcpy(respuesta,"Error");
			}
			write (sock_conn,respuesta, strlen(respuesta));
			
		}
		if (codigo == 6)	//enviar lista de conectados
		{
			int j;
			j=0;
			strcpy(respuesta, "6/");
			char numero[10];
			sprintf(numero, "%d", lista_conectados.num);
			strcat(respuesta, numero);
			
			while ( j < lista_conectados.num)
			{
				strcat(respuesta, "/");
				strcat (respuesta, lista_conectados.usuarios[j].jugador);
				j = j + 1;
			}
			write (sock_conn,respuesta, strlen(respuesta));
		}
	}
}




int main(int argc, char *argv[])
{
	
	int sock_conn;
	int sock_listen;
	struct sockaddr_in serv_adr;
	pthread_t thread[100];
	int sockets[100];
	
	
	// INICIALITZACIONS
	// Abrimos socket
	/*if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)*/
	if ((sock_listen = socket(AF_INET, SOCK_STREAM,0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicializa a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// escucharemos en el port 
	serv_adr.sin_port = htons(9058);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	int i;
	int j=0;	
//	int e= BBDD	();
	pthread_mutex_init(&mutex, NULL);
	for(;;)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		sockets[j] = sock_conn;
		pthread_create ( &thread[j], NULL, AtenderCliente, &sockets[j]);
		j++;
	}
	for (i=0;i<100;i++){
		pthread_join(thread[i],NULL);
	}
	
	
	
}
