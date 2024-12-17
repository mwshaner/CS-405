// NumericOverflows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>     // std::cout
#include <limits>       // std::numeric_limits
#include <stdexcept>    // std::exception

/// <summary>
/// Template function to abstract away the logic of:
///   start + (increment * steps)
/// </summary>
/// <typeparam name="T">A type that with basic math functions</typeparam>
/// <param name="start">The number to start with</param>
/// <param name="increment">How much to add each step</param>
/// <param name="steps">The number of steps to iterate</param>
/// <returns>start + (increment * steps)</returns>
template <typename T>
T add_numbers(T const& start, T const& increment, unsigned long int const& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        /*
        ------------------------------------------CHANGES---------------------------------------------------------------------------
           To detect whether an overflow is about to happen, the result must be greater than the difference of MAX and the increment
           in the case of signed char, if result = 125, increment = 25, and MAX = 127, then:
           
           25 > 0 TRUE
           125 > 127 - 25 = 102 TRUE
           
           thus adding 25 to 125 will result in an overflow
        ----------------------------------------------------------------------------------------------------------------------------
        */

        // adding a postive number unitl overlfow
        if (increment > 0 && result > std::numeric_limits<T>::max() - increment)
        {
            // if an overflow has occured throw an exception
            throw std::overflow_error("ERROR: Numeric overflow has occured!");
        }
        // adding a negative number which results in subtraction until underflow
        else if (increment < 0 && result < std::numeric_limits<T>::min() - increment)
        {
            throw std::underflow_error("ERROR: Numeric underflow has occured!");
        }
        result += increment;
    }
    return result;
}

/// <summary>
/// Template function to abstract away the logic of:
///   start - (increment * steps)
/// </summary>
/// <typeparam name="T">A type that with basic math functions</typeparam>
/// <param name="start">The number to start with</param>
/// <param name="increment">How much to subtract each step</param>
/// <param name="steps">The number of steps to iterate</param>
/// <returns>start - (increment * steps)</returns>

template <typename T>
T subtract_numbers(T const& start, T const& decrement, unsigned long int const& steps)
{
    T result = start;

    /*
     ------------------------------------------CHANGES------------------------------------------------------------------------
           To detect whether an underflow is about to happen, the result must be smaller than the sum of MIN and the decrement
           in the case of unsigned char, if result = 0, increment = 51, and MIN = 0, then:

           51 > 0 TRUE
           0 < 0 + 25 = 25 TRUE

           thus subtracting 25 from 0 will result in an underflow
    --------------------------------------------------------------------------------------------------------------------------
    */

    for (unsigned long int i = 0; i < steps; ++i) 
    {
        // subtraction of a positive number
        if (decrement > 0 && result < std::numeric_limits<T>::min() + decrement)
        {
            // if an underflow has occured throw an exception
            throw std::underflow_error("ERROR: Numeric underflow has occured!");
        }
        // subtraction of a negative number which results in addition
        else if (decrement < 0 && result > std::numeric_limits<T>::max() + decrement)
        {
            throw std::overflow_error("ERROR: Numeric overflow has occured!");
        }
        result = result -= decrement;
    }
    return result;
}


//  NOTE:
//    You will see the unary ('+') operator used in front of the variables in the test_XXX methods.
//    This forces the output to be a number for cases where cout would assume it is a character. 

template <typename T>
void test_overflow()
{
    // TODO: The add_numbers template function will overflow in the second method call
    //        You need to change the add_numbers method to:
    //        1. Detect when an overflow will happen
    //        2. Prevent it from happening
    //        3. Return the correct value when no overflow happened or
    //        4. Return something to tell test_overflow the addition failed
    //        NOTE: The add_numbers method must remain a template in the NumericFunctions header.
    //
    //        You need to change the test_overflow method to:
    //        1. Detect when an add_numbers failed
    //        2. Inform the user the overflow happened
    //        3. A successful result displays the same result as before you changed the method
    //        NOTE: You cannot change anything between START / END DO NOT CHANGE
    //              The test_overflow method must remain a template in the NumericOverflows source file
    //
    //  There are more than one possible solution to this problem. 
    //  The solution must work for all of the data types used to call test_overflow() in main().

    // START DO NOT CHANGE
    //  how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we add each step (result should be: start + (increment * steps))
    const T increment = std::numeric_limits<T>::max() / steps;
    // whats our starting point
    const T start = 0;

    std::cout << "Overflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE


    /*
    * -------------------------CHANGES-----------------------------------------------
        Try to add the numbers and catch an overflow, underflow, or unkown exception
        and handle it by printing an error message to the console
     --------------------------------------------------------------------------------
    */

    // No overflow test
    T result;
    std::cout << "\tAdding Numbers Without Overflow (" << +start << ", " << +increment << ", " << steps << ") = ";
   
    try
    {
        result = add_numbers<T>(start, increment, steps); // Attempt to add the numbers
        std::cout << +result << '\n'; // Display the result of the addition
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR Unknown Addition Error " << e.what() << '\n';
    }

    // Overflow test
    std::cout << "\tAdding Numbers With Overflow (" << +start << ", " << +increment << ", " << (steps + 1) << ") = ";
   
    try
    {
        result = add_numbers<T>(start, increment, steps + 1); // Attempt to add the numbers
        std::cout << +result; // Display the result of the addition
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR Unknown Addition Error " << e.what() << '\n';
    }
    std::cout << '\n';
}


