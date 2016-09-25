#ifndef LISTA_H_
#define LISTA_H_

#include "package.h"

typedef struct{
	package_t *packages;
	int size;
}list_t;

int list_create(list_t *self);
int list_destroy(list_t *self);
int list_addPackage(list_t *self, package_t package);
package_t * list_getPackages(list_t self);

#endif /* LISTA_H_ */
;
