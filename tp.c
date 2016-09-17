#include <stddef.h>
#include <string.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dateTime.h"
#include "socket.h"


#ifndef INVALID_PARAMS
#define INVALID_PARAMS 2
#endif

#ifndef CONNECTION_ERROR
#define CONNECTION_ERROR -1
#endif

#ifndef ERROR
#define ERROR -1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef SERVER
#define SERVER "server"
#endif

#ifndef CLIENT
#define CLIENT "client"
#endif

#ifndef MAX_PORT
#define MAX_PORT  45003
#endif

#ifndef MIN_PORT
#define MIN_PORT  1
#endif

#ifndef STEP_MAX
#define STEP_MAX 60000
#endif

#ifndef STEP_MIN
#define STEP_MIN 1
#endif

#ifndef RECIBIENDO
#define RECIBIENDO "Recibiendo termoestato. ID = "
#endif

#ifndef DATOS_RECIBIDOS
#define DATOS_RECIBIDOS  "Datos recibidos: "
#endif

#ifndef TERMOESTATO_DESCONECTADO
#define TERMOESTATO_DESCONECTADO  "Termoestato desconectado. ID = "
#endif

#ifndef TEMP_MIN
#define TEMP_MIN  0
#endif

#ifndef TEMP_MAX
#define TEMP_MAX  767
#endif

#ifndef MAX
#define MAX "Max="
#endif

#ifndef MIN
#define MIN "Min="
#endif

#ifndef MEDIANA
#define MEDIANA "Mediana="
#endif

#ifndef MUESTRAS
#define MUESTRAS "Muestras="
#endif

/*int initializeClient(char *argv[]){
	char *hostname = argv[2];
	char *port = argv[3];
	char *idTermostato = argv[4];
	char *step = argv [5];
	char *startDate = argv[6];
	//char *fileName

	return 0;
}*/

bool isValidParams(int argc, char *argv[], int server){
	int step= 0, port = 20;
	char *ptr;
	FILE *sensorInput;


	if (server == 0){
		if(argc !=3){
			return false;
		}

		//VALIDA EL RANGO DE TCP VALIDO
		port = strtol(argv[2], &ptr, 10);
		if ((port > MAX_PORT) || (port < MIN_PORT)){
			return false;
		}
	}
	else{
		if(argc !=8){
			return false;
		}

		dateTime_t dt;
		int isValid=0;
		const char delimiter[7] = " -.:_/";

		//VALIDA EL RANGO DE PUERTO TCP VALIDO
		port = strtol(argv[3], &ptr, 10);
		if ((port > MAX_PORT) || (port < MIN_PORT)){
			return false;
		}

		//VALIDA EL RANGO DEL STEP
		step = strtol(argv[5], &ptr, 10);

		if (step < STEP_MIN || step > STEP_MAX){
			return false;
		}

		//VALIDA QUE SEA UNA FECHA Y DIA VALIDOS
		isValid = dateTime_createWithString(&dt,argv[6], delimiter);
		if (isValid==INVALID_PARAMS){
			return false;
		}

		//VALIDA QUE EL ARCHIVO EXISTA
		sensorInput = fopen(argv[7],"rb");
		if ( sensorInput == NULL ) {
			fprintf(stderr, "No existe el archivo %s \n", argv[7]);
			return false;
		}
		fclose(sensorInput);
	}

	printf("VALID PARAMS \n");
	return true;
}

int isServerMode(int argc, char *argv[]){
	/**Codigos de retorno: 0 servidor, 1 cliente, 2 error"**/

	printf("argc %d \n", argc);
	printf("mode %s \n", argv[1]);
	if (argc < 3){
		return INVALID_PARAMS;
	}

	if (strcmp(argv[1],SERVER) == 0){
		printf("IS SERVER \n");
		return 0;
	}

	if (strcmp(argv[1],CLIENT) == 0){
			printf("IS CLIENT \n");
			return 1;
	}
	else

	return 2;
}

int getMaxQuantityPerDay(char *step){
	int timeStep=0;
	char *ptr;
	timeStep = strtol(step, &ptr, 10);

	if ((timeStep > STEP_MAX)  || (timeStep < STEP_MIN)){
		return ERROR;
	}
	return ((60000 /  timeStep) *60 *24);
}

int comparefunc (const void *a,const void *b)
{
   return ( *(float*)a - *(float*)b );
}

