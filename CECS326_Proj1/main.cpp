#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <array>
#include <queue>
#include <string>

using namespace std;

int main()
{

	struct memory_block_table
	{

		int free_blocks = 1024 - 32;
		bool memory_state[1024];

	};

	struct process_control_block
	{

		int PID;
		int * ptable = nullptr;
		int size;
		process_control_block * next = nullptr;

	};

	int * page_table;

	struct ready_queue
	{

		process_control_block * head_ptr = nullptr;
		process_control_block * tail_ptr = nullptr;

	};

	ready_queue rq;
	memory_block_table mbt;
	process_control_block * nextptr = nullptr;
	process_control_block * tempptr = nullptr;
	int PID = 0, block_req = 0, first = 0, last = 0, 
		menu = 0, npl = 0, PID_remove = 0, total_pblocks = 0;
	int unique_ID = 99; //unique ID
	bool detect = false;

	for (int m = 0; m < sizeof(mbt.memory_state); m++)
		mbt.memory_state[m] = 1;

	for (int k = 0; k < 32; k++) //reserve address 0-31
		mbt.memory_state[k] = 0;

	do {

		cout << "Please enter one of the following digits:" << endl;
		cout << "1: Initiate Process" << endl;
		cout << "2: Print all processes in ready queue" << endl;
		cout << "3: Terminate process with a specific PID" << endl;
		cout << "4: Exit" << endl;
		cout << "Menu number: ";
		cin >> menu;

		while ((cin.fail()) | (menu < 1) | (menu > 4))
		{

			cin.clear();
			cin.ignore();
			cout << endl << "Error! Please enter a valid integer between 1 to 4!" << endl;
			cout << "Menu number: ";
			cin >> menu;

		}

		switch (menu)
		{

		case 1:

			nextptr = new process_control_block;

			srand(time(NULL));
			block_req = (rand() % 250 + 10); //generate random number of required blocks
			total_pblocks += block_req;

			if (mbt.free_blocks >= block_req)
			{

				unique_ID++;
				page_table = new int[block_req];
				int z = 0;

				for (int j = 0; j < block_req; j++)
				{

					while (mbt.memory_state[z] != 1)
						z++;

					mbt.memory_state[z] = 0;
					mbt.free_blocks--;

					page_table[j] = z;

				}

				nextptr->PID = unique_ID;
				nextptr->size = block_req;
				nextptr->ptable = page_table;
				nextptr->next = nullptr;

				if (rq.head_ptr == nullptr)
					rq.head_ptr = nextptr;

				if(rq.tail_ptr != nullptr)
					rq.tail_ptr->next = nextptr;
				rq.tail_ptr = nextptr;

				cout << "Page Table:" << endl;
				cout << "Page table of memory block table number: " << rq.head_ptr->PID;
				for (int d = 0; d < rq.head_ptr->size; d++)
				{

					if (d % 20 == 0)
						cout << endl;

					cout << rq.head_ptr->ptable[d] << " ";

				}

				if (rq.head_ptr->next != nullptr)
				{

					nextptr = rq.head_ptr->next;

					while (nextptr != nullptr)
					{

						cout << endl << "Page table of memory block table number: " << nextptr->PID;

						for (int d = 0; d < nextptr->size; d++)
						{

							if (d % 20 == 0)
								cout << endl;

							cout << nextptr->ptable[d] << " ";

						}

						nextptr = nextptr->next;

					}

				}

				cout << endl << "Memory Block Table:";
				for (int d = 0; d < size(mbt.memory_state); d++)
				{

					if (d % 40 == 0)
						cout << endl;

					cout << mbt.memory_state[d] << " ";

				}

				cout << endl;

			}

			else
				cout << "Error! Insufficient free blocks" << endl;

			break;

		case 2:

			if (rq.head_ptr == nullptr)
				cout << "No PCB found in ready queue." << endl;

			else
			{
				cout << "PID in ready queue:" << endl;
				cout << "PID: " << rq.head_ptr->PID << endl;
				nextptr = new process_control_block;

				if (rq.head_ptr->next != nullptr)
				{

					nextptr = rq.head_ptr->next;

					while (nextptr != nullptr)
					{

						cout << "PID: " << nextptr->PID << endl;
						nextptr = nextptr->next;

					}

				}

			}

			break;

		case 3:

			nextptr = new process_control_block;
			tempptr = new process_control_block;

			detect = false;
			cout << "Please enter PID to terminate:";
			cin >> PID_remove;

			if (rq.head_ptr == nullptr)
			{

				cout << "Error! No PCB available for deletion." << endl;
				break;

			}
			
			else if (rq.head_ptr->PID == PID_remove)
			{

				mbt.free_blocks += rq.head_ptr->size;

				for (int u = 0; u < rq.head_ptr->size; u++)
					mbt.memory_state[rq.head_ptr->ptable[u]] = 1;

				if (rq.head_ptr->next == nullptr)
					rq.tail_ptr = nullptr;

				nextptr = rq.head_ptr->next;
				delete rq.head_ptr;
				rq.head_ptr = nextptr;
				detect = true;

			}

			else if (rq.tail_ptr->PID == PID_remove)
			{

				nextptr = rq.head_ptr;

				while (nextptr->next != rq.tail_ptr)
					nextptr = nextptr->next;

				for (int u = 0; u < nextptr->next->size; u++)
					mbt.memory_state[nextptr->next->ptable[u]] = 1;

				mbt.free_blocks += nextptr->next->size;
				delete nextptr->next;

				nextptr->next = nullptr;
				rq.tail_ptr = nextptr;

				detect = true;

			}

			else if (rq.head_ptr->next != nullptr)
			{

				nextptr = rq.head_ptr;
				while ((nextptr->next != nullptr) & (detect == false))
				{

					if (nextptr->next->PID == PID_remove)
					{

						tempptr = nextptr->next;

						for (int u = 0; u < tempptr->size; u++)
							mbt.memory_state[tempptr->ptable[u]] = 1;
						
						mbt.free_blocks += tempptr->size;
						
						nextptr->next = nextptr->next->next;

						delete tempptr;
						detect = true;

					}

					else
						nextptr = nextptr->next;

				}

			}

			if (detect == false)
				cout << "Error! PCB not found. Please check PID." << endl;

			break;
		case 4:
			char exit;

			if (rq.head_ptr == nullptr)
				cout << "No PCB found in ready queue.";

			else
			{

				cout << "PID in ready queue:" << endl;
				cout << "PID: " << rq.head_ptr->PID << endl;
				nextptr = new process_control_block;

				if (rq.head_ptr->next != nullptr)
				{

					nextptr = rq.head_ptr->next;

					while (nextptr != nullptr)
					{

						cout << "PID: " << nextptr->PID << endl;
						nextptr = nextptr->next;

					}

				}

			}

			cout << endl << "Memory Block Table:" << endl;
			for (int d = 0; d < size(mbt.memory_state); d++)
			{

				if (d % 40 == 0)
					cout << endl;

				cout << mbt.memory_state[d] << " ";

			}

			if (rq.tail_ptr != nullptr)
			{

				cout << endl << "Ready queue is not empty. Would you still like to exit?" << endl;
				cout << "Enter 'Y' to exit or 'N' to stay: ";
				cin >> exit;

				while ((cin.fail()) | ((exit != 'Y') & (exit != 'N')))
				{

					cout << endl << "Error! Please enter either 'Y' to exit or 'N' to stay: ";
					cin >> exit;

				}

				if (exit == 'N')
					menu = 0;

				else
				{

					while (rq.head_ptr != nullptr)
					{

						nextptr = rq.head_ptr;

						if (nextptr->next != nullptr)
						{

							rq.head_ptr = rq.head_ptr->next;
							mbt.free_blocks += nextptr->size;

							for (int u = 0; u < nextptr->size; u++)
								mbt.memory_state[nextptr->ptable[u]] = 1;

							delete nextptr;

						}

						else
						{

							mbt.free_blocks += nextptr->size;

							for (int u = 0; u < nextptr->size; u++)
								mbt.memory_state[nextptr->ptable[u]] = 1;

							delete nextptr;
							rq.head_ptr = nullptr;
							rq.tail_ptr = nullptr;

						}

					}

				}

			}

			break;

		default:
			cout << "Error! Please check input!" << endl;
			break;

		};

	} while (menu != 4);

	cout << "Program successfully executed." << endl;

	return 0;

}