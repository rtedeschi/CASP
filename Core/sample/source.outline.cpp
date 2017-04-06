int outline_example(int a, bool b) {

    int c = a + b;

    bool d = a || b;

    for (int i = 0; i < 10; i++) {
        
        c = c + i;

    }

    display(c);
    display(a);
    display(b);

    if (d) {
        do_this();
    }

    if (!d)
        do_this();
    else if (c)
        do_that();
    else
        noop();
    
}

int display() {

    do_something_with_input(a);

}