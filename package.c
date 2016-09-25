#include "package.h"
#include <math.h>

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

int package_getSize(package_t self){
	return self.size;
}

int package_addTemperature(package_t *self, int pos,char *temp){
	float ftemp= atof(temp);
	//float rfloat = floor(ftemp);
	self->temperatures[pos] = ftemp;
	self->size++;
	return 0;
}

int package_destroy(package_t *self){
	self->size =0;
	free(self->dt);

	return 0;
}

