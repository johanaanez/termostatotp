#include "dateTime.h"

int getMaxDay(int month){
    switch (month){
    case 12:
    	return MAX_DAY;
    case 11:
    	return MAX_DAY_APRIL;
    case 10:
    	return MAX_DAY;
    case 9:
    	return MAX_DAY_APRIL;
    case 8:
    	return MAX_DAY;
    case 7:
    	return MAX_DAY;
    case 6:
    	return MAX_DAY_APRIL;
    case 5:
    	return MAX_DAY;
    case 4:
    	return MAX_DAY_APRIL;
    case 3:
    	return MAX_DAY;
    case 2:
    	return MAX_DAY_FEB;
    case 1:
    	return MAX_DAY;
    default:
    	return MAX_DAY;
    }
}

bool dateTime_isYearValid(dateTime_t *self){
	if(self->year >0){
		return true;
	}
	return false;
}

bool dateTime_isMonthValid(dateTime_t *self){
	if (self->month>0 && self->month <= MAX_MONTH){
		return true;
	}
	return false;
}

bool dateTime_isDayValid(dateTime_t *self){
	int maxDays = getMaxDay(self->month);
	if (self->day > 0 && self->day <= maxDays){
		return true;
	}
	return false;
}

bool dateTime_isHourValid(dateTime_t *self){
	if (self->hour >= 0 && self->hour <= MAX_HOUR){
		return true;
	}
	return false;
}

bool dateTime_isMinutesValid(dateTime_t *self){
	if (self->minutes >= 0 && self->minutes <= MAX_MINUTES){
		return true;
	}
	return false;
}

bool dateTime_isSecondsValid(dateTime_t *self){
	if (self->seconds >= 0 && self->seconds <= MAX_SECONDS){
		return true;
	}
	return false;
}

bool dateTime_isValid(dateTime_t *self){
	if (dateTime_isYearValid(self) && dateTime_isMonthValid(self) && dateTime_isDayValid(self) &&
			dateTime_isHourValid(self) && dateTime_isMinutesValid(self) && dateTime_isSecondsValid(self)){
		return true;
	}
	return false;
}

int dateTime_create(dateTime_t *self, int year,int month,int day, int hour, int minutes, int seconds, int milliseconds){
	self->year = year;
	self->month = month;
	self->day = day;
	self->hour = hour;
	self->minutes = minutes;
	self->seconds = seconds;
	self->year = milliseconds;

	return 0;
}

int dateTime_createWithString(dateTime_t *self, char string[], const char delimiterDate[]){
	char *token;
	char *ptr;
	int value=0;
	char stringToParser[20];

	strncpy(stringToParser, string, 20);

	token = strtok(stringToParser, delimiterDate);
	value = strtol(token, &ptr, 10);
	self->year = value;
	if (dateTime_isYearValid(self) == false){
		return INVALID_PARAMS;
	}

	token = strtok(NULL, delimiterDate);
	value = strtol(token, &ptr, 10);
	self->month = value;
	if (dateTime_isMonthValid(self) == false){
		return INVALID_PARAMS;
	}


	token = strtok(NULL, delimiterDate);
	value = strtol(token, &ptr, 10);
	self->day = value;
	if (dateTime_isDayValid(self) == false){
		return INVALID_PARAMS;
	}

	token = strtok(NULL, delimiterDate);
	value = strtol(token, &ptr, 10);
	self->hour = value;
	if (dateTime_isHourValid(self) == false){
		return INVALID_PARAMS;
	}

	token = strtok(NULL, delimiterDate);
	value = strtol(token, &ptr, 10);
	self->minutes = value;
	if (dateTime_isMinutesValid(self) == false){
		return INVALID_PARAMS;
	}

	token = strtok(NULL, delimiterDate);
	value = strtol(token, &ptr, 10);
	self->seconds = value;
	if (dateTime_isSecondsValid(self) == false){
		return INVALID_PARAMS;
	}

	return 0;
}

int dateTime_destroy(dateTime_t *self){
	return 0;
}


int dateTime_increaseYear(dateTime_t *self){
	self->year ++;
	return 0;
}

int dateTime_increaseMonth(dateTime_t *self){
	self->month ++;

	if (self->month >12){
		self->month =1;
		dateTime_increaseYear(self);
	}
	return 0;
}



