int analyze_example(int n) {

    for (int i = 0; i < n; i++) {
        some_function(n);
    }

}

int some_function(int n) {
    int p;

    for (int i = n; i > 1; i = i / 10) {
        p = p + another_function();
    }

    return p;
}