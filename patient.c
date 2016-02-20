#include "headers.h"
#include "patient.h"

char* patientDB = "patientDB";
struct Patient patient[100];
int patientCount;
void savePatient() {
		printf("a1\n");
	int patientCount = getPatientCount();
	int i;
	FILE *fp = fopen(patientDB, "w");
	char intStr[10];
	char buffer[100];
	sprintf(intStr,"%d", patientCount);
	fputs(intStr,fp);
	fputs("\n",fp);
		printf("a2\n");
	for(i=0;i<patientCount;i++){
		fputs(patient[i].name,fp);
		fputs(patient[i].lastName,fp);
		fputs(patient[i].pesel,fp);
		fputs(patient[i].login,fp);
		fputs(patient[i].password,fp);
		fputs(patient[i].doctorName,fp);
		fputs(patient[i].doctorLastName,fp);
		
		strftime(buffer,100,"%c",&patient[i].registrationDate);
		fputs(buffer,fp);
		fputs("\n",fp);
		strftime(buffer,100,"%c",&patient[i].visitDate);
		fputs(buffer,fp);
		fputs("\n",fp);
		
		sprintf(intStr,"%d", patient[i].loginAttempt);
		fputs(intStr,fp);
		fputs("\n",fp);
		sprintf(intStr,"%d", patient[i].confirmation);
		fputs(intStr,fp);
			printf("a3\n");
		fputs("\n",fp);
			printf("a4\n");
	}
	fclose(fp);
}

//imie; nazwisko; pesel; login; haslo; imieLekarza; nazwiskoLekarza; dataRejestracji; data_wizyty; probyLogowania; potwierdzenie
void loadPatient() {
	
	FILE *fp = fopen(patientDB,"r");
	if(fp == NULL){ // jesli nie ma pliku tworzy go i wpisuje ilosc pacjentow=0
		fp = fopen(patientDB,"wr");
		char pc = '0';
		fputs(&pc, fp);
		fputs("\n",fp);
	}
	else{
			char buffer[1000];
			char dateBuff[1000];
			fgets(buffer, sizeof(int),fp);
			patientCount = atoi(buffer);
			int i;
			for(i = 0; i<patientCount;i++){
				fgets(buffer,sizeof(buffer),fp); // wczytaj imie
				strcpy(patient[i].name,buffer);	
				fgets(buffer,sizeof(buffer),fp); // nazwisko
				strcpy(patient[i].lastName,buffer);
				fgets(buffer,sizeof(buffer),fp); // pesel
				strcpy(patient[i].pesel,buffer);
				fgets(buffer,sizeof(buffer),fp); // login
				strcpy(patient[i].login,buffer);
				fgets(buffer,sizeof(buffer),fp); // haslo
				strcpy(patient[i].password,buffer);
				fgets(buffer,sizeof(buffer),fp); // imieLekarza
				strcpy(patient[i].doctorName,buffer);
				fgets(buffer,sizeof(buffer),fp); // nazwiskoLekarza
				strcpy(patient[i].doctorLastName,buffer);
				fgets(buffer,sizeof(buffer),fp); //dataRejestracji
				strptime(buffer,"%c",&patient[i].registrationDate);
				fgets(buffer,sizeof(buffer),fp); //data_wizyty
				strptime(buffer,"%c",&patient[i].visitDate);
				fgets(buffer,sizeof(buffer),fp); //probyLogowania
				patient[i].loginAttempt = atoi(buffer);
				fgets(buffer,sizeof(buffer),fp); //potwierdzenie
				patient[i].confirmation = atoi(buffer);
			}		
	}
	fclose(fp);
}
//imie; nazwisko; pesel; login; haslo; imieLekarza; nazwiskoLekarza; dataRejestracji; data_wizyty; probyLogowania; potwierdzenie
void addPatientX(){
	
			char buffer[1000];
		
			
			
			FILE *fp = fopen(patientDB, "a");
				if(fp == NULL){ // jesli nie ma pliku tworzy go i wpisuje ilosc pacjentow=0
					printf("BLAD PRZY OTWIRANIU PATIENDB\n");
				}
			strcpy(buffer, addPatientMessage.name);
			strcat(buffer,addPatientMessage.lastName);
			strcat(buffer,addPatientMessage.pesel);
			strcat(buffer,addPatientMessage.login);
			strcat(buffer,addPatientMessage.password);
			strcat(buffer, "\n\n"); // puste: imieLekarza, nazwiskoLekarza,
			
			struct tm* tm_info;
			time_t timer;
			time(&timer);
			char timebuff[100];
			tm_info = localtime(&timer);
			strftime(timebuff, 100, "%c", tm_info);
			strcat(buffer,timebuff);
			
			strcat(buffer,"\n");
			strcat(buffer, "0\n0\n0\n"); // puste: dataWizyty, probyLogowania, pitwierdzenie
			fputs(buffer,fp);
			fclose(fp);

			incrementPatientCount();
			loadPatient();
			printPatients();
		
}



void incrementPatientCount() {
	int patientCount = getPatientCount();
	patientCount++;
	FILE *fp = fopen(patientDB,"r+");
	fseek(fp,0,SEEK_SET);
	char intStr[10];
	sprintf(intStr,"%d", patientCount);
	fputs(intStr,fp);
	fclose(fp);
}
int getPatientCount() {
    FILE *fp = fopen(patientDB, "r");
    char *buffer;
    if (fp != NULL) {
        fgets(buffer, sizeof(int), fp);
        patientCount = atof(buffer);
        fclose(fp);
        return patientCount;
    }
    else {
        return 0;
    }
    
}
void printPatients() {
	int i;
	for(i=0;i<patientCount;i++){
		printf("#Pacjent nr: %d\n", (i+1));
		printf("Imie: %s",patient[i].name);
		printf("Nazwisko: %s",patient[i].lastName);
		printf("Pesel: %s",patient[i].pesel);
		printf("Login: %s",patient[i].login);
		printf("Haslo: %s",patient[i].password);
		printf("Imie Lekarza: %s",patient[i].doctorName);
		printf("Nazwisko Lekarza: %s\n",patient[i].doctorLastName);
		printf("data rej: %d\n",patient[i].registrationDate.tm_hour);
		printf("data wiz: %d\n",patient[i].visitDate.tm_mday);
	}
}
