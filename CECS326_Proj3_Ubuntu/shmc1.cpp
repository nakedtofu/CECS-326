/* shmc1.cpp */
#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>

using namespace std;

CLASS *class_ptr;
void *memptr;
char *pname;
int shmid, ret;
void rpterror(char *), srand(), perror(), sleep();
void sell_seats();

//This is the shared memory consumer's main code which will sell seats until seats last.
//Otherwise, it will handle and display error messages when appropriate.
main(int argc, char* argv[])
{

	if (argc < 2) //if argc < 2 print system error message and exit
	{

		fprintf (stderr, "Usage:, %s shmid\n", argv[0]);
		exit(1);

	}

	pname = argv[0];
	sscanf (argv[1], "%d", &shmid); //saves shared memory ID into 2nd element in argv as a C string
	
	//attach shared memory of ID shmid granting full permission (read and write)
	//and save the value to the memory pointer
	memptr = shmat (shmid, (void *)0, 0);

	//if memory pointer gets a value of -1 it means an error has occurred;
	//therefore, call the error handler and exit
	if (memptr == (char *)-1 ) 
	{

		rpterror ("shmat failed");
		exit(2);

	}

	class_ptr = (struct CLASS *)memptr; //set global variable class pointer to memory pointer
	sell_seats();
	ret = shmdt(memptr); //detach memory pointer and exit
	exit(0);

}

//This function will handle the selling of the seats.
void sell_seats() 
{

	int all_out = 0;
	srand ( (unsigned) getpid() ); //gets random pid

	//stays in the loop until ticket inventory becomes sold out
	while ( !all_out) 
	{   /* loop to sell all seats */

		//if remaining seat greater than 0, do the following: 
		//-sleep for 1 - 5 seconds(random)
	    //-decrement remaining seat by 1
	    //-sleep for 1 - 5 seconds(random)
		//-print remaining number of seats
		if (class_ptr->seats_left > 0) 
		{

			sleep ( (unsigned)rand()%5 + 1);
			class_ptr->seats_left--;
			sleep ( (unsigned)rand()%5 + 1);
			cout << pname << " SOLD SEAT -- " << class_ptr->seats_left << " left" << endl;

		}
		
		//else do the following:
		//-increment all_out by 1 (set sold out flag to true)
		//-print out no seats left message
		else 
		{

			all_out++;
			cout << pname << " sees no seats left" << endl;

		}

		sleep ( (unsigned)rand()%10 + 1); //sleep for 1 - 10 seconds(random)

	}

}

//Function that handles system errors
void rpterror(char* string)
{

	char errline[50];
	sprintf (errline, "%s %s", string, pname); //stores parameter and program name in errline as a C string
	perror (errline); //print system error message with errline

}
