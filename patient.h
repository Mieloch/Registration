#ifndef PATIENT_H
#define PATIENT_H
#include "headers.h"
typedef struct Patient{
    char name[30];
    char lastName[30];
    char pesel[13];
    char login[15];
    char password[15];
    char doctorName[30];
    char doctorLastName[30];
    struct tm registrationDate;
    struct tm visitDate;
    int loginAttempt;
    int confirmation;
}Patient;
extern struct Patient patient[100];
extern int patientCount;
void addPatientX();
int getPatientCount();
void savePatient();
void loadPatient();

void incrementPatientCount();
void printPatients();
#endif
