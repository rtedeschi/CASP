void fn1(int n) {
	for (int i = 0; i < n; i++) {
		fn2(n);
	}

	fn2();
}
void fn2(int n) {
	for (int i = n - 1; i >= 0; i--) {
		fn3(n);
	}

	display(n);
}
void fn3(int n) {
	int p = 0;
	for (int i = 0; i < n; i += 2) {
		fn4(n);
		p += 5;
	}
}
void fn4(int n) {
	for (int i = n; i > 1; i /= 2) {
		fn5(n);
	}
}
void fn5(int n) {
	for (int i = 1; i < n; i *= 3) {
		display(i);
	}
}

void display(int i) {
	// cout << i << endl;
}