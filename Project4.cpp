//============================================================================
// Name        : Project4.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <cstring>

using namespace std;

//message struct to store the fields of the message
struct message {
	string from;
	string date;
	string email_adress;
	string subject;
	string msg;
	string message_body;
	string id;
	message() {
		from = date = email_adress = subject = message_body = msg = id = "";
	}
	void display() {
		cout << from << endl << endl;
		cout << date << endl << endl;
		cout << email_adress << endl << endl;
		cout << subject << endl << endl;
		cout << msg << endl << endl;
		cout << message_body << endl << endl;
		cout << id << endl << endl;
	}
};

/*
 * A function to read the mbox
 * filename is the name of the file to read
 * message * return type means return an array of the messages which is of type struct
 */
message* readBox(string filename) {
	message * messages[10];
	for (int i = 0; i < 10; i++) {
		messages[i] = new message();
	}
	ifstream box;
	string fname = filename;
	box.open(filename.c_str());
	if (box.is_open()) {
		int msgCount = 0;
		//outputting file
		while (!box.eof()) {
			getline(box, messages[msgCount]->from);
			getline(box, messages[msgCount]->date);
			getline(box, messages[msgCount]->email_adress);
			getline(box, messages[msgCount]->subject);
			getline(box, messages[msgCount]->msg);
			getline(box, messages[msgCount]->message_body);
			getline(box, messages[msgCount]->id);
			msgCount++;
		}
	} else {
		cout << "The mbox file doesn't exist\n";
	}
	return *messages;
}

/*
 * A function to save the mailbox to the file
 * Argument: <filename> The name of the file to save the mailbox
 * Argument: <messageArray> the array of message structure which is write to the file
 * Argument: <length> the length of the messages
 */

void savebox(string filname, message** messageArray, int length) {
	ofstream file;
	file.open(filname.c_str());
	int saveCount = 0;
	for (int i = 0; i < length; i++) {
		if (messageArray[i] != NULL) {
			file << messageArray[i]->from << endl;
			file << messageArray[i]->date << endl;
			file << messageArray[i]->email_adress << endl;
			file << messageArray[i]->subject << endl;
			file << messageArray[i]->msg << endl;
			file << messageArray[i]->message_body << endl;
			file << messageArray[i]->id << endl;
			file << endl;
			saveCount++;
		}

	}

	cout << "\t" << saveCount << " Messages saved\n" << endl;
	file.close();
}

/*Check authenticity is a function to check if the message is valid or not by
 * checking it's values and the id across it if they match
 * Argument: <structArray> this is the mail box which is of valid messages
 * Argument: <toCheck> the struct pointer of which values are compared
 */
bool checkAuthenticity(message** structArray, message *toCheck, int length) {

	bool flag = true;
	for (int i = 0; i < length; i++) {
//		structArray[i]->display();
		if (structArray[i] != NULL)
			if (structArray[i]->id == toCheck->id)
//				&& structArray[i]->date == toCheck->date
//				&& structArray[i]->email_adress == toCheck->email_adress
//				&& structArray[i]->subject == toCheck->subject
//				&& structArray[i]->msg == toCheck->msg
//				&& structArray[i]->message_body == toCheck->message_body &&structArray[i]->id!=toCheck->id)
					{
//			if (structArray[i]->id != toCheck->id)
				flag = false; //end of the if

			}

	}

	return flag;
}

/*
 * A function to clear the space allocated
 * Argument is the length of the array and it's pointer
 */
void clearMemory(message** clear, int length) {
	for (int i = 0; i < length; i++) {
		delete clear[i];
	}
}

/*
 * This will double the size of the structure
 */
void IncreaseSize(message** hacked, int len) {
	message * temp[len * 2];
	for (int i = 0; i < len; i++) {
		temp[i] = hacked[i];
	}
	hacked = temp;
}

