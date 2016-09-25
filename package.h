#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dateTime.h"


typedef struct {
	float temperatures[15];
	int size;
	dateTime_t *dt;
	int maxQuantityPerMin;
} package_t;

int package_create(package_t *self,dateTime_t *dt);
int package_destroy(package_t *self);
float* package_getTemperatures(package_t *self);
dateTime_t* package_getDateTime(package_t self);
int package_setDateTime(package_t *self, dateTime_t *dt);
int package_getSize(package_t self);
int package_addTemperature(package_t *self, int pos,char *temp);

#endif /* PACKAGE_H_ */