float getMediana(float temperatures[], int size){
	float med=0;
	if (size <= 0 || temperatures == NULL){
		return 1;
	}

	if (size == 1){
		return temperatures[0];
	}
	if (size % 2 != 0){
		return temperatures[(size/2) -1];
	}

	med = (temperatures[(size/2)] +  temperatures[(size/2)-1])/2;
	return med;
}

int getMinAndMax(float temperatures[],int size, char *line){
	float max, min , mediana;
	char *bufferMax = malloc(50*sizeof(char));
	char *bufferMin = malloc(6*sizeof(char));
	char *bufferMediana = malloc(6*sizeof(char));


	if (size <= 0 || temperatures == NULL){
		return ERROR;
	}

    qsort(temperatures, size, sizeof(float), comparefunc);
	min = temperatures[0];
    mediana = getMediana(temperatures, size);
	max = temperatures[size-1];

	snprintf(bufferMax, 6, "%.02f", max);
	snprintf(bufferMin, 6, "%.02f", min);
	snprintf(bufferMediana, 6, "%.02f", mediana);

	strcat(bufferMax, " ");
	strcat(bufferMax, MIN);
    strcat(bufferMax, bufferMin);
    strcat(bufferMax, " ");
    strcat(bufferMax, MEDIANA);
    strcat(bufferMax, bufferMediana);
    strncpy(line, bufferMax, 50 );

    free(bufferMax);
    free(bufferMin);
    free(bufferMediana);

    return 0;
}

int writeLog(float temperatures[],char *date, char *idTermostato, char *step){
	char *line = malloc(60*sizeof(char));
	char *values = malloc(50*sizeof(char));
	int size = (sizeof(temperatures) / sizeof(float));
	char *cant= malloc(7*sizeof(char));

	int maxQuantity = getMaxQuantityPerDay(step);

	if ( (maxQuantity == ERROR) || (size > maxQuantity)  ){
		return ERROR;
	}

	sprintf(cant, "%d", size);
	getMinAndMax(temperatures, size,values);

	strncpy(line, date, 10);
	strcat(line, " ");
	strcat(line, idTermostato);
	strcat(line, " ");

	//printf("NO CONSTANTES \n");
	strcat(line, MAX);
	strcat(line, values);
	strcat(line, " ");
	strcat(line, MUESTRAS);
	strcat(line, cant);

	//printf("LINEA COMPLETA \n");
	//fputs(line, outPutTemperatures);
	printf(DATOS_RECIBIDOS);
	printf("%d \n", size);

	free(cant);
	free(line);
	free(values);
	return 0;
}

int logTemperatures(socket_t *server,socket_t *accepted_socket,float temperatures[], char *date, char *idTermostato, char *step){
	//DIVIDIR EN DIAS Y ESCRIBIR POR DIAS
	writeLog(temperatures, date, idTermostato, step);
	return 0;
}

int getMaxQuantityTempPerMin(char *step, int seconds){
	int timeStep=0;
	char *ptr;
	timeStep = strtol(step, &ptr, 10);

	if ((timeStep > STEP_MAX)  || (timeStep < STEP_MIN)){
		return ERROR;
	}
	if (seconds< MAX_SECONDS){
		return ((1000 * seconds) / timeStep);

	}
	return (STEP_MAX / timeStep);
}


bool isValidTemp(unsigned short int temp){
	if (temp > TEMP_MAX || temp < TEMP_MIN){
		return false;
	}

	return true;
}

float transformTemp(unsigned short int temp){
	float convertedTemp = (temp -170) * 0.1;
	return convertedTemp;
}

int readAtemperature(FILE *inputTemperatures, unsigned short int *temp,float lastValue, float *convertedTemp){
	int status = fread(temp, 2, 1, inputTemperatures);
	*temp = htons(*temp);
	if (isValidTemp(*temp) == false){
		*temp = lastValue;
	}
	*convertedTemp = transformTemp(*temp);

	return status;
}

