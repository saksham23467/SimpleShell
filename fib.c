#include <stdio.h>
#include <stdlib.h>

int fibonacci(int n){
    if(n <= 0){
        return 0;
    }
    else if (n == 1){
        return 1; 
    }
    else{
        int a = 0, b = 1, next;
        for (int i = 2; i <= n; i++) {
            next = a + b; 
            a = b;       
            b = next;    
        }
        return b; 
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return 1; 
    }

    int n = atoi(argv[1]); 

    if(n < 0){
        fprintf(stderr, "Please enter a non-negative integer.\n");
        return 1; 
    }

    int result = fibonacci(n);
    printf("Fibonacci of %d is %d\n", n, result);
    return 0; 
}

