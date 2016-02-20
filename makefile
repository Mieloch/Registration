sopmake: registration.c patient.c doctor.c
	gcc -o registration patient.c registration.c doctor.c -I -Wall.
