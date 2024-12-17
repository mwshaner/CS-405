// BufferOverflow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iomanip>
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

int main()
{
	std::cout << "Buffer Overflow Example" << std::endl;

	// TODO: The user can type more than 20 characters and overflow the buffer, resulting in account_number being replaced -
	//  even though it is a constant and the compiler buffer overflow checks are on.
	//  You need to modify this method to prevent buffer overflow without changing the account_number
	//  variable, and its position in the declaration. It must always be directly before the variable used for input.
	//  You must notify the user if they entered too much data.

	/*
	 ---------------------------------------CHANGES--------------------------------------------------------------

	     Swapped out the char array for a std::string. The string class in C++ dynamically allocates memory,
		 thus it is not possible to generate a buffer overflow or string truncation as the user_input
		 variable will always allocate enough space to store whatever has been input by the user.
	-------------------------------------------------------------------------------------------------------------
	*/
	const std::string account_number = "CharlieBrown42";
	std::string user_input;

	// Even though buffer overflow is not possible we still check for input greater than 20 characters
	try 
	{
		std::cout << "Enter a value: ";
		std::cin >> user_input;

		// Possible buffer overflow attempt or just people making a mistake, 
		// either way notify the user that input must be less than 20 characters
		if (user_input.length() > 20)
		{
			throw std::length_error("\n[ERROR]: Account Numbers Must be less than 20 characters \n");
		}
	}
	catch (std::length_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (...)
	{
		std::cout << "Unkown Error occured" << '\n';
	}

	std::cout << '\n' << "You entered : " << user_input << '\n';
	std::cout << "Account Number = " << account_number << '\n';

	std::make_unique<int>();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu