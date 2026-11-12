#include <stdio.h>
#include <stdlib.h>


static int cmp(const void* a, const void* b) {
    int num1 = *(int*)a;
    int num2 = *(int*)b;
    return(num1>num2) - (num1<num2);
}

int threeSumClosest(int* nums, int numsSize, int target) {
     int i=0;
    int j=0;
    int k=0;
    int res=0;
    qsort(nums, numsSize, sizeof(int), cmp);
    int close=abs(nums[0]+nums[1]+nums[2]-target);
    for(i=0;i<numsSize-2;i++){
        for(j=i+1,k=numsSize-1;j<k;){
            int sum=nums[i]+nums[j]+nums[k];
        int l=abs(sum-target);
        if(l<=close){
            close=l;
            res=sum;
        }
        if(sum>target) k--;
        else j++;
    }  
    }return res;
}
