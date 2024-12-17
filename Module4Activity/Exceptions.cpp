// Exceptions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept>


/*
    -------------------Custom application_error exception type--------------------
*/
class application_error : public std::exception
{
private:
    std::string message;

public:
    application_error(const char* msg)
        : message(msg)
    {

    }

    const char* what() const throw()
    {
        return message.c_str();
    }


};

bool do_even_more_custom_application_logic()
{
    std::cout << "Running Even More Custom Application Logic." << std::endl;

    // throw logic error exception
    throw std::logic_error("[ERROR]: Exception occured in function: do_even_more_custom_application_logic()");

    return true;
}
void do_custom_application_logic()
{
    std::cout << "Running Custom Application Logic." << std::endl;

    /*
    --------------Try and catch block to catch any exceptions thrown by do_even_more_custom_application_logic()-------------
    */
    try 
    {
        if (do_even_more_custom_application_logic())
        {
            std::cout << "Even More Custom Application Logic Succeeded." << std::endl;
        }
    } 
    catch (const std::exception& e) 
    {
        std::cout << e.what() << '\n';
    }

    std::cout << "Leaving Custom Application Logic." << std::endl;

    // throw the custom application error exception
    throw application_error("[ERROR]: The application encountered an error in function: do_custom_application_logic()");

}

float divide(float num, float den)
{
    // If the denominator is zero, throw a domain_error exception
    if (den == 0)
    {
        throw std::domain_error("[ERROR]: Division by zero is not defined! Exception thrown in function: divide()");
    }
    else
    {
        return (num / den);
    } 
}

void do_division() noexcept
{
    float numerator = 10.0f;
    float denominator = 0;

    /*
    -----------Try to divide by zero and catch the domain_error exception-------------
    */
    try
    {
        auto result = divide(numerator, denominator);
        std::cout << "divide(" << numerator << ", " << denominator << ") = " << result << std::endl;
    } 
    catch (const std::domain_error& e)
    {
        std::cout << e.what() << '\n';
    }

}

int main()
{
    /*
    -----------Wrap the entire main code in a try block and catch the relevant exceptions-------------
    */
    try
    {
        std::cout << "Exceptions Tests!" << std::endl;
        do_division();
        do_custom_application_logic();
    }
    catch (const application_error& e)
    {
        std::cout << e.what();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
    catch (...)
    {
        std::cout << "[ERROR] Unkown exception has occured";
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu