#include "NumClass.h"

int int_pow(int base, int exp) {
    int result = 1;
    while (exp != 0) {
        result *= base;
        --exp;
    }
    return result;
}

/**
 * helper function for calculates the number of digits in a given integer n.
 * If n is negative, it returns -1.
 */
int numLen(int n) {
    if (n < 0) return -1;

    int count = 0;
    while (n != 0) {
        count++;
        n /= 10;
    }
    return count;
}

int isArmstrong(int n) {
    if (n <= 0) return FALSE;

    int len = numLen(n);
    int sum = 0;
    int temp = n;

    while (temp != 0) {
        sum += int_pow(temp % 10, len);
        temp /= 10;
    }

    return sum == n ? TRUE : FALSE;
}

int isPalindrome(int n) {
    if (n <= 0) return FALSE;

    int temp = n;
    int revers = 0;

    while (temp != 0) {
        revers = revers * 10 + temp % 10;
        temp /= 10;
    }

    return revers == n ? TRUE : FALSE;
}