// receiver.cpp

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
	int qid = msgget(ftok(".", 'u'), 0); //obtain queue id

	cout << "Receiver: " << qid << " begins execution." << endl;

	msgrcv(qid, (struct msgbuf *)&msg, size, 114, 0); // receives message from type 114
	cout << getpid() << ": gets message" << endl; // notify that it has recieved the message
	cout << "Message content: " << msg.greeting << endl; // print out message content

	cout << getpid() << ": now exits" << endl; // notify that the child process will exit

}
