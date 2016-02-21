#include <stdio.h>
#include <stdlib.h>
#include "headers.h"
#include "patient.h"
#include "doctor.h"
void loginUser();
void sendDoctorsInDate();
void addVisit();
int getDayOfWeek(int d, int m, int y);
int checkFreeTerm(char* doctorLastName, int day, int month, int year, int term);
void sendVisitInfo();
void giveVacation();
void cancelVisit();
int checkVacation(int d, int m, int y, char* doctorLastName);
char buffer[1000];
AddPatientMessage addPatientMessage;
LoginMessage loginMessage;
InformationMessage informationMessage;
int patientCount;
int doctorCount;
int visitTerms[3] = {9,10,11};
DoctorMessage doctorMessage;

//extern Patient patient[100];


int main(int argc, char** argv){
	
	msgid = msgget(997,IPC_CREAT | 0666);
	if(msgid == -1) perror("Tworzenie kolejki komunikatow");
	
	patientCount = getPatientCount();
	loadPatient(); 
	loadDoctors();
	doctorCount = getDoctorCount();
	printDoctors();
	printPatients();
			printf("begin\n");

	while(1){
		if(msgrcv(msgid,&addPatientMessage,sizeof(addPatientMessage),1,IPC_NOWAIT) != -1){
			addPatientX();
		}
		else if(msgrcv(msgid,&loginMessage,sizeof(loginMessage),2,IPC_NOWAIT) != -1){
			loginUser();
		}
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),3,IPC_NOWAIT) != -1){
			sendDoctorsInDate();
		}
	
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),6,IPC_NOWAIT) != -1){
			sendVisitInfo();
		}
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),5,IPC_NOWAIT) != -1){
			cancelVisit();
		}
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),4,IPC_NOWAIT) != -1){
			addVisit();
		}
		else if(msgrcv(msgid,&doctorMessage,sizeof(doctorMessage),10,IPC_NOWAIT) != -1){
			giveVacation();
		}



		//printPatients();
		sleep(1);
	}
	return 0;
}
int getDayOfWeek(int d, int m, int y){
	return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
}
int checkVacation(int d, int m, int y, char* doctorLastName){
	int i;
		for(i=0;i<doctorCount;i++){
			if(strcmp(doctor[i].lastName ,informationMessage.doctorLastName) == 0 &&
			doctor[i].vacationDate.tm_mday == d &&
			doctor[i].vacationDate.tm_mon == m &&
			doctor[i].vacationDate.tm_year == y
			){
					
					return 0; // ma urlop tego dnia
			}	
	}
	return 1; // nie ma urlopu
}
void giveVacation(){
	int i,k;
	int patientCount = getPatientCount();
	loadPatient();
	for(i=0;i<doctorCount;i++){
		if(strcmp(doctor[i].lastName,doctorMessage.doctorLastName)==0){
			printf("jest lekarz\n");
			doctor[i].vacationDate.tm_mday = doctorMessage.day;
			doctor[i].vacationDate.tm_mon = doctorMessage.month - 1;
			doctor[i].vacationDate.tm_year = doctorMessage.year - 1900;
			
			for(k=0;k<patientCount;k++){
				int visitDay = patient[k].visitDate.tm_mday;
				int visitMonth = patient[k].visitDate.tm_mon;
				int visitYear =patient[k].visitDate.tm_year;
				if(strcmp(patient[k].doctorLastName,doctor[i].lastName) == 0 && visitDay == doctorMessage.day && doctorMessage.month == visitMonth && doctorMessage.year == visitYear){
					printf("kasujemy wizyte\n");
					strcpy(patient[k].doctorLastName, "\n");
					strcpy(patient[k].doctorName, "\n");
					patient[k].visitDate.tm_year = 0;
				}
			}
			strcpy(doctorMessage.information,"Udzielono urlopu");
			doctorMessage.mtype = doctorMessage.type;
			msgsnd(msgid,&doctorMessage,sizeof(doctorMessage),0);
			savePatient();
			saveDoctors();
			break;
		}
	}
	
}
void sendVisitInfo(){
	printf("start sendVisitInfo\n");
		loadPatient();
		char buffer[100];
		char result[1000];
		strcpy(result, "");
		int i,k;
		int patientCount = getPatientCount();
		for(i=0;i<patientCount;i++){
			if(strcmp(patient[i].login, informationMessage.login)==0 && patient[i].visitDate.tm_year != 0){
				strcat(result, patient[i].doctorLastName);
				strcat(result, patient[i].doctorName);
				strftime(buffer,100,"%c",&patient[i].visitDate);
				strcat(result, buffer);
			}
		}
		strcpy(informationMessage.information,result);
		informationMessage.mtype = informationMessage.type;
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
			printf("end sendVisitInfo\n");

}
int checkFreeTerm(char* doctorLastName, int day, int month, int year, int term){


	int i,k;
	for(i=0;i<doctorCount;i++){
			if(strcmp(doctor[i].lastName ,informationMessage.doctorLastName) == 0){
					for(k=0;k<20;k++){
						if(doctor[i].visits[k].tm_mday == day &&
						doctor[i].visits[k].tm_mon == month &&
						doctor[i].visits[k].tm_year == year &&
						doctor[i].visits[k].tm_hour == term){
							return 0; // termin juz jest wybrany
						}
					}
					return 1; // nie znaleziono terminu czyli wolne
			}	
	}
	return 0; // nie znaleziono lekarza wiec jest zle

}
void cancelVisit(){
	printf("start cancelVisit\n");
		char buffer[100];
		int i,k;
		int day = informationMessage.day;
		int month = informationMessage.month -1;
		int year = informationMessage.year -1900;
		int term = informationMessage.term;
		int patientCount = getPatientCount();
		strcpy(informationMessage.information, "Cos poszlo nie tak\n");
		for(i=0;i<patientCount;i++){
			if(strcmp(patient[i].login, informationMessage.login)==0){
				strcpy(patient[i].doctorLastName, "\n");
				strcpy(patient[i].doctorName, "\n");

				patient[i].visitDate.tm_year = 0;
				strcpy(informationMessage.information, "Odwolano\n");
				break;
			}
		}
		savePatient();
		loadVisits();
		printf("TYPE %d\n",informationMessage.type);
		//printf("mtype %d\n",informationMessage.mtype);
		informationMessage.mtype = informationMessage.type;
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
			printf("koniec cancelVisit\n");

}
void addVisit(){
		printf("start addVisit\n");

		loadVisits(); // aktualizacja wizyt
		char buffer[100];
		int i,k;
		int day = informationMessage.day;
		int month = informationMessage.month -1;
		int year = informationMessage.year -1900;
		int term = informationMessage.term;
		if(term < 9 || term > 11){
			strcpy(informationMessage.information, "Podales zla godzine\n");
			informationMessage.mtype = informationMessage.type;
			msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
			return;
		}
		//sprawdzanie terminu
		if(checkFreeTerm(informationMessage.doctorLastName,day,month,year,term) == 1 ){

			//dopisywanei wizyty pacjentowi
			for(i=0;i<doctorCount;i++){
				if(strcmp(doctor[i].lastName, informationMessage.doctorLastName) == 0){ // znajdz lekarza o wybranym nazwisku
					for(k=0;k<patientCount;k++){
						if(strcmp(patient[k].login,informationMessage.login)==0){ // znajdz pacjenta o danym loginie
							strcpy(patient[k].doctorName,doctor[i].name);
							strcpy(patient[k].doctorLastName,doctor[i].lastName);
							// TODO terminy
							patient[k].visitDate.tm_mday = day;
							patient[k].visitDate.tm_mon = month;
							patient[k].visitDate.tm_year = year;
							patient[k].visitDate.tm_hour = term;
							
						}
					}
				}
			}
			strcpy(informationMessage.information, "Zarejestrowano wizyte\n");
			printPatients(); // zapisanie wizyty w pliku
			savePatient(); // zapisanie wizyty w pliku
			loadVisits(); // aktualizacja wizyt
			
		}
		else if(checkVacation(day,month,year, informationMessage.doctorLastName) == 1){
			strcpy(informationMessage.information, "Lekarz ma urlop tego dnia!\n\n");

		}
		else{ // nie ma wolnego
			strcpy(informationMessage.information, "Podany termin jest zajety!\n\n");
		}
		
		informationMessage.mtype = informationMessage.type;
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
		printf("koniec addVisit\n");
}

