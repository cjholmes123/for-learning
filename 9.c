#include <stdbool.h>






bool isPalindrome(int x) {
    int i=0;
    if(x<0) return false;
    int num[10];
    while(x>0){
        num[i]=x%10;
        x=x/10;
        i++;
    }
    for(int j=0; j<i/2; j++){
        if(num[j]!=num[i-1-j]) return false;
    }
    return true;
}




