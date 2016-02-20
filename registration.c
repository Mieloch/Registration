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
void cancelVisit();
char buffer[1000];
AddPatientMessage addPatientMessage;
LoginMessage loginMessage;
InformationMessage informationMessage;
int patientCount;
int doctorCount;
int visitTerms[3] = {9,10,11};
//extern Patient patient[100];


int main(int argc, char** argv){
	
	msgid = msgget(997,IPC_CREAT | 0666);
	if(msgid == -1) perror("Tworzenie kolejki komunikatow");
	
	patientCount = getPatientCount();
	loadPatient(); 
	loadDoctors();
	doctorCount = getDoctorCount();
	//printDoctors();
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
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),4,IPC_NOWAIT) != -1){
			addVisit();
		}
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),5,IPC_NOWAIT) != -1){
			cancelVisit();
		}
		else if(msgrcv(msgid,&informationMessage,sizeof(informationMessage),6,IPC_NOWAIT) != -1){
			sendVisitInfo();
		}


		//printPatients();
		sleep(1);
	}
	return 0;
}
int getDayOfWeek(int d, int m, int y){
	return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
}
void sendVisitInfo(){
	printf("a\n");
		loadPatient();
	printf("b\n");
		char buffer[100];
		char result[1000];
		strcpy(result, "");
		int i,k;
		int patientCount = getPatientCount();
		printf("A\n");
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
		printf("B\n");
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
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
		char buffer[100];
		int i,k;
		int day = informationMessage.day;
		int month = informationMessage.month -1;
		int year = informationMessage.year -1900;
		int term = informationMessage.term;
		int patientCount = getPatientCount();
		strcpy(informationMessage.information, "Cos poszlo nie tak");
		for(i=0;i<patientCount;i++){
			if(strcmp(patient[i].login, informationMessage.login)==0){
				strcpy(patient[i].doctorLastName, "");
				strcpy(patient[i].doctorName, "");

				patient[i].visitDate.tm_year = 0;
				strcpy(informationMessage.information, "Odwolano");
				break;
			}
		}
		savePatient();
		loadVisits();
		informationMessage.mtype = informationMessage.type;
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
		
}
void addVisit(){
			printf("add0\n");

		loadVisits(); // aktualizacja wizyt
		char buffer[100];
		int i,k;
		int day = informationMessage.day;
		int month = informationMessage.month -1;
		int year = informationMessage.year -1900;
		int term = informationMessage.term;
		printf("add1\n");
		//sprawdzanie terminu
		if(checkFreeTerm(informationMessage.doctorLastName,day,month,year,term) == 1){
					printf("add2\n");

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
		else{ // nie ma wolnego
			strcpy(informationMessage.information, "Podany termin jest zajety!\n\n");
		}
		
		informationMessage.mtype = informationMessage.type;
		msgsnd(msgid,&informationMessage,sizeof(informationMessage),0);
		
}

void sendDoctorsInDate(){
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
						isTaken = 0;
						strftime(buffer,100,"%w",&doctor[i].visitDay);
						int doctorVisitDay = atoi(buffer);
						for(k=0;k<20;k++){	
							if(doctor[i].visits[k].tm_hour == visitTerms[j] && doctorVisitDay == dayOfWeek){
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
		if(sameLogin == 0 && samePassword == 0){
			loginMessage.ok = 1;
			break;
		}
		else{
			loginMessage.ok = 0;
		}
	}
	msgsnd(msgid,&loginMessage,sizeof(loginMessage),0);
}
