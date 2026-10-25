#include <stdio.h>
int main(){
    getchar();
    return 0;
}
double findMedianSortedArrays1(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int n=nums1Size;
    int m=nums2Size;
    int numsum[n+m];
    int i=0;
    int j=0;
    int k=0;
    while(i<n&&j<m){
        if (nums1[i]<nums2[j]) numsum[k++]=nums1[i++];
        else numsum[k++]=nums2[j++];
    }
    while(i<n){
        numsum[k++]=nums1[i++];
    }
    while(j<m){
        numsum[k++]=nums2[j++];
    }
    if((n+m)%2==1){
        return numsum[(int)((n+m)/2)];
    }else{
        return (numsum[(int)((n+m)/2)]+numsum[(int)((n+m)/2)-1])/2.0;
    }
}






int min(int a,int b){
    return a<b?a:b;
}
int kth(int n,int m,int k,int*nums1,int*nums2){
    int p=0;
    int q=0;
    while(p<n&&q<m&&k>1){
     int i=min(n-1,p+k/2-1);
     int j=min(m-1,q+k/2-1);
        if(nums1[i]<nums2[j]){
            k-=(i-p+1);
            p=i+1;
        }else{
            k-=(j-q+1);
            q=j+1;
        }
    }
if(p==n) return nums2[q+k-1];
else if(q==m) return nums1[p+k-1];
else return min(nums1[p],nums2[q]);
       
}
double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size){
    int n=nums1Size;
    int m=nums2Size;
    int k=(n+m+1)/2;
    if((n+m)%2==1) return kth(n,m,k,nums1,nums2);
    else return (kth(n,m,k,nums1,nums2)+kth(n,m,k+1,nums1,nums2))/2.0;
}