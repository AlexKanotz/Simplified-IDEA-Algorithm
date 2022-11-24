/*
Name: Alex Kanotz
Date: 11/23/2022
Assignment 4: Secure Computing Assignments
Due Date: 11/25/2022
Class: CEN4020 - Software Engineering I

About this project:
This program will implement a simplified verion
of an IDEA encryption algorithm.

Compile code:
g++ -o main -std=c++11 main.cpp

Assumptions: NA
*/


#include <iostream>
#include <string>
#include <bitset>

using namespace std;


// Global and constant variables
const int MAXTEXTSIZE = 4; // Max size for text array
const int MAXKEYSIZE = 8; // Max size for key array
const int MAXARRAYSIZE = 14; // Max size for encrypted array

// Function prototypes
bool validateInput(const string input);                             // Validate user input
void updateKey(unsigned int key[MAXKEYSIZE], const unsigned int k); // Update key after circular shift
unsigned int circularShift(unsigned int key[MAXKEYSIZE]);           // Shift 32-bit key

// First round
void firstRound(unsigned int text[MAXTEXTSIZE], unsigned int key[MAXKEYSIZE]);
// Second, third, and fourth round
void secondThirdFourthRound(unsigned int text[MAXTEXTSIZE], unsigned int key[MAXKEYSIZE]);
// Fifth round
void fifthRound(unsigned int text[MAXTEXTSIZE], unsigned int key[MAXKEYSIZE]);


// ***********************************************
// *                main function                *
// ***********************************************
int main()
{
	// Initializations
	string text = ""; // String of text
	string key = "";  // Binary key

	unsigned int binaryTextArray[MAXTEXTSIZE] = {0,0,0,0};       // Store binary text
	unsigned int binaryKeyArray[MAXKEYSIZE] = {0,0,0,0,0,0,0,0}; // Store binary key

	// Prompt user to enter a string of text
	do
	{
		cout << "Enter a string of text: ";
		getline(cin, text);
	} while (!validateInput(text));

	int inputLength = text.length(); // Length of text
	unsigned int temp = 0; // Temporary storage

	// Convert text from string to unsigned int
	for (int i = 0; i < inputLength; i++)
		temp += text[i];

	// Store 4 blocks of 4 bits each from text in array
	text = bitset<16>(temp).to_string();
	string s1 = "";
	int const x = 4;
	int y = 0;

	for (int i = 0; i < MAXTEXTSIZE; i++)
	{
		s1 = text.substr(x * y, x);
		binaryTextArray[i] = stoi(s1, nullptr, 2);
		y++;
	}

	// Prompt user to enter a 32-bit key in binary
	inputLength = 0;
	do
	{
		cout << "Enter a 32-bit key in binary: ";
		getline(cin, key);

		inputLength = key.length(); // Length of key
		string tempKey = "", str = "";
		int j = 0, k = 0;

		// Convert text from string to unsigned int
		for (int i = 0; i < inputLength; i++)
		{
			if (!isspace(key[i]))
			{
				j++;
				tempKey += key[i];
				str += key[i];
				if (j % 4 == 0)
				{
					binaryKeyArray[k] = stoi(tempKey, nullptr, 2);
					tempKey = "";
					k++;
					j = 0;
				}
			}
		}
		inputLength = str.length();
		if (inputLength != 32)
			cout << "Invalid bit size!" << endl;
	} while (inputLength != 32);

	// Perform encryption algorithm
	firstRound(binaryTextArray, binaryKeyArray);
	secondThirdFourthRound(binaryTextArray, binaryKeyArray);
	secondThirdFourthRound(binaryTextArray, binaryKeyArray);
	secondThirdFourthRound(binaryTextArray, binaryKeyArray);
	temp = circularShift(binaryKeyArray); // Circular shift
	updateKey(binaryKeyArray, temp);      // Update key
	fifthRound(binaryTextArray, binaryKeyArray);

	cout << "Encrypted text: ";
	for (int i = 0; i < MAXTEXTSIZE; i++)
		cout << bitset<4>(binaryTextArray[i]) << " ";
	cout << endl;

	return 0;
}


// ***********************************************
// *           validateInput function            *
// ***********************************************
// Return true if input was entered correctly, false
// otherwise.
bool validateInput(const string input)
{
	int inputLength = input.length(); // Length of input

	// Determine if string is 16-256 characters long
	if (inputLength < 16 || inputLength > 256)
	{
		cout << "Invalid input length!" << endl;
		return false;
	}

	// Determine if string does not contain ASCII values
	for (int i = 0; i < inputLength; i++)
	{
		char c = input[i];
		// Validate character's ASCII value
		if (int(c) > 127)
		{
			cout << "Invalid ASCII value" << endl;
			return false;
		}
	}
	return true;
}


// ***********************************************
// *              updateKey function             *
// ***********************************************
// Update key after circular shift.
void updateKey(unsigned int key[MAXKEYSIZE], const unsigned int k)
{
	string str = bitset<32>(k).to_string();
	string s1 = "";
	int const x = 4;
	int y = 0;

	for (int i = 0; i < MAXKEYSIZE; i++)
	{
		s1 = str.substr(x * y, x);
		key[i] = stoi(s1, nullptr, 2);
		y++;
	}
}


// ***********************************************
// *           circularShift function            *
// ***********************************************
// Shift 32-bit word 6 places to the right.
unsigned int circularShift(unsigned int key[MAXKEYSIZE])
{
	// Shift the bits
	string shiftKey = "";
	for (int i = 0; i < MAXKEYSIZE; i++)
	{
		shiftKey += bitset<4>(key[i]).to_string();
	}

	auto bit = bitset<32>(shiftKey);
	unsigned int b = bit.to_ulong();

	return (b << 6) | (b >> (32 - 6));
}