void sendDoctorsInDate(){
	printf("start doctorsInDay\n");
		int dayOfWeek = getDayOfWeek(informationMessage.day,informationMessage.month,informationMessage.year);
		int isTaken = 0;
		char buffer[100];
		char result[1000];
		strcpy(result,"");
		int i,k,j;
			for(j=0;j<3;j++){
				
				strcat(result,"\nGodzina: ");
				char intStr[15];
				sprintf(intStr,"%d", visitTerms[j]);
				strcat(result,intStr);
				strcat(result,":00\n");		
				for(i=0;i<doctorCount;i++){
						strftime(buffer,100,"%w",&doctor[i].visitDay);
						int doctorVisitDay = atoi(buffer);
						if(doctorVisitDay != dayOfWeek){
							continue;
						}
		
		
						for(k=0;k<20;k++){	
							isTaken = 0;
							if(doctor[i].visits[k].tm_hour == visitTerms[j]){
								isTaken =1;
								break;
							}
						}
						if(isTaken == 0)	
								strcat(result,doctor[i].lastName);
					}
		}
		if(strlen(result) == 0){
			strcpy(informationMessage.information, "Nikt nie przyjmuje tego dnia!\n");
		}
		else{
			strcpy(informationMessage.information, result);
		}
		informationMessage.mtype = informationMessage.type;
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
			printf("ends doctorsInDay\n");

}
void loginUser(){
	printf("login\n");
	char* login = loginMessage.login;
	char* password = loginMessage.password;
	int i;

	loginMessage.mtype = loginMessage.type;
	
	patientCount = getPatientCount();
	loadPatient();
	for(i=0;i<patientCount;i++){

		int sameLogin = strcmp(patient[i].login, login);
		int samePassword = strcmp(patient[i].password, password);
		printf("%d %d\n",sameLogin, samePassword);
		if(sameLogin == 0 ){
			if(samePassword == 0){
				loginMessage.ok = 1;
				break;
			}
			else{
				patient[i].loginAttempt++;
			}
		}
		else{
			loginMessage.ok = 0;
		}
	}
	savePatient();
	msgsnd(msgid,&loginMessage,sizeof(loginMessage),0);
}