int dateTime_increaseDay(dateTime_t *self){
	int maxDays = getMaxDay(self->month);
	self->day ++;

	if (self->day >maxDays){
		self->day =1;
		dateTime_increaseMonth(self);
	}

	return 0;
}

int dateTime_increaseHours(dateTime_t *self){
	self->hour++;

	if (self->hour >23){
		self->hour =0;
		dateTime_increaseDay(self);
	}

	return 0;
}

int dateTime_increaseMinutes(dateTime_t *self){
	if(dateTime_isValid(self)){
		self->minutes++;
		if (self->minutes > MAX_MINUTES){
				self->minutes =0;
				dateTime_increaseHours(self);
		}

	}
	return INVALID_PARAMS;
}

int dateTime_increaseSeconds(dateTime_t *self){
	if(dateTime_isValid(self)){
		self->seconds++;
			if (self->seconds >59){
				self->seconds =0;
				dateTime_increaseMinutes(self);
		}
		return 0;
	}

	return INVALID_PARAMS;
}

int dateTime_increaseMilliseconds(dateTime_t *self){
	if(dateTime_isValid(self)){
		self->milliseconds ++;

		if (self->milliseconds >MAX_MILLISECONDS){
			self->milliseconds =0;
			dateTime_increaseSeconds(self);
		}
		return 0;
	}
	return INVALID_PARAMS;
}

int fillInt(int number, char *filledString){
	char *numberToString= malloc(2*sizeof(char));
	char *zero= "0";

	if (number < 10){
		snprintf(numberToString, 2, "%d", number);
		strncpy(filledString, zero, 2);
		strcat(filledString, numberToString);
	}
	else{
		snprintf(filledString, 3, "%d", number);
	}

	free(numberToString);
	return 0;
}

char* dateTime_get(dateTime_t *self, char *dt){
	if(dateTime_isValid(self)==true){
		char *year = malloc(5*sizeof(char));
		char *month= malloc(3*sizeof(char));
		char *day= malloc(3*sizeof(char));
		char *hour= malloc(3*sizeof(char));
		char *minutes = malloc(3*sizeof(char));
		char *seconds="00";

		//Transformo los elementos de int a char*
		snprintf(year, 5, "%d", self->year);
		fillInt(self->month, month);
		fillInt(self->day, day);
		fillInt(self->hour, hour);
		fillInt(self->minutes, minutes);

		//Concateno los elementos
		strncpy(dt, year, 5);
		strcat(dt, ".");
		strcat(dt, month);
		strcat(dt, ".");
		strcat(dt, day);
		strcat(dt, "-");
		strcat(dt, hour);
		strcat(dt, ":");
		strcat(dt, minutes);
		strcat(dt, ":");
		strcat(dt, seconds);

		free(year);
		free(month);
		free(day);
		free(hour);
		free(minutes);

		return dt;
	}
	return "";

}

bool dateTime_isNewDay(dateTime_t *self){
	if(self->hour == 0 && self->minutes== 0 && self->seconds==0){
		return true;
	}
	return false;
}

bool dateTime_isLastMinuteOfDay(dateTime_t *self){
	if(self->hour == MAX_HOUR && self->minutes== MAX_MINUTES){
		return true;
	}
	return false;
}

bool dateTime_isNewYear(dateTime_t *self){
	if(dateTime_isNewDay(self) && self->month == 1 && self->day== 1){
			return true;
		}
	return false;
}

int dateTime_getMinutesToNextDay(dateTime_t *self){
	if (dateTime_isNewDay(self)){
		return 0;
	}
	return (((MAX_MINUTES+1)*(MAX_HOUR+1)) - self->minutes);
}

bool dateTime_equals(dateTime_t self, dateTime_t other){
	/* Only compare date, not time*/

	if(self.year == other.year && self.month == other.month && self.day == other.day ){
		return true;
	}
	return false;
}

int dateTime_compare(dateTime_t self, dateTime_t other){
	/* returns 1  if self.date  > other,date
	 * returns 0  if self.date == other.date
	 * returns -1 if self.date  < other.date  */

	if(self.year >= other.year && self.month >= other.month > self.day > other.day ){
		return 1;
	}

	if (dateTime_equals(self, other)){
		return 0;
	}

	return -1;
}
