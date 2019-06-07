/* shmp1.cpp */
#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

using namespace std;

//Operating System class with class no. 1001, date Dec. 1st 1986, and 15 seats in inventory 
CLASS myclass = { "1001", "120186", "Operating Systems", 15 };

#define NCHILD 3 //number of child processes to make

//create named mutex semaphore with read write permission for user and group
sem_t *sem = sem_open( "naoaki", O_CREAT | O_EXCL, 0660, 1 );

int shm_init( void * );
void wait_and_wrap_up( int [], void *, int );
void rpterror( char *, char * );

//Main function that creates child processes and waits until the child processes complete
main(int argc, char *argv[])
{

	int child[NCHILD], i, shmid, semid;
	void *shm_ptr;
	char ascshmid[10], ascsemid[10], pname[14];
	strcpy (pname, argv[0]); //copy C string of program name and store it into char array pname
	
	//call shared memory initialize function passing shared memory pointer as
	//the parameter and store returned value(ID) into shared memory ID
	shmid = shm_init(shm_ptr);
	
	//stores shared memory ID into ascii type shared memory ID
	sprintf (ascshmid, "%d", shmid);

	for (i = 0; i < NCHILD; i++) //for loop to create 3 child processes
	{

		child[i] = fork();

		// -go through case statement checking for forked value;
		// -if -1 it is the child process, therefore display fork error and exit
		// -if 0 it is the parent process, therefore create child processes
		//  and make the child exit after completion
		switch (child[i]) 
		{

			case -1:
				rpterror ("fork failure", pname);
				exit(1);

			case 0:
				sprintf (pname, "shmc%d", i+1);
				execl("shmc1", pname, ascshmid, (char *)0);
				perror ("execl failed");
				exit (2);

		}
	}

	wait_and_wrap_up (child, shm_ptr, shmid); // wait until child finishes

}

//Function that creates shared memory together with its ID, attaches the ID specified shared memory
//to the shared memory pointer, and copies the content/memory into the pointer
int shm_init(void *shm_ptr)
{

	int shmid;
	//  create a shared memory of class size with permission only granted to the
	//  owner for reading and writing(0600) in the current directory and save
	//  identifier value into shared memory ID
	shmid = shmget(ftok(".",'u'), sizeof(CLASS), 0600 | IPC_CREAT);

	if (shmid == -1) 
	{
		perror ("shmget failed");
		exit(3);
	}

	//attach shared memory of ID shmid granting full permission(read and write)
	//and save the value to the shared memory pointer
	shm_ptr = shmat(shmid, (void * ) 0, 0);

	if (shm_ptr == (void *) -1) 
	{
		perror ("shmat failed");
		exit(4);
	}

	//copy class size bytes of memory from myclass and store it into shared memory pointer
	memcpy (shm_ptr, (void *) &myclass, sizeof(CLASS) );
	return (shmid);

}

//Function that halts (busy wait) until child completes process and 
//deatach/destroy the shared memory segment
void wait_and_wrap_up(int child[], void *shm_ptr, int shmid)
{

	int wait_rtn, w, ch_active = NCHILD;

	while (ch_active > 0) //loop until no more active child left
	{

		wait_rtn = wait( (int *)0 ); // -wait for child process to complete

		//if child process complete, decrement active child # and break from for loop
		for (w = 0; w < NCHILD; w++)
			if (child[w] == wait_rtn) 
			{
				ch_active--;
				break;
			}

	}

	//dettatch semaphore
	sem_unlink( "semaphore" );
	//close semaphore
	sem_close( sem );

	cout << "Parent removing shm" << endl;
	shmdt (shm_ptr); //parent detaches shared memory
	shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0); //marks shared memory as destroyed
	exit (0);

}

//Function that handles system errors
void rpterror(char *string, char *pname)
{

	char errline[50];
	sprintf (errline, "%s %s", string, pname); //stores parameter and program name in errline as a C string
	perror (errline); //print system error message with errline

}
