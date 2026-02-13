#include "aia_sort.h"
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// TODO: Implement sorting algorithm 
// Adelina - Count Sort
void countSort(vector<int>& arr) {
    int n = arr.size();

    int maxval = 0;
    for (int i = 0; i < n; i++)
        maxval = max(maxval, arr[i]);

    vector<int> cntArr(maxval + 1, 0);

    for (int i = 0; i < n; i++)
        cntArr[arr[i]]++;

    for (int i = 1; i <= maxval; i++)
        cntArr[i] += cntArr[i - 1];

    vector<int> ans(n);
    for (int i = n - 1; i >= 0; i--) {
        ans[cntArr[arr[i]] - 1] = arr[i];
        cntArr[arr[i]]--;
    }
    arr = ans;
}

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
//Andreea

int partition(vector<int> &nums, int low, int high) {
    int pivot = nums[high];   // choose last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (nums[j] <= pivot) {
            i++;
            swap(nums[i], nums[j]);
        }
    }

    swap(nums[i + 1], nums[high]);
    return i + 1;
}

void quickSort(vector<int> &nums, int low, int high) {
    if (low < high) {
        int pi = partition(nums, low, high);
        quickSort(nums, low, pi - 1);
        quickSort(nums, pi + 1, high);
    }
}

void sortArray(vector<int> &nums) {
    if (!nums.empty()) {
        quickSort(nums, 0, nums.size() - 1);
    }
}

// TODO: Implement sorting algorithm
void sort_diana(vector<int> &nums);

// TODO: Implement sorting algorithm
// Magda - Bubble sort
void bubble_sort(vector<int> &nums) {
    int n = nums.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (nums[j] > nums[j + 1]) {
                swap(nums[j], nums[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// TODO: Implement sorting algorithm
//Milena
void radixSort(int array[], int n){
    int *temp=new int[n];

    int minVal=array[0], maxVal=array[0];
    for(int i=1; i<n; i++){
    if(array[i]>maxVal) maxVal=array[i];
    if(array[i]<minVal) minVal=array[i];
    }

    int shift=(minVal<0)? -minVal:0;
    for(int i=0; i<n; i++)
        array[i]+=shift;
    maxVal+=shift;

    for(int exp=1; maxVal/exp>0; exp*=10){
        int count[10]={0};

        for(int i=0; i<n; i++)
            count[(array[i]/exp)%10]++;

        for(int i=0; i<10; i++)
            count[i]+=count[i-1];

        for(int i=n-1; i>=0; i--){
            int d=(array[i]/exp)%10;
            temp[--count[d]]=array[i];
        }

        for(int i=0; i<n; i++)
            array[i]=temp[i];
    }

    for(int i=0; i<n; i++)
        array[i]-=shift;

    delete[] temp;
}
