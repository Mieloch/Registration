#include "headers.h"
void sendPatientData();
void login();
void setVisit();
void getFromUser(char* msg);
void getDoctorInDate();
void chooseDoctor();
void editVisit();
void getVisitInfo();
void cancelVisit();
AddPatientMessage addPatientMessage;
LoginMessage loginMessage;
InformationMessage informationMessage;
int msgId;
char* logedLogin;
int isLoged;
char buffer[1000];
int pid;
int day,month,year;
int main(){
	day = month = year = -1;
	isLoged = 0;
	msgId = msgget(997,0666);
	if(msgId == -1){
		perror("utworzenie kolejki komunikatow");
	}
	pid = getpid();
	
	while(1){
		if(isLoged){
			printf("Zalogowano jako: %s\n", logedLogin);
		}
		printf("#1. Zarejestruj się\n");
		printf("#2. Zaloguj się\n");
		printf("#3. Umów się na wizyte\n");
		printf("#4. Edytuj wizyte\n");
		char choose;
		fgets(&choose,5,stdin);
		switch(choose){
			case '1': // dodanie pacjenta
			{
				sendPatientData();
				break;
			}
			case '2': // logowanie
			{
				login();
				break;
			}
			case '3': // umawianie wizyty
			{
				setVisit();
				break;
			}
			case '4': // umawianie wizyty
			{
				editVisit();
				break;
			}
		}
		sleep(1);
		day = year = month = -1;
	}
	return 0;
}
void editVisit(){
	if(isLoged == 0){
		printf("\n!!Musisz byc zalogowany by edytowac wizyty!!\n");
		return;
	}
	char choose;
	do{
		getVisitInfo();
		
		printf("\n#1. Odwolaj wizyte\n"); 
		printf("#2. Zmien termin wizyty\n"); 
		printf("#q. Cofnij\n"); 
		
		fgets(&choose,5,stdin);
		switch(choose){
			case '1' : {
				cancelVisit();
				break;
			}
			case '2' : {
				//chooseDoctor();
				break;
			}
		}
		
		
	}while(choose != 'q');
	
}
void getVisitInfo(){
	printf("start getvisitInfo\n");
	strcpy(informationMessage.login, logedLogin);
	informationMessage.type = pid;
	informationMessage.mtype = 6;
	msgsnd(msgId, &informationMessage, sizeof(informationMessage),0);
	msgrcv(msgId, &informationMessage, sizeof(informationMessage),pid,0);
	if(strlen(informationMessage.information)==0){
		printf("Nie masz umowionej wizyty\n");
	}
	else{
		printf("Jestes umowiony do\n %s\n", informationMessage.information);
	}
}
void cancelVisit(){
	strcpy(informationMessage.login, logedLogin);
	informationMessage.type = pid;
	informationMessage.mtype = 5;
	msgsnd(msgId, &informationMessage, sizeof(informationMessage),0);
	msgrcv(msgId, &informationMessage, sizeof(informationMessage),pid,0);
	printf("%s\n",informationMessage.information);
}
void setVisit(){
	if(isLoged == 0){
		printf("\n!!Musisz byc zalogowany by umowic sie na wizyte!!\n");
		return;
	}
	char choose;
		
	do{
		printf("\n#1. Wyswietl lekarzy przyjmujacych danego dnia\n"); 
		printf("#2. Umów wizyte do lekarza na wybrana date\n"); 
		if(day != -1 && month && -1 && year != -1){
			printf("Wybrana data: %d-%d-%d\n", day,month,year);
		}
		printf("#q. Cofnij\n"); 
		fgets(&choose,5,stdin);
		switch(choose){
			case '1' : {
				getDoctorInDate();
				break;
			}
			case '2' : {
				chooseDoctor();
				break;
			}
		}
		
		
	}while(choose != 'q');
}
void chooseDoctor(){
	if(day == -1 || month  == -1 || year == -1){
		printf("Wybierz date.\n");
		getFromUser("Podaj dzien: ");
		informationMessage.day = day = atoi(buffer);
		getFromUser("Podaj miesiac: ");
		informationMessage.month = month = atoi(buffer);
		getFromUser("Podaj rok: ");
		informationMessage.year = year = atoi(buffer);
	}
	
	getFromUser("Podaj godzine: \n");
	informationMessage.term= atoi(buffer);
	
	getFromUser("Podaj nazwisko lekarza: \n");
	strcpy(informationMessage.doctorLastName, buffer);
	
	strcpy(informationMessage.login, logedLogin);
	informationMessage.type = pid;
	informationMessage.mtype = 4;
	msgsnd(msgId, &informationMessage, sizeof(informationMessage),0);
	msgrcv(msgId, &informationMessage, sizeof(informationMessage),pid,0);
	printf("\n%s\n", informationMessage.information);
	
}
void getDoctorInDate(){
	getFromUser("Podaj dzien: ");
	informationMessage.day = day = atoi(buffer);
	getFromUser("Podaj miesiac: ");
	informationMessage.month = month = atoi(buffer);
	getFromUser("Podaj rok: ");
	informationMessage.year = year = atoi(buffer);
	
	informationMessage.type = pid;
	informationMessage.mtype = 3;
	msgsnd(msgId, &informationMessage, sizeof(informationMessage),0);
	msgrcv(msgId, &informationMessage, sizeof(informationMessage),pid,0);
	printf("\nLekarze przyjmujacy tego dnia:\n %s\n", informationMessage.information);

}
void login(){
	if(isLoged){
		printf("Nastapilo wylogowanie\n");
	}
	getFromUser("Podaj login: ");
	strcpy(loginMessage.login, buffer);
	getFromUser("Podaj haslo: ");
	strcpy(loginMessage.password, buffer);
	
	loginMessage.type = pid;
	loginMessage.mtype = 2;
	msgsnd(msgId, &loginMessage, sizeof(loginMessage),0);
	msgrcv(msgId, &loginMessage, sizeof(loginMessage),pid,0);
	if(loginMessage.ok == 1){
		printf("Zalogowano\n");
		logedLogin = loginMessage.login;
		isLoged = 1;
	}else{
		printf("Podano zle dane logowania\n");
		isLoged = 0;
	}
}
void sendPatientData(){
	getFromUser("Podaj imie: ");
	strcpy(addPatientMessage.name, buffer);
	getFromUser("Podaj nazwisko: ");
	strcpy(addPatientMessage.lastName, buffer);
	getFromUser("Podaj pesel: ");
	strcpy(addPatientMessage.pesel, buffer);
	getFromUser("Podaj login: ");
	strcpy(addPatientMessage.login, buffer);
	getFromUser("Podaj haslo: ");
	strcpy(addPatientMessage.password, buffer);
	
	addPatientMessage.type = pid;
	addPatientMessage.mtype = 1;
	
	msgsnd(msgId, &addPatientMessage, sizeof(addPatientMessage),0);
}
void getFromUser(char* msg){
	printf("%s",msg);
	fgets(buffer,sizeof(buffer),stdin);
}
