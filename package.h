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


typedef struct {
	float *temperatures;
	int size;
	char *hora;
	char *idTermostato;
	int maxQuantity;
} package_t;

int package_create(package_t *self);
int package_destroy(package_t *self);

#endif /* PACKAGE_H_ */
