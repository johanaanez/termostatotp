#include "list.h"

int list_create(list_t *self){
	self->size =0;
	return 0;
}
int list_destroy(list_t *self){
	memset(self->packages, 0, self->size);
	self->size =0;
	return 0;
}

package_t getLastPackage(list_t self){
	int size = self.size;
	return self.packages[size-1];
}

//PRIMER TEMPERATURA SE GUARDA MAL! que paquete se le esta enviando?
//Se le envia siempre la misma dir de mem del paquete o de la fecha?
//SIEMPRE LE CAMBIA LA FECHA AL PAQUETE ANTERIOR!Entonces en el 2do pack no crea un nuevo paquete
//intenta entrar a un paquete anterior y da erro!
int list_addPackage(list_t *self, package_t package){
	dateTime_t *dt = package_getDateTime(package);
	dateTime_t newDate = *dt;
	dateTime_t *lastDt;
	package_t lastPackage;

	float *temperatures = malloc((package_getSize(package))*sizeof(float));
	temperatures = package_getTemperatures(&package);

	if(self->size>0){
		lastPackage = getLastPackage(*self);
		lastDt = package_getDateTime(lastPackage);
	}

	int size = package_getSize(package);
	int i;

	/* if already exists a package in list with package's date*/

	/* If do not exist, i add package to self->list*/
	if(self->size == 0 || dateTime_compare(*dt, *lastDt) > 0 ){
		package_t newPackage;
		package_create(&newPackage,&newDate);

		for(i=0; i< size; i++){
			package_addTemperature(&newPackage, i, temperatures[i]);
		}

		self->packages[self->size] = newPackage;
		self->size++;
	}
	else{
		int initialSize = package_getSize(lastPackage);
		for(i=0; i< size; i++){
			package_addTemperature(&lastPackage, initialSize+i, temperatures[i]);
		}

		char d[20];
		/*printf("%s", dateTime_get(package.dt, d));
		for(i=0; i< lastPackage.size; i++){
			printf("%f,", lastPackage.temperatures[i]);
		}*/

	}


	return 0;
}

package_t* list_getPackages(list_t *self){
	return self->packages;
}

int list_getPackagesPerDay(list_t self, dateTime_t dt, package_t *package){
	int i;
	for(i=0; i<self.size; i++){
		if(dateTime_equals( *package_getDateTime(self.packages[i]) , dt )){
			package= &self.packages[i];
		}
	}

	return 0;
}

int list_getSize(list_t self){
	return self.size;
}
