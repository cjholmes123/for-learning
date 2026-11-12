#include <stdio.h>
#include <stdlib.h>


static int cmp(const void* a, const void* b) {
    int num1 = *(int*)a;
    int num2 = *(int*)b;
    return(num1>num2) - (num1<num2);
}

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes){
    int i=0;
    int j=0;
    int k=0;
    int l=0;
    int**res=(int**)malloc(sizeof(int*)*numsSize*numsSize/2);
    qsort(nums, numsSize, sizeof(int), cmp);
    for(i=0;i<numsSize&&nums[i]<=0;i++){
        if(i>0&&nums[i]==nums[i-1]) continue;
        for(j=i+1,k=numsSize-1;j<k;){
            int sum=nums[i]+nums[j]+nums[k];
        if(sum==0){
            res[l]=(int*)malloc(sizeof(int)*3);
            res[l][0]=nums[i];
                res[l][1]=nums[j];
                res[l][2]=nums[k];
                l++;
                k--;
                j++;
                while(j<k&&j>0&&nums[j]==nums[j-1]) j++;
        while(j<k&&k<numsSize-1&&nums[k]==nums[k+1]) k--;
            }
        else if(sum<0) j++;
        else k--;
        
        }
    }   
    *returnSize=l;
    *returnColumnSizes=(int*)malloc(sizeof(int)*l);
    for(int m=0;m<l;m++){
        (*returnColumnSizes)[m]=3;
    }return res;
}





