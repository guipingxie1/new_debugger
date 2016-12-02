int get_sum(int a, int b) {
    return a + b;
}
 
void function(int a, int b) {
    int k = 0;
    k = get_sum(a, b);
    
    for (int j = 0; j < 10; ++j)
        ++k;
    
    int i = k >> 1;
} 

int main() {
    int a = 4;
    int b = 6;
    a = 5;
    b = 5;
    int c = a + b;
    int d = c -= 2;    
    return 0;
}
