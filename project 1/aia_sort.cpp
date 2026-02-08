#include "aia_sort.h"
#include <vector>
#include <algorithm>
using namespace std;

// TODO: Implement sorting algorithm
void sort_adelina(vector<int> &nums);

// TODO: Implement sorting algorithm
//Ana
const int RUN = 32;

// Insertion Sort for small subarrays
void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

// Merge two sorted runs
void merge(vector<int>& arr, int l, int m, int r) {
    int len1 = m - l + 1, len2 = r - m;
    vector<int> left(len1), right(len2);

    for (int i = 0; i < len1; i++) left[i] = arr[l + i];
    for (int i = 0; i < len2; i++) right[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) arr[k++] = left[i++];
        else arr[k++] = right[j++];
    }
    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];
}

// main Timsort logic
void timSort(vector<int> &nums) {
    int n = nums.size();
    if (n < 2) return;

    // Sort small chunks of the array
    for (int i = 0; i < n; i += RUN) {
        insertionSort(nums, i, min((i + RUN - 1), (n - 1)));
    }

    // Merge chunks iteratively
    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));

            if (mid < right) {
                merge(nums, left, mid, right);
            }
        }
    }
}

// TODO: Implement sorting algorithm
void sort_anastasia(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_andreea(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_diana(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_magda(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_milena(vector<int> &nums);
