#pragma once

typedef struct AddPatientMessage
{
	long mtype;
	int type;
    char name[30];
    char lastName[30];
    char pesel[13];
    char login[15];
    char password[15];
}AddPatientMessage;
AddPatientMessage addPatientMessage;

typedef struct LoginMessage
{
	long mtype;
	int type;
    char login[15];
    char password[15];
    int ok;
}LoginMessage;
LoginMessage loginMessage;

typedef struct InformationMessage
{
	long mtype;
	int type;
    int day;
    int month;
    int year;
    int term;
    char login[100];
    char doctorLastName[100];
    char information[1000];

}InformationMessage;
InformationMessage informationMessage;

int msgid;
