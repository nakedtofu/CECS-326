// master.cpp
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

// message queue (buffer)
struct buf
{

	long mtype; // type(address) of message
	char greeting[50]; // message size of 50 characters

};

//first child
void child_proc_one() //receiver child
{
	
	execlp("./sender", "NULL", NULL);

}

void child_proc_two() //sender child
{

	execlp("./receiver", "NULL", NULL);

}

int main()
{

	int qid = msgget(ftok(".", 'u'), IPC_EXCL | IPC_CREAT | 0600); // get qid using ftok()

	// spawning two child processes
	pid_t cpid = fork(); // spawn first child

	if (cpid == 0) // check if parent or child; if child, then execute
	{
		child_proc_one(); // call the child one function
		exit(0); // exit process when finished
	}

	pid_t cpid_1 = cpid; // save cpid1

	cpid = fork(); // spawn second child

	if (cpid == 0) // check if parent or child; if child, then execute
	{
		child_proc_two(); // call the child two function
		exit(0); // exit process when finished
	}

	pid_t cpid_2 = cpid; // save cpid2

	cout << endl;
	cout << "Message Queue ID: " << qid << endl;
	cout << "Child 1 Process ID: " << cpid_1 << endl;
	cout << "Child 2 Process ID: " << cpid_2 << endl;

	while (wait(NULL) != -1); // waiting for both children to terminate
	msgctl(qid, IPC_RMID, NULL); // terminate message queue (buffer)
	cout << "parent proc: " << getpid() << " now exits" << endl; // notify that the parent will exit
	exit(0); // parent exits

}
