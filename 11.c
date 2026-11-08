#include <stdio.h>
#include <math.h>








int maxArea(int* height, int heightSize) {
    int n = heightSize;
    int max = 0;
    int left = 0;
    int right = n - 1;

    while (left < right) {
        int h = fmin(height[left], height[right]);
        int w = right - left;
        int area = h * w;
        max = fmax(max, area);

        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }

    return max;
}