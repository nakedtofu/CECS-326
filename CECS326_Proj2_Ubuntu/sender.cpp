// sender.cpp

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <string>
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

int main()
{

	buf msg; // creates a message buffer
	int size = sizeof(msg) - sizeof(long); // buffer size - 32bits(size of long)

	char message[50];

	int qid = msgget(ftok(".", 'u'), 0); //obtain queue id

	cout << "Sender: " << qid << " begins execution." << endl;

	cout << "Please enter message within 50 characters: " << endl;
	cin.getline(message, 50);
	cout << endl;

	while ( cin.fail() )
	{

		cout << "Error! Please enter message within 50 characters: " << endl;
		cin.getline(message, 50);
		cout << endl;

	}

	strcpy(msg.greeting, message); // writes new message
	cout << getpid() << ": sends message" << endl; // notify that it will send the new message
	msg.mtype = 114; // set destination to type 114
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sends message to type 114

	cout << getpid() << ": now exits" << endl; // notify that the child process will exit

}
