#include "headers.h"
#include "doctor.h"
#include "patient.h"

char* doctorDB = "doctorDB";
int doctorCount;
struct Doctor doctor[10];

void loadDoctors(){
		FILE *fp = fopen(doctorDB,"r");
	if(fp == NULL){ // jesli nie ma pliku tworzy go i wpisuje ilosc lekarzy=0
		fp = fopen(doctorDB,"wr");
		char pc = '0';
		fputs(&pc, fp);
		fputs("\n",fp);
	}
	else{
			char buffer[1000];
			char dateBuff[1000];
			fgets(buffer, sizeof(int),fp);
			doctorCount = atoi(buffer);
			int i;
			for(i = 0; i<doctorCount;i++){
				fgets(buffer,sizeof(buffer),fp); // wczytaj imie
				strcpy(doctor[i].name,buffer);
				
				fgets(buffer,sizeof(buffer),fp); // wczytaj nazwisko
				strcpy(doctor[i].lastName,buffer);
				
				fgets(buffer,sizeof(buffer),fp); // wczytaj date urlopu
				strptime(buffer,"%c",&doctor[i].vacationDate);
				
				fgets(buffer,sizeof(buffer),fp); // wczytaj dlugosc urlopu
				doctor[i].vacationDuration = atoi(buffer);
				
				fgets(buffer,sizeof(buffer),fp); // wczytaj termin w jakim przymuje
				strptime(buffer,"%w",&doctor[i].visitDay);
				strftime(buffer,100,"%a",&doctor[i].visitDay);
					
			}
			loadVisits();		
	}
	fclose(fp);
}
void loadVisits(){
	printf("start loadVisit\n");
	char buffer[1000];
	int i,j,k;
	for(i =0;i<doctorCount;i++){
		k=0;
		for(j=0;j<patientCount;j++){
			if(!strcmp(patient[j].doctorLastName,doctor[i].lastName)){
				doctor[i].visits[k] = patient[j].visitDate;
				k++;
			}
		}
	}
	printf("koniec loadVisit\n");
}

void saveDoctors(){
	int doctorCount = getDoctorCount();
	int i;
	FILE *fp = fopen(doctorDB, "w");
	char intStr[10];
	char buffer[100];
	sprintf(intStr,"%d", doctorCount);
	fputs(intStr,fp);
	fputs("\n",fp);
	for(i=0;i<doctorCount;i++){
		fputs(doctor[i].name,fp); // imie
		fputs(doctor[i].lastName,fp); // nazwisko
		
		strftime(buffer,100,"%c",&doctor[i].vacationDate); // data urlopu
		fputs(buffer,fp);
		fputs("\n",fp);
		
		sprintf(intStr, "%d", doctor[i].vacationDuration); // dlugosc urlopu
		fputs(intStr,fp);
		fputs("\n",fp);
		
		strftime(buffer,100,"%w",&doctor[i].visitDay); //dzien przyjecia
		fputs(buffer,fp);
		fputs("\n",fp);		
	}
	fclose(fp);
}

void printDoctors(){
	int i;
	char buffer[100];
	for(i=0;i<doctorCount;i++){
		printf("#Lekarz nr: %d\n",(i+1));
		printf("Imie: %s",doctor[i].name);
		printf("Nazwisko: %s",doctor[i].lastName);
		strftime(buffer,100,"%w",&doctor[i].visitDay);
		printf("Dzien przyjecia: %s\n",buffer);
		strftime(buffer,100,"%d %b %Y",&doctor[i].vacationDate);
		printf("Początek urlupu: %s\n",buffer);
		printf("Długość urlopu: %d\n",doctor[i].vacationDuration);
		
	}
}
void incrementDoctorCount(){
	int doctorCount = getDoctorCount();
	doctorCount++;
	FILE *fp = fopen(doctorDB,"r+");
	fseek(fp,0,SEEK_SET);
	char intStr[10];
	sprintf(intStr,"%d", doctorCount);
	fputs(intStr,fp);
	fclose(fp);
}
int getDoctorCount(){
    FILE *fp = fopen(doctorDB, "r");
    char buffer[20];
    if (fp != NULL) {		    
        fgets(buffer, sizeof(int), fp);
        doctorCount = atoi(buffer);
        fclose(fp);
        return doctorCount;
    }
    else {
        return 0;
    }
}
