#include <stdio.h>
#include "NumClass.h"

int isPrime(int n) {
    // for some reason, in this assignment, 1 is a prime number. but in reality, it is not.
    if (n < 1) return FALSE;

    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return FALSE;
        }
    }

    return TRUE;
}

int factorial(int n) {
    if (n < 0) return -1;
    if (n == 0) return 1;

    return n * factorial(n - 1);
}

int isStrong(int n) {
    if (n <= 0) return FALSE;

    int sum = 0;
    int temp = n;
    while (temp != 0) {
        sum += factorial(temp % 10);
        temp /= 10;
    }

    return sum == n ? TRUE : FALSE;
}
