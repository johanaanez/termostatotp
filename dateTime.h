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

#ifndef MAX_MINUTES_PER_DAY
#define MAX_MINUTES_PER_DAY 1440
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


int dateTime_create(dateTime_t *self, int year,int month,int day, int hour, int minutes, int seconds, int milliseconds);
int dateTime_createWithString(dateTime_t *self, char string[], const char delimiter[]);
int dateTime_destroy(dateTime_t *self);
bool dateTime_isValid(dateTime_t *self);
int dateTime_increaseDay(dateTime_t *self);
int dateTime_increaseHours(dateTime_t *self);
int dateTime_increaseMinutes(dateTime_t *self);
int dateTime_increaseSeconds(dateTime_t *self);
int increaseMilliseconds(dateTime_t *self);
bool dateTime_isNewDay(dateTime_t *self);
bool dateTime_isNewYear(dateTime_t *self);
int dateTime_getMinutesToNextDay(dateTime_t *self);
char* dateTime_get(dateTime_t *self, char *dt);
bool dateTime_isLastMinuteOfDay(dateTime_t *self);