int main() {
	int vInitial = 10;
	int hInitial = 10; // variable to detect if the array grow to its max size
	message * valid[vInitial];  //An array to store the valid message
	message * hacked[hInitial]; //An array to store the hacked messages

	int validCount = 0; //The message count in the valid
	int hackedCount = 0; //The message count in hacked

	ifstream myReadFile; // To read file
	myReadFile.open("commandfile.txt"); //opening command file conataining commands
	if (myReadFile.is_open()) {
		string buffer;
		string subBuffer;

		//will read the file till end is reached
		while (!myReadFile.eof()) {
			buffer = "";
			myReadFile >> buffer;  //reading file word by word
//			getline(myReadFile, buffer);
			if (buffer == "load") {
				myReadFile >> subBuffer;
				cout << "Command: " << buffer + " " + subBuffer << endl << endl;
				message* mbox = readBox(subBuffer);   //to load the mbox

				valid[validCount] = mbox;
				validCount++;
//				mbox[0].display();

				cout << "\tMessage: " << validCount << endl << endl;
			} else if (buffer == "add") {
				cout << "Command: " << buffer << endl << endl;
				message *read;
				myReadFile >> subBuffer; // to move to the new line in the file

				read = new message();
				//reading the mail from the file
				getline(myReadFile, read->from);
				getline(myReadFile, read->date);
				getline(myReadFile, read->email_adress);
				getline(myReadFile, read->subject);
				getline(myReadFile, read->msg);
				getline(myReadFile, read->message_body);
				getline(myReadFile, read->id);

				//to see if the message is not hacked
				if (checkAuthenticity(valid, read, validCount) == true) {
					valid[validCount] = read;
					validCount++;
					if (validCount >= vInitial) {
						IncreaseSize(valid, vInitial);
						vInitial *= 2;
					}
//				read->display();
					cout << "\tMessage Added\n" << endl;
				} else {
					cout << "\tMessage not authentic. Send to hacked list\n"
							<< endl;
					hacked[hackedCount] = read;
					hackedCount++;
					// If the
					if (hackedCount >= hInitial) {
						IncreaseSize(hacked, hInitial);
						hInitial *= 2;
					}
				}

			} else if (buffer == "show") {
				myReadFile >> subBuffer;
				cout << "Command: " + buffer + " " + subBuffer << endl << endl;
				if (subBuffer == "inbox") {
					for (int i = 0; i < validCount; i++) {
						if (valid[i] != NULL) {
							valid[i]->display();
							cout << endl;
						}
					}
				} else {
					for (int i = 0; i < hackedCount; i++) {
						if (hacked[i] != NULL) {
							hacked[i]->display();
							cout << endl;
						}
					}
				}
			} else if (buffer == "remove") {
				myReadFile >> subBuffer;
				if (subBuffer == "inbox") {
					myReadFile >> subBuffer;
					//converting string to number
					istringstream convert(subBuffer);
					int index;
					convert >> index;
					cout << "Command: " << buffer + " " + " " << index << endl
							<< endl;
					if (index >= 0 && index <= validCount) {
						if (valid[index] != NULL)
							delete valid[index];
						cout << "\tMessage Removed\n" << endl;
					} else {
						cout << "\tMessage not removed\n" << endl;
						cout << "\tMesage " << index
								<< " not a valid message number\n" << endl;
					}
				}
				if (subBuffer == "hacked") {
					string number = " ";
					myReadFile >> number;
					//converting string to number
					istringstream convert(number);
					int index;
					convert >> index;
					cout << "Command: " << buffer + " " + subBuffer + " "
							<< index << endl;
					if (index >= 0 && index <= validCount) {
						if (hacked[index] != NULL)
							delete hacked[index];
						cout << "\tMessage remove\n" << endl;

					} else {
						cout << "\tMessage not removed\n" << endl;
						cout << "\tMessage " << index
								<< " not a valid message number\n" << endl;

					}
				}

			} else if (buffer == "save") {
				myReadFile >> subBuffer;
				if (subBuffer == "inbox") {
					string filename = "";
					myReadFile >> filename;
					cout << "Command: " << buffer << " " << subBuffer << " "
							<< filename << endl << endl;

					savebox(filename, valid, validCount);
				} else if (subBuffer == "hacked") {
					string filename = "";
					myReadFile >> filename;
					savebox(filename, valid, hackedCount);
				}
			}
		}
	} else
		cout
				<< "The command file does not exist, please make sure the commandfile.txt is in the correct location.\n"
				<< endl;
	myReadFile.close();

//	clearMemory(valid, validCount);
//	clearMemory(hacked, hackedCount);
	return 0;
}

