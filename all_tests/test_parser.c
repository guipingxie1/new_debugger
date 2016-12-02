void random_funct(long l, char c, float f) {
	int i = 0;
	++i;
	
	int b = 5;
	i <<= b;
	b *= i;
}

int get(int c) {
    int a = 9;
    int b;
    
    random_funct(0, 0, 0);
    
    b = 0;
    return a + b + c;
}

int main() {
    int a, b, c, d, e = 4;
    a = 6; b = 7; c = 8, d = get(9);
    
    char* parse = "parse";
    char m = 'm';
    char ee = 'e';  
    
    char* hello = "hello world";
	
	double g = 1.05;
	double G = 11.0567;
	
	long l = 1L << 32;
	
	short s = 1 << 14; 

	int sum = 0;
	for (int i = 0; i < 10; ++i)
		sum += i;
    
    float f = 3.5;
    
    if (sum == 10)
        f *= 3.5;
        
    return 0;
}
