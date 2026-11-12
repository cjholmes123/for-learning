#include <string.h>
#include<stdlib.h>
static int min(char** strs, int strsSize){
    int res=0;
    for(int i=0;i<strsSize;i++){
        if(strlen(strs[i])<strlen(strs[res])){
            res=i;
        }
    }
    return res;
}
char* longestCommonPrefix(char** strs, int strsSize) {
    while(strlen(strs[min(strs,strsSize)])!=0){
    char*result=malloc(200);
    int i=0;
    int j=0;
    for(j=0;j<(int)strlen(strs[min(strs,strsSize)]);j++){
        for(i=1;i<strsSize;i++){
            if(strs[i][j]!=strs[0][j]){
                result[j]='\0';
                return result;
            }
        }
                result[j]=strs[0][j]; 
}result[j]='\0';
return result;
}
return "";
}





