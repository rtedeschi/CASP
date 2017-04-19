#include "string"

using namespace std;

void display(string);
void negative(int i);
void positive(int i);
void say_something(string = "Hi");

int main(int a, bool b) {

    int c = a + b;

    bool d = a || b;

    for (int i = 0; i < 10; i++) {
        
        c = c + i;
        display("" + c);

    }

    display("Value: " + c);
    display("" + a);
    display("" + b);

    if (d) {
        display("" + d);
    } else {

        if (a < 0)
            negative(a);
        else if (a > 0)
            positive(a);
        else
            say_something();

    }

    return 0;
    
}

void display(string s) {
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