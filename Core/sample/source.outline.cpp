/*
    This file is a sample input file for C.A.S.P Outline Module
    It is also a good example for the Translate Module

    This is a valid C++ source file, and can be compiled.
    For demonstration purposes, use this line in a command line at the root of the Core directory to compile:
    
    @cl /c sample/source.outline.cpp /Fo"sample/source.outline.obj" /EHsc

    make sure to comment out do_something_with_input(s), as it is not defined and will throw an error 
*/

#include "string"

using namespace std;

/*
    Display should call another method to show the data to the user
    This prototype demonstrates a parameter declaration with no id or default value
*/
void display(string);  
/*
    Displays the opposite integral value of the input
    This prototype demonstrates a parameter declaration with an id but no default value
*/    
void negative(int i);
/*
    Displays the integral value of the input
    This prototype demonstrates a parameter declaration with an id but no default value
*/    
void positive(int i);
/*
    Displays a string
    This prototype demonstrates a parameter declaration with an id and a default value
*/    
void say_something(string = "Hi");

/*
    Program entry point
*/
int main(int a, bool b) {

    // simple declaration, assignment, and algebraic operation
    int c = a + b;

    // simple declaration, assignment, and boolean operation
    bool d = a || b;

    // for loop from 0 to 9
    for (int i = 0; i < 10; i++) {
        
        // accumulate i to c and display the new value for c
        c = c + i;
        display("" + c);

    }

    // display the final value for c
    display("Value: " + c);
    // display the value for a
    display("" + a);
    // display the value for b
    display("" + b);

    // if d is true, display it
    if (d) {
        display("" + d);
    } else {

        // if a is negative, invoke negative()
        if (a < 0)
            negative(a);
        // if a is positive, invoke positive()
        else if (a > 0)
            positive(a);
        // if a is zero, invoke say_something() using the default parameter
        else
            say_something();

    }

    // return success
    return 0;
    
}

void display(string s) {
    // COMMENT OUT IF COMPILING FOR DEMONSTRATION
    // This line should be replaced with alert(s) or console.log(s) if translating to JavaScript for execution
    do_something_with_input(s);
}

void negative(int i) {
    display("" + (i * -1));
}

void positive(int i) {
    display("" + i);
}

void say_something(string s) {
    display(s);
}