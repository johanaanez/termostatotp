#include <stddef.h>
#include <string.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "socket.h"
#include "list.h"

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

#ifndef ENVIANDO
#define ENVIANDO  "Enviando "
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


bool isValidParams(int argc, char *argv[], int server){

	if (server == 0){
		if(argc !=3){
			return false;
		}

	}
	else{
		if(argc !=8){
			return false;
		}

	}
	return true;
}

int isServerMode(int argc, char *argv[]){
	/**Codigos de retorno: 0 servidor, 1 cliente, 2 error"**/

	if (argc < 3){
		return INVALID_PARAMS;
	}

	if (strcmp(argv[1],SERVER) == 0){
		return 0;
	}

	if (strcmp(argv[1],CLIENT) == 0){
			return 1;
	}

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

int comparefunc (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
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
		return temperatures[(size/2)];
	}

	med = (temperatures[(size/2)] +  temperatures[(size/2)-1])/2;
	return med;
}

int getMinAndMax(float temperatures[],int size, char *line){
	float max, min , mediana;

	if (size <= 0 || temperatures == NULL){
		return ERROR;
	}

	char *bufferMax = malloc(50*sizeof(char));
	char *bufferMin = malloc(6*sizeof(char));
	char *bufferMediana = malloc(6*sizeof(char));

    qsort(temperatures, size, sizeof(float), comparefunc);
	min = temperatures[size-1];
    mediana = getMediana(temperatures, size);
	max = temperatures[0];

	int i;
	for(i=0;i<size; i++){
		fprintf(stderr, "%0.1f  ", *(temperatures+i));
	}
	fprintf(stderr, "\n");

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


int writeLogPerDay(float temperatures[],int size,char *date, char *idTermostato){
	char *line = malloc(60*sizeof(char));
	char *values = malloc(50*sizeof(char));
	char *cant= malloc(7*sizeof(char));
	sprintf(cant, "%d", size);
	getMinAndMax(temperatures, size,values);

	strncpy(line, date, 10);
	strcat(line, " ");
	strcat(line, idTermostato);
	strcat(line, " ");


	strcat(line, MAX);
	strcat(line, values);
	strcat(line, " ");
	strcat(line, MUESTRAS);
	strcat(line, cant);

	fprintf(stdout,"%s\n", line);

	free(cant);
	free(line);
	free(values);
	return 0;
}

int writeLog(list_t *list, dateTime_t dates[],int days, char *idTermo){
	int i;

	for(i=0; i<days; i++){
		char date[20];
		dateTime_get(&dates[i], date);
		package_t package;
		list_getPackagesPerDay(*list, dates[i], &package);
		int size = package_getSize(package);
		float *temperatures = malloc(size*sizeof(float));
		temperatures = package_getTemperatures(&package);
		writeLogPerDay(temperatures, size, date, idTermo);
	}

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
		return INVALID_PARAMS;
	}

	int maxQuantityPerMin = getMaxQuantityTempPerMin(step,MAX_SECONDS);
	int quantityPerDay, quantityPerMin=1, status =1;
	float lastValue = 0 ,convertedTemp = 0;
	unsigned short int temp=0;
	char  stringdt[20];
	char tempToString[6];
	memset(tempToString, 0, 6);

	const char delimiter[7] = " -/_.:";
	char space=' ';

	dateTime_t dt;
	dateTime_createWithString(&dt, startDate, delimiter);
	quantityPerMin = getMaxQuantityTempPerMin(step,dt.seconds);

	bool firstMinute = true;
	int firstvalue= (maxQuantityPerMin - quantityPerMin);
	status= readAtemperature(inputTemperatures,&temp, lastValue, &convertedTemp);
	quantityPerDay = 1;
	quantityPerMin++;

	while (status>0 ){
		if (quantityPerMin == 1 || firstMinute){
			dateTime_get(&dt, stringdt);
			status = socket_send(skt, stringdt,strlen(stringdt)+1);
			if (status == 0){
				fclose(inputTemperatures);
				return CONNECTION_ERROR;
			}
			//printf("%s", stringdt);
			firstMinute = false;
		}

		snprintf(tempToString, 7, "%c%.1f", space, convertedTemp);
		if (quantityPerMin< maxQuantityPerMin){
			//printf("%s", tempToString);
		}

		if (quantityPerMin== maxQuantityPerMin){
			char lastTempToString[6];
			memset(lastTempToString, 0, 6);
			strncpy(lastTempToString,tempToString,6);
			snprintf(tempToString, 8, "%s%c", lastTempToString, '\n');
			//printf("%s", tempToString);

			if (firstvalue< maxQuantityPerMin){
				quantityPerMin = firstvalue;
				firstvalue = maxQuantityPerMin;
			}

			fprintf(stderr,"%s - Enviando %d muestras\n", stringdt, quantityPerMin);

			quantityPerMin= 0;
			dateTime_increaseMinutes(&dt);
		}

		status = socket_send(skt, tempToString,strlen(tempToString)+1);
		if (status == 0){
			fclose(inputTemperatures);
			return CONNECTION_ERROR;
		}

		lastValue = temp;
		status= readAtemperature(inputTemperatures,&temp, lastValue, &convertedTemp);
		quantityPerMin++;
		quantityPerDay++;

	}

	fprintf(stderr,"%s - Enviando %d muestras\n", stringdt, quantityPerMin-1);

	fclose(inputTemperatures);
	return 0;
}

int saveTemperatures(package_t *package,int pos, char *string){
	float ftemp= atof(string);
	package_addTemperature(package, pos,ftemp);
	return 0;
}


int main(int argc, char *argv[]) {

	int status = 0;
	int isServer = 0;
	int bytes;

	//INICIALIZACION CLIENTE
	char *hostname = "localhost";
	char *port = "8080";
	char *idTermostato= malloc(7*sizeof(char));

	char *startDate;
	char *fileSensor;

	int skt;
	bool continue_running = true;

	isServer = isServerMode(argc,argv);

	//PARAMETROS INVALIDOS
	if (isValidParams(argc, argv, isServer) == false){
		return INVALID_PARAMS;
	}

	socket_t server;
	socket_t client;

	if (isServer == 0){ //MODO SERVER
		socket_create(&server);
		socket_create(&client);
		port = argv[2];
		status = socket_bind_and_listen(&server, port);

		if (status == 0){
			return CONNECTION_ERROR;
		}

		skt = socket_accept(&server, &client);

		if (skt < 0){
			return CONNECTION_ERROR;
		}

		bytes = socket_receive(&client, idTermostato,7);

		fprintf(stderr, RECIBIENDO);
		fprintf(stderr,"%s", idTermostato);

		if (bytes < 0){
			return CONNECTION_ERROR;
		}


		char buffer[21];
		char date[21];
		memset(buffer, 0, 21);
		int days= 0,minutes = 0, quantityPerDay = 0, quantityPerMin =1 ;
		bool isNewMinute=false;
		const char delimiter[7] = " -/_.:";
		dateTime_t dt;
		list_t list;
		list_create(&list);
		package_t package;
		dateTime_t dates[3];


		while (continue_running) {
			bytes = socket_receiveTemp(&client, buffer, 20, &isNewMinute);
			if (bytes <= 0){
				continue_running = false;
			}
			if(isNewMinute){ //LEYO LA ULTIMA TEMPERATURA
				isNewMinute = false;
				saveTemperatures(&package, quantityPerMin-1, buffer);
				fprintf(stderr,"%s- ", date);
				fprintf(stderr,DATOS_RECIBIDOS);
				fprintf(stderr,"%d\n", quantityPerMin);

				//list_addPackage(&list, package);
				if (dateTime_isLastMinuteOfDay(&dt)){
					dates[days] = dt;
					days++;
				}
				quantityPerMin = 0;
				if(dateTime_isNewDay(&dt)){
					days++;
				}

			}
			if(strlen(buffer)>19){ //LEE UNA FECHA y CREA EL PAQUETE
				strncpy(date, buffer, strlen(buffer)+1);
				dateTime_createWithString(&dt,date, delimiter);
				package_create(&package, &dt);
				minutes++;
			}

			else{
				quantityPerMin++;
				quantityPerDay++;
				saveTemperatures(&package, quantityPerMin-2, buffer);
			}

		}



		fprintf(stderr,"%s- ", date);
		fprintf(stderr,DATOS_RECIBIDOS);
		fprintf(stderr,"%d\n", quantityPerMin-1);
		fprintf(stdout,"Dia:%d\n", days);
		fprintf(stdout,"Minutos en el dia: %d\n", minutes);

		socket_shutdown(&server);
		socket_shutdown(&client);
		fprintf(stderr, TERMOESTATO_DESCONECTADO);
		fprintf(stderr, "%s \n", idTermostato);

		//writeLog(&list,dates, days, idTermostato);
		package_destroy(&package);
		list_destroy(&list);
		socket_destroy(&client);
		socket_destroy(&server);

	}
	else{ //MODO CLIENTE
		socket_create(&client);
		hostname = argv[2];
		port = argv[3];
		idTermostato = argv[4];
		char *step = argv[5];
		startDate = argv[6];
		fileSensor = argv[7];
		char idTermoToSend[10];
		memset(idTermoToSend, 0, 10);
		char enter= '\n';

		status = socket_connect(&server, hostname, port);

		if (status == ERROR){
			return CONNECTION_ERROR;
		}

		snprintf(idTermoToSend, 10, "%s%c", idTermostato, enter);
		status = socket_send(&server, idTermoToSend, strlen(idTermoToSend)+1);

		status = sendTemperatures(&server, fileSensor, startDate, step);

		if (status != SUCCESS){
			return 	CONNECTION_ERROR;
		}

		socket_shutdown(&client);
		socket_destroy(&client);
	}

	return SUCCESS;
}
