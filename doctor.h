#ifndef DOCTOR_H
#define DOCTOR_H
#include "headers.h"
typedef struct Doctor{
	char name[30];
	char lastName[30];
	struct tm vacationDate;
	int vacationDuration;
	struct tm visits[20];
	struct tm visitDay;
}Doctor;
extern struct Doctor doctor[10];
char buffer[1000];

void loadVisits();
void incrementDoctorCount();
int getDoctorCount();
void loadDoctors();
void saveDoctors();
void addDoctor();
void printDoctors();
#endif
