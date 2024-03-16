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

int isArmstrongHelper(int n, int count) {
    if (n == 0) return 0;
    return int_pow(n % 10, count) + isArmstrongHelper(n / 10, count);
}

/**
 * check if n is an Armstrong number in recursive way
 */
int isArmstrong(int n) {
    if (n <= 0) return FALSE;

    int len = numLen(n);
    return isArmstrongHelper(n, len) == n ? TRUE : FALSE;
}

int isPalindromeHelper(int n, int reversed) {
    if (n == 0) return reversed;
    return isPalindromeHelper(n / 10, reversed * 10 + n % 10);
}

int isPalindrome(int n) {
    if (n <= 0) return FALSE;
    return isPalindromeHelper(n, 0) == n ? TRUE : FALSE;
}