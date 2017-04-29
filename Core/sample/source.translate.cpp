int factorial(int n) {

	int result = 1;
	for (int i = 2; i <= n; i++) {
		result *= i;
	}
	return result;
}

void display(string val) {

	some_display_function(val);

}

void main(int argc, char** argv) {
	
	int f = factorial(5);
	
	string val = to_string(f);

	display(val);

}