#include "aia_sort.h"
using namespace std;

// TODO: Implement sorting algorithm
void sort_adelina(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_ana(vector<int> &nums);

void heapify(vector<int> &nums, int n, int i) {
    int largest = i;

    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && nums[l] > nums[largest]) {
        largest = l;
    }

    if (r < nums.size() && nums[r] > nums[largest]) {
        largest = r;
    }
    if (largest != i) {
        swap(nums[largest], nums[i]);
        heapify(nums, n, largest);
    }
}

// TODO: Optimize Heap Sort
void heap_sort(vector<int> &nums) {
    int n = nums.size();

    for (int i = 0; i < n / 2 - 1; i++) {
        heapify(nums, nums.size(), i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(nums[0], nums[i]);
        heapify(nums, i, 0);
    }
}

// TODO: Implement sorting algorithm
void sort_andreea(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_diana(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_magda(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_milena(vector<int> &nums);
