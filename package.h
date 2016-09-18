/*
 * serverData.h
 *
 *  Created on: 05/09/2016
 *      Author: joha
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dateTime.h"


typedef struct {
	float *temperatures;
	int size;
	dateTime_t *dt;
	char *idTermostato;
	int maxQuantityPerMin;
} package_t;

int package_create(package_t *self,char *string);
int package_destroy(package_t *self);
float* package_getTemperatures(package_t *self);
dateTime_t* package_getDateTime(package_t *self);
float* package_getTemperatures(package_t *self);

#endif /* PACKAGE_H_ */
