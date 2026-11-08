#include <stdio.h>
#include <stdlib.h>








char* intToRoman(int num) {
    static char* romanNumerals[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    static int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char* result= malloc(16); // Allocate memory for the result 
    int j = 0; // Index for the result string
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            for(const char* p = romanNumerals[i]; *p ; p++) {
              result[j++] = *p; // Append the Roman numeral to the result
            }
            num -= values[i];
        }
    }
    result[j] = '\0'; // Null-terminate the result string
    return result;
}






