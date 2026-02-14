#include "aia_sort.h"
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

// Adelina - Bucket Sort
void insertionSort(vector<int>& bucket) {
    for (int i = 1; i < bucket.size(); ++i) {
        int key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j--;
        }
        bucket[j + 1] = key;
    }
}

void bucketSort(vector<int>& nums) {
    int n = nums.size();
    if (n <= 1) return;

    int max_val = *max_element(nums.begin(), nums.end());

    vector<vector<int>> b(n);

    for (int i = 0; i < n; i++) {
        int bi = (nums[i] * n) / (max_val + 1);
        b[bi].push_back(nums[i]);
    }

    for (int i = 0; i < n; i++) {
        insertionSort(b[i]);
    }

    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j : b[i]) {
            nums[index++] = j;
        }
    }
}


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

void heapify(vector<int>& nums, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && nums[l] > nums[largest]) largest = l;
    if (r < n && nums[r] > nums[largest]) largest = r;

    if (largest != i) {
        swap(nums[i], nums[largest]);
        heapify(nums, n, largest);
    }
}

void heap_sort(vector<int>& nums) {
    int n = nums.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(nums, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(nums[0], nums[i]);
        heapify(nums, i, 0);
    }
}


// Andreea - Quick Sort
// avg O(n log n), worst case O(n^2)

void insertionSortRange(vector<int> &nums, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = nums[i];
        int j = i - 1;
        while (j >= low && nums[j] > key) {
            nums[j + 1] = nums[j];
            j--;
        }
        nums[j + 1] = key;
    }
}

// picks the median of first, middle and last element as pivot
int medianOfThree(vector<int> &nums, int low, int high) {
    int mid = low + (high - low) / 2;
    // sortins these 3 values so the middle one ends up as pivot
    if (nums[low] > nums[mid]) swap(nums[low], nums[mid]);
    if (nums[low] > nums[high]) swap(nums[low], nums[high]);
    if (nums[mid] > nums[high]) swap(nums[mid], nums[high]);
    // put pivot at the end so partition can work with it
    swap(nums[mid], nums[high]);
    return nums[high];
}

// standard Lomuto partition
int partition(vector<int> &nums, int low, int high) {
    int pivot = medianOfThree(nums, low, high);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (nums[j] <= pivot) {
            i++;
            swap(nums[i], nums[j]);
        }
    }

    // puts pivot back where it belongs
    swap(nums[i + 1], nums[high]);
    return i + 1;
}

// recurses on the smaller one
// and loops on the bigger one
void quickSort(vector<int> &nums, int low, int high) {
    while (high - low > 16) {
        int pi = partition(nums, low, high);
        if (pi - low < high - pi) {
            quickSort(nums, low, pi - 1);
            low = pi + 1;
        } else {
            quickSort(nums, pi + 1, high);
            high = pi - 1;
        }
    }
    // small leftover chunks get sorted with insertion sort
    if (low < high) {
        insertionSortRange(nums, low, high);
    }
}

void sort_andreea(vector<int> &nums) {
    if (nums.size() > 1) {
        quickSort(nums, 0, nums.size() - 1);
    }
}

// TODO: Implement sorting algorithm
void sort_diana(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_magda(vector<int> &nums);

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
