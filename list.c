#include "list.h"

int list_create(list_t *self){
	self->size =0;
	return 0;
}
int list_destroy(list_t *self){
	return 0;
}
int list_addPackage(list_t *self, package_t package){
	return 0;
}
package_t* list_getPackages(list_t self){
	return self.packages;
}

