# include <stdio.h>
#include<string.h>
#include<stdlib.h>
int check(char*s,int left,int right);
int*seek(char*s,int left,int right,int*result);
char* longestPalindrome(char* s);
int main(){
    printf("%s\n",longestPalindrome("abb"));
    getchar();
    return 0;
}
int check(char*s,int left,int right){
    if(s[left]==s[right]&&right==left+2) return 1;
    else if(s[left]==s[right]&&right==left+1) return 1;
    else return 0;
    }


int*seek(char*s,int left,int right,int*result){
    while(left>=0 && right<(int)strlen(s) && s[left]==s[right]){
        left--;
        right++;
    }
    result[0] = left+1;
    result[1] = right-1;
    return result;
}


char* longestPalindrome(char* s) {
    int n=(int)strlen(s);
    int left=0,right1=1,right2=2,i1=0,i2=0,j1=0,j2=0;
    int len1=0,len2=0;
    int*result=(int*)malloc(sizeof(int)*2);
    while(right2<n||right1<n){
        if(check(s,left,right1)){
            result=seek(s,left,right1,result);
            if(result[1]-result[0]+1>len1){
                len1=result[1]-result[0]+1;
            i1=result[0];
            j1=result[1];}
        }if(right2<n){
        if(check(s,left,right2)){
            result=seek(s,left,right2,result);
            if(result[1]-result[0]+1>len2){
                len2=result[1]-result[0]+1;
            i2=result[0];
            j2=result[1];}
        }}
        
            left++;
            right1++;
            right2++;
        
    }if(i1==0&&j1==0&&i2==0&&j2==0){
           char*res=(char*)malloc(sizeof(char)*(2));
            res[0]=s[0];
            res[1]='\0';
            return res;
        }
    if(n>2){
    if(len1>len2){
        char*res=(char*)malloc(sizeof(char)*(len1+1));
        for(int i=i1;i<j1+1;i++){
            res[i-i1]=s[i];
        }
        res[len1]='\0';
        return res;
    }
    else{
        char*res=(char*)malloc(sizeof(char)*(len2+1));
        for(int i=i2;i<j2+1;i++){
            res[i-i2]=s[i];
        }
        res[len2]='\0';
        return res;
    }
}else if(n==2){
        if(s[0]==s[1]){
            char*res=(char*)malloc(sizeof(char)*(3));
            res[0]=s[0];
            res[1]=s[1];
            res[2]='\0';
            return res;
        }else {
            char*res=(char*)malloc(sizeof(char)*(2));
            res[0]=s[0];
            res[1]='\0';
            return res;
        }
} else return s;
}
