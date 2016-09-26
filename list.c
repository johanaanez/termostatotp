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

int list_addPackage(list_t *self, package_t package){
	float *temperatures = malloc((package_getSize(package))*sizeof(float));
	temperatures = package_getTemperatures(&package);

	dateTime_t *dt = package_getDateTime(package);
	dateTime_t *lastDt = package_getDateTime(self->packages[self->size-1]);
	int size = package_getSize(package);
	int i;

	/* if already exists a package in list with package's date*/
	if(dateTime_compare(*dt, *lastDt) > 0 ){
		package_setDateTime(&self->packages[self->size], dt);

		for(i=0; i< size; i++){
			package_addTemperature(&self->packages[self->size], i, temperatures[i]);
		}
		self->size++;
	}
	else{
		int initialSize = package_getSize(self->packages[self->size-1]);
		for(i=0; i< size; i++){
			package_addTemperature(&self->packages[self->size-1], initialSize+i, temperatures[i]);
		}

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