int sendTemperatures(socket_t *skt, char *fileSensor,char *startDate,char *step){
	FILE *inputTemperatures= fopen(fileSensor,"rb");

	if ( inputTemperatures == NULL ) {
		fprintf(stderr, "Error al abrir el archivo de temp");
		return INVALID_PARAMS;
	}

	int maxQuantityPerMin = getMaxQuantityTempPerMin(step,MAX_SECONDS);
	int quantityPerDay=0, quantityPerMin=0, status =1;
	float lastValue = 0 ,convertedTemp = 0;
	unsigned short int temp=0;
	char  stringdt[20];
	char *tempToString = malloc((6)*sizeof(char));
	const char delimiter[7] = " -/_.:";

	dateTime_t dt;
	dateTime_createWithString(&dt, startDate, delimiter);
	quantityPerMin = getMaxQuantityTempPerMin(step,dt.seconds);
	char temperatures[(6*maxQuantityPerMin)*(sizeof(char))+20];
	bool firstMinute = true;


	while (status = (readAtemperature(inputTemperatures,&temp, lastValue, &convertedTemp) ) > 0){
		if (quantityPerMin == 0 || firstMinute == true){
			firstMinute = false;
			memset(temperatures, '\0', sizeof(temperatures));
			dateTime_get(&dt, stringdt);
			strcat(temperatures, stringdt);
			strcat(temperatures, " ");
		}

		snprintf(tempToString, 6, "%.1f", convertedTemp);
		strcat(temperatures, tempToString);
		strcat(temperatures, " ");

		quantityPerMin++;
		quantityPerDay++;
		if (quantityPerMin== maxQuantityPerMin){
			printf("%s \n", temperatures);
			strcat(temperatures, "\n");
			int bytes = sizeof(temperatures)+1;
			status = socket_send(skt, temperatures,bytes);
			if (status == 0){
				return CONNECTION_ERROR;
			}

			printf("%s - Enviando %d muestras \n", stringdt, quantityPerMin);
			quantityPerMin= 0;
			dateTime_increaseMinutes(&dt);

		}
		lastValue = temp;
	}

	//socket_send(skt, temperatures, sizeof(temperatures));
	printf("%s \n", temperatures);
	printf("Enviando %d muestras \n", quantityPerMin);
	printf("%d \n", quantityPerDay);

	free(tempToString);

	fclose(inputTemperatures);
	return 0;
}

int main(int argc, char *argv[]) {

	int status = 0;
	int isServer = 0;
	int bytes;


	//INICIALIZACION CLIENTE
	char *hostname = "localhost";
	const char *port ="8080";
	char *idTermostato;

	char *startDate;
	char *fileSensor;

	int skt;
	bool continue_running = true;

	unsigned short len = 0;
	char small_buf[60];
	float temperatures[]= {10.1,10.2, 10.3, -18.0, 70.0, 11.0};
	isServer = isServerMode(argc,argv);

	//PARAMETROS INVALIDOS
	if (isValidParams(argc, argv, isServer) == false){
		printf("INVALID_PARAMS");
		return INVALID_PARAMS;
	}

	//VALIDACION SERVIDOR
	if (isServer == 0){
		socket_t *server = malloc(sizeof(socket_t));
		socket_t *client = malloc(sizeof(client));
		socket_create(server);
		status = socket_bind_and_listen(server, port);

		if (status == 0){
			return CONNECTION_ERROR;
		}

		skt = socket_accept(server, client);

		if (skt < 0){
			return CONNECTION_ERROR;
		}

		//printf(RECIBIENDO);
		//printf("%s \n", idTermostato);

		//LOOP PARA RECEIVE
		bytes = socket_receive(client, idTermostato, sizeof(idTermostato));

		if (bytes < 0){
			return CONNECTION_ERROR;
		}

		while (continue_running) {
			 memset(small_buf, 0, strlen(small_buf));
			 socket_receive(client, small_buf, 60);

			 len = atoi(small_buf);
			 if (len == 0) {
				continue_running = false;
			 }
		}


		//logTemperatures(server, client, temperatures, startDate, idTermostato, step);
		socket_shutdown(server);
		printf(TERMOESTATO_DESCONECTADO);
		printf("%s \n", idTermostato);
		socket_destroy(server);
	}
	else{
		socket_t client;
		socket_create(&client);
		hostname = argv[2];
		port = argv[3];
		idTermostato = argv[4];
		char *step = argv[5];
		startDate = argv[6];
		fileSensor = argv[7];

		status = socket_connect(&client, hostname, port);

		if (status == ERROR){
			return CONNECTION_ERROR;
		}

		status = sendTemperatures(&client, fileSensor, startDate, step);

		if (status != SUCCESS){
			return 	CONNECTION_ERROR;
		}

		//status = socket_send(client, paquete, sizeof(startDate));

		socket_shutdown(&client);
		socket_destroy(&client);
	}

	printf("FINISH \n");
	return SUCCESS;
}