// ***********************************************
// *             firstRound function             *
// ***********************************************
// Complete the first round which consists of 14
// steps that use addition modulo, multiplication
// modulo, and bitwise XOR. Steps 11, 12, 13, and
// 14 will become the updated encrypted text.
void firstRound(unsigned int text[MAXTEXTSIZE], unsigned int key[MAXKEYSIZE])
{
	// Create an array to store the values from each step
	unsigned int encrypted[MAXARRAYSIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	// Perform steps
	encrypted[0] = (text[0] * key[0]) % 17;            // Step 1: X1 * K1
	encrypted[1] = (text[1] + key[1]) % 16;            // Step 2: X2 + K2
	encrypted[2] = (text[2] + key[2]) % 16;            // Step 3: X3 + K3
	encrypted[3] = (text[3] * key[3]) % 17;            // Step 4: X4 * K4
	encrypted[4] = encrypted[0] ^ encrypted[2];        // Step 5: Step 1 ^ Step 3
	encrypted[5] = encrypted[1] ^ encrypted[3];        // Step 6: Step 2 ^ Step 4
	encrypted[6] = (encrypted[4] * key[4]) % 17;       // Step 7: Step 5 * K5
	encrypted[7] = (encrypted[5] + encrypted[6]) % 16; // Step 8: Step 6 + Step 7
	encrypted[8] = (encrypted[7] * key[5]) % 17;       // Step 9: Step 8 * K6
	encrypted[9] = (encrypted[6] + encrypted[8]) % 16; // Step 10: Step 7 + Step 9
	encrypted[10] = encrypted[0] ^ encrypted[8];       // Step 11: Step 1 ^ Step 9
	encrypted[11] = encrypted[2] ^ encrypted[8];       // Step 12: Step 3 ^ Step 9
	encrypted[12] = encrypted[1] ^ encrypted[9];       // Step 13: Step 2 ^ Step 10
	encrypted[13] = encrypted[3] ^ encrypted[9];       // Step 14: Step 4 ^ Step 10

	text[0] = encrypted[10];
	text[1] = encrypted[11];
	text[2] = encrypted[12];
	text[3] = encrypted[13];
}


// ***********************************************
// *       secondThirdFourthRound function       *
// ***********************************************
// Complete the second, third, and fourth round which
// consists of 14 steps that use addition modulo,
// multiplication modulo, and bitwise XOR. Steps
// 11, 12, 13, and 14 will become the updated encrypted
// text. A circular shift will be used.
void secondThirdFourthRound(unsigned int text[MAXTEXTSIZE], unsigned int key[MAXKEYSIZE])
{
	// Create an array to store the values from each step
	unsigned int encrypted[MAXARRAYSIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	// Perform steps
	encrypted[0] = (text[0] * key[6]) % 17;            // Step 1: X1 * K7
	encrypted[1] = (text[1] + key[7]) % 16;            // Step 2: X2 + K8
	unsigned int temp = circularShift(key);            // Circular shift
	updateKey(key, temp);                              // Update key
	encrypted[2] = (text[2] + key[2]) % 16;            // Step 3: X3 + K3
	encrypted[3] = (text[3] * key[3]) % 17;            // Step 4: X4 * K4
	encrypted[4] = encrypted[0] ^ encrypted[2];        // Step 5: Step 1 ^ Step 3
	encrypted[5] = encrypted[1] ^ encrypted[3];        // Step 6: Step 2 ^ Step 4
	encrypted[6] = (encrypted[4] * key[4]) % 17;       // Step 7: Step 5 * K5
	encrypted[7] = (encrypted[5] + encrypted[6]) % 16; // Step 8: Step 6 + Step 7
	encrypted[8] = (encrypted[7] * key[5]) % 17;       // Step 9: Step 8 * K6
	encrypted[9] = (encrypted[6] + encrypted[8]) % 16; // Step 10: Step 7 + Step 9
	encrypted[10] = encrypted[0] ^ encrypted[8];       // Step 11: Step 1 ^ Step 9
	encrypted[11] = encrypted[2] ^ encrypted[8];       // Step 12: Step 3 ^ Step 9
	encrypted[12] = encrypted[1] ^ encrypted[9];       // Step 13: Step 2 ^ Step 10
	encrypted[13] = encrypted[3] ^ encrypted[9];       // Step 14: Step 4 ^ Step 10

	text[0] = encrypted[10];
	text[1] = encrypted[11];
	text[2] = encrypted[12];
	text[3] = encrypted[13];
}


// ***********************************************
// *             fifthRound function             *
// ***********************************************
// Complete the fifth round which consists of 4
// steps that use addition modulo and multiplication
// modulo. Steps 1, 2, 3, and 4 will become the
// updated encrypted text.
void fifthRound(unsigned int text[MAXTEXTSIZE], unsigned int key[MAXKEYSIZE])
{
	// Create an array to store the values from each step
	unsigned int encrypted[MAXARRAYSIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	// Perform steps
	encrypted[0] = (text[0] * key[0]) % 17; // Step 1: X1 * K7
	encrypted[1] = (text[1] + key[0]) % 16; // Step 2: X2 + K8
	encrypted[2] = (text[2] + key[2]) % 16; // Step 3: X3 + K3
	encrypted[3] = (text[3] * key[3]) % 17; // Step 4: X4 * K4

	text[0] = encrypted[0];
	text[1] = encrypted[1];
	text[2] = encrypted[2];
	text[3] = encrypted[3];
}