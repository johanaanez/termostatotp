#ifndef MAX_MONTH
#define MAX_MONTH 12
#endif

#ifndef MAX_DAY
#define MAX_DAY 31
#endif

#ifndef MAX_DAY_FEB
#define MAX_DAY_FEB 28
#endif

#ifndef MAX_DAY_APRIL
#define MAX_DAY_APRIL 30
#endif

#ifndef MAX_HOUR
#define MAX_HOUR 23
#endif

#ifndef MAX_MINUTES
#define MAX_MINUTES 59
#endif

#ifndef MAX_SECONDS
#define MAX_SECONDS 59
#endif

#ifndef MAX_MILLISECONDS
#define MAX_MILLISECONDS 999
#endif

#ifndef INVALID_PARAMS
#define INVALID_PARAMS 2
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minutes;
	int seconds;
	int milliseconds;
} dateTime_t;

enum dayOfMonth
  {
	january =1,
	february=2,
	march,
	april,
	may,
	june,
	july,
	august,
	september,
	october,
	november,
	december
  };


int dateTime_create(dateTime_t *self, int year,int month,int day, int hour, int minutes, int seconds, int milliseconds);
int dateTime_createWithString(dateTime_t *self, char string[], const char delimiter[]);
int dateTime_destroy(dateTime_t *self);
bool dateTime_isValid(dateTime_t *self);
int dateTime_increaseDay(dateTime_t *self);
int dateTime_increaseHours(dateTime_t *self);
int dateTime_increaseMinutes(dateTime_t *self);
int dateTime_increaseSeconds(dateTime_t *self);
int dateTime_increaseMilliseconds(dateTime_t *self);
char* dateTime_get(dateTime_t *self, char *dt);