/*
   -------------------------CHANGES---------------------------------------------------
      Added a new template method specifically for testing signed integer overflow.
      Can be used to check that an underflow exception is generate if add_numbers
      is inadvertanly used to subtract numbers. Currently it is setup to test
      the same overflow case as test_overflow()
   -----------------------------------------------------------------------------------
*/
template <typename T>
void test_overflow_signed()
{
    // TODO: The add_numbers template function will overflow in the second method call
    //        You need to change the add_numbers method to:
    //        1. Detect when an overflow will happen
    //        2. Prevent it from happening
    //        3. Return the correct value when no overflow happened or
    //        4. Return something to tell test_overflow the addition failed
    //        NOTE: The add_numbers method must remain a template in the NumericFunctions header.
    //
    //        You need to change the test_overflow method to:
    //        1. Detect when an add_numbers failed
    //        2. Inform the user the overflow happened
    //        3. A successful result displays the same result as before you changed the method
    //        NOTE: You cannot change anything between START / END DO NOT CHANGE
    //              The test_overflow method must remain a template in the NumericOverflows source file
    //
    //  There are more than one possible solution to this problem. 
    //  The solution must work for all of the data types used to call test_overflow() in main().

    // START DO NOT CHANGE
    //  how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we add each step (result should be: start + (increment * steps))
    const T increment = std::numeric_limits<T>::max() / steps;
    // whats our starting point
    const T start = 0;

    std::cout << "Overflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE


    /*
    * -------------------------CHANGES-----------------------------------------------
        Try to add the numbers and catch an overflow, underflow, or unkown exception
        and handle it by printing an error message to the console
     --------------------------------------------------------------------------------
    */

    // No overflow test
    T result;
    std::cout << "\tAdding Numbers Without Overflow (" << +start << ", " << +increment << ", " << steps << ") = ";

    try
    {
        result = add_numbers<T>(start, increment, steps); // Attempt to add the numbers
        std::cout << +result << '\n'; // Display the result of the addition
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR Unknown Addition Error " << e.what() << '\n';
    }

    // Overflow test
    std::cout << "\tAdding Numbers With Overflow (" << +start << ", " << +increment << ", " << (steps + 1) << ") = ";

    try
    {
        result = add_numbers<T>(start, increment, steps + 1); // Attempt to add the numbers
        std::cout << +result; // Display the result of the addition
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR Unknown Addition Error " << e.what() << '\n';
    }
    std::cout << '\n';
}

template <typename T>
void test_underflow()
{
    // TODO: The subtract_numbers template function will underflow in the second method call
    //        You need to change the subtract_numbers method to:
    //        1. Detect when an underflow will happen
    //        2. Prevent it from happening
    //        3. Return the correct value when no underflow happened or
    //        4. Return something to tell test_underflow the subtraction failed
    //        NOTE: The subtract_numbers method must remain a template in the NumericFunctions header.
    //
    //        You need to change the test_underflow method to:
    //        1. Detect when an subtract_numbers failed
    //        2. Inform the user the underflow happened
    //        3. A successful result displays the same result as before you changed the method
    //        NOTE: You cannot change anything between START / END DO NOT CHANGE
    //              The test_underflow method must remain a template in the NumericOverflows source file
    //
    //  There are more than one possible solution to this problem. 
    //  The solution must work for all of the data types used to call test_overflow() in main().

    // START DO NOT CHANGE
    //  how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we subtract each step (result should be: start - (increment * steps))
    const T decrement = std::numeric_limits<T>::max() / steps;
    // whats our starting point
    const T start = std::numeric_limits<T>::max();

    std::cout << "Underflow Test of Type = " << typeid(T).name() << std::endl;
    // END DO NOT CHANGE

    /*
    * -------------------------CHANGES---------------------------------------------------
        Try to subtract the numbers and catch an overflow, underflow, or unkown exception
        and handle it by printing an error message to the console
     ------------------------------------------------------------------------------------
    */

    // No underflow test
    T result;
    std::cout << "\tSubtracting Numbers Without Overflow (" << +start << ", " << +decrement << ", " << steps << ") = ";
    try
    {
        result = subtract_numbers<T>(start, decrement, steps); // Attempt to subtract the numbers
        std::cout << +result << '\n'; // Display the result of the subtraction
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: Unknown Subtraction Error " << e.what() << '\n';
    }

    // Underflow test
    std::cout << "\tSubtracting Numbers With Overflow (" << +start << ", " << +decrement << ", " << (steps + 1) << ") = ";
    try
    {
        result = subtract_numbers<T>(start, decrement, steps + 1); // Attempt to subtract the numbers
        std::cout << +result << '\n'; // Display the result of the subtraction
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: Unknown Subtraction Error " << e.what() << '\n';
    }
    std::cout << '\n';
}

/*
   -------------------------CHANGES---------------------------------------------------
      Added a new template method specifically for testing signed integer underflow.
      Starts at MIN + 5 and decrements by 1 until steps or steps + 1.
   -----------------------------------------------------------------------------------
*/
template <typename T>
void test_underflow_signed()
{
    //  how many times will we iterate
    const unsigned long int steps = 5;
    // how much will we subtract each step (result should be: start - (increment * steps))
    const T decrement = 1;
    // whats our starting point
    const T start = std::numeric_limits<T>::min() + 5;

    std::cout << "Underflow Test of Type = " << typeid(T).name() << std::endl;
 
    /*
    * -------------------------CHANGES---------------------------------------------------
        Try to subtract the numbers and catch an overflow, underflow, or unkown exception
        and handle it by printing an error message to the console
     ------------------------------------------------------------------------------------
    */

    // No underflow test
    T result;
    std::cout << "\tSubtracting Numbers Without Overflow (" << +start << ", " << +decrement << ", " << steps << ") = ";
    try
    {
        result = subtract_numbers<T>(start, decrement, steps); // Attempt to subtract the numbers
        std::cout << +result << '\n'; // Display the result of the subtraction
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: Unknown Subtraction Error " << e.what() << '\n';
    }

    // Underflow test
    std::cout << "\tSubtracting Numbers With Overflow (" << +start << ", " << +decrement << ", " << (steps + 1) << ") = ";
    try
    {
        result = subtract_numbers<T>(start, decrement, steps + 1); // Attempt to subtract the numbers
        std::cout << +result << '\n'; // Display the result of the subtraction
    }
    catch (const std::overflow_error& e) // Catch the thrown exeption if an overflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::underflow_error& e) // Catch the thrown exeption if an underflow occurs
    {
        std::cout << e.what() << '\n'; // Display the exception message to the user
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: Unknown Subtraction Error " << e.what() << '\n';
    }
    std::cout << '\n';
}

void do_overflow_tests(const std::string& star_line)
{
    std::cout << std::endl << star_line << std::endl;
    std::cout << "*** Running Overflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    /*
   -------------------------CHANGES---------------------------------------------------
      Using new template function test_overflow_signed for signed integers
   -----------------------------------------------------------------------------------
    */

    // Testing C++ primitive times see: https://www.geeksforgeeks.org/c-data-types/
    test_overflow_signed<char>();
    test_overflow_signed<short int>();
    test_overflow_signed<int>();
    test_overflow_signed<long>();
    test_overflow_signed<long long>();

    // unsigned integers
    test_overflow<unsigned char>();
    test_overflow<wchar_t>();
    test_overflow<unsigned short int>();
    test_overflow<unsigned int>();
    test_overflow<unsigned long>();
    test_overflow<unsigned long long>();

    // real numbers
    test_overflow<float>();
    test_overflow<double>();
    test_overflow<long double>();
}

void do_underflow_tests(const std::string& star_line)
{
    std::cout << std::endl << star_line << std::endl;
    std::cout << "*** Running Underflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;


    // Testing C++ primitive times see: https://www.geeksforgeeks.org/c-data-types/
    /*
     -------------------------CHANGES---------------------------------------------------
        Modified the do_underflow_tests function to call the new test_underflow_signed
        function in order to actually test signed integer underflow
     ------------------------------------------------------------------------------------
    */
   
    // signed integers
    test_underflow_signed<char>();
    test_underflow_signed<short int>();
    test_underflow_signed<int>();
    test_underflow_signed<long>();
    test_underflow_signed<long long>();


    // unsigned integers
    test_underflow<unsigned char>();
    test_underflow<wchar_t>();
    test_underflow<unsigned short int>();
    test_underflow<unsigned int>();
    test_underflow<unsigned long>();
    test_underflow<unsigned long long>();

    // real numbers
    test_underflow<float>();
    test_underflow<double>();
    test_underflow<long double>();
}

/// <summary>
/// Entry point into the application
/// </summary>
/// <returns>0 when complete</returns>
int main()
{
    //  create a string of "*" to use in the console
    const std::string star_line = std::string(50, '*');

    std::cout << "Starting Numeric Underflow / Overflow Tests!" << std::endl;

    // run the overflow tests
    do_overflow_tests(star_line);

    // run the underflow tests
    do_underflow_tests(star_line);

    std::cout << std::endl << "All Numeric Underflow / Overflow Tests Complete!" << std::endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu