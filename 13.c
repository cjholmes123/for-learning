





int search(char* s, char c) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == c) {
            return i; // Return the index of the character if found
        }
    }
    return -1; // Return -1 if the character is not found
}
int romanToInt(char* s) {
    static char romanNumerals[]= {'M', 'D',  'C',  'L', 'X',  'V',  'I'};
    static int values[]= {1000, 500,  100, 50, 10,  5, 1};
    int result = 0; 
    for(int i=0;s[i]!='\0';i++){
        for(int j=0;j<7;j++){
            if(s[i]==romanNumerals[j]&&j!=0&&s[i+1]!='\0'&&search(romanNumerals,s[i+1])<j){
                result-=values[j];
                break;
            }
            else if(s[i]==romanNumerals[j]){
                result+=values[j];
                break;
            }
    }
}return result;
}




