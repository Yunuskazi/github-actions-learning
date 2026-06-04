#include <stdio.h>

int main() {
    printf("Hello from C Programming!\n");
    printf("GitHub Actions is compiling and running this C code!\n");
    
    int sum = 0;
    for (int i = 1; i <= 10; i++) {
        sum += i;
    }
    printf("Sum of numbers 1 to 10: %d\n", sum);
    
    return 0;
}
