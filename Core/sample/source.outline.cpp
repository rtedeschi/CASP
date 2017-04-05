int outline_example(int a, bool b) {

    int c = a + b;

    bool d = a || b;

    for (int i = 0; i < 10; i++) {
        
        c = c + i;

    }

    display(c);
    display(a);
    display(b);
    
}

int display(void a) {

    do_something_with_input(a);

}
