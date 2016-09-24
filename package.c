#include "package.h"

int package_create(package_t *self,dateTime_t *dt){
	self->dt = dt;
	self->size = 0;
	return 0;
}

float* package_getTemperatures(package_t *self){
	return self->temperatures;
}

dateTime_t* package_getDateTime(package_t self){
	return self.dt;
}
int package_setDateTime(package_t *self, dateTime_t *dt){
	return 0;

}

float* package_getSize(package_t self){
	return self.size;
}
int package_addTemperature(package_t *self, char *temp){
	return 0;
}

int package_destroy(package_t *self){
	return 0;
}

