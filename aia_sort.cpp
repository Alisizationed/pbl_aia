#include "aia_sort.h"
using namespace std;

// TODO: Implement sorting algorithm
void sort_adelina(vector<int> &nums){}

// TODO: Implement sorting algorithm
void sort_ana(vector<int> &nums){}

// TODO: Implement sorting algorithm
void sort_anastasia(vector<int> &nums){}

// TODO: Implement sorting algorithm
void sort_andreea(vector<int> &nums){}

// TODO: Implement sorting algorithm
//Diana
void shell_sort(vector<int> &nums) {
    int n = nums.size();

    for(int gap = n / 2; gap > 0; gap /= 2) {
        for(int i = gap; i < n; i ++) {
            int temp = nums[i];
            int j = i;

            while (j >= gap && nums[j - gap] > temp) {
                nums[j] = nums[j - gap];
                j -= gap;
            }
            nums[j] = temp;
        }
    }
}

// TODO: Implement sorting algorithm
void sort_magda(vector<int> &nums){}

// TODO: Implement sorting algorithm
void sort_milena(vector<int> &nums){}

void aia_sort(vector<int> &nums) {
    sort_adelina(nums);
}
