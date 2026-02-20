#include "aia_sort.h"
#include <future>
#include <thread>
#include <bits/stdc++.h>
#include <stdio.h>
#include <math.h>
using namespace std;

// Adelina - IntroSort
// Swap two integers
void swapValue(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Insertion sort for small subarrays
void insertionSort(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Heapify function used by HeapSort
void heapify(int arr[], int n, int i, int offset)
{
    int largest = i;
    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[offset + left] > arr[offset + largest])
        largest = left;

    if (right < n && arr[offset + right] > arr[offset + largest])
        largest = right;

    if (largest != i)
    {
        swapValue(&arr[offset + i], &arr[offset + largest]);
        heapify(arr, n, largest, offset);
    }
}

// HeapSort used when recursion depth is exceeded
void heapSort(int arr[], int left, int right)
{
    int n = right - left + 1;

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, left);

    for (int i = n - 1; i > 0; i--)
    {
        swapValue(&arr[left], &arr[left + i]);
        heapify(arr, i, 0, left);
    }
}

// Median-of-three pivot selection
int medianOfThree(int arr[], int a, int b, int c)
{
    if ((arr[a] < arr[b] && arr[b] < arr[c]) ||
        (arr[c] < arr[b] && arr[b] < arr[a]))
        return b;

    if ((arr[b] < arr[a] && arr[a] < arr[c]) ||
        (arr[c] < arr[a] && arr[a] < arr[b]))
        return a;

    return c;
}

// Partition function (QuickSort style)
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swapValue(&arr[i], &arr[j]);
        }
    }

    swapValue(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Recursive Introsort utility
void introsortUtil(int arr[], int left, int right, int depthLimit)
{
    int size = right - left + 1;

    if (size < 16)
    {
        insertionSort(arr, left, right);
        return;
    }

    if (depthLimit == 0)
    {
        heapSort(arr, left, right);
        return;
    }

    int mid = left + size / 2;
    int pivotIndex = medianOfThree(arr, left, mid, right);
    swapValue(&arr[pivotIndex], &arr[right]);

    int p = partition(arr, left, right);

    introsortUtil(arr, left, p - 1, depthLimit - 1);
    introsortUtil(arr, p + 1, right, depthLimit - 1);
}

// Introsort entry function
void introsort(vector<int> &arr)
{
    int n = sizeof(arr) / sizeof(arr[0]);
    int depthLimit = 2 * (int)log2(n);
    introsortUtil(arr, 0, n - 1, depthLimit);
}



//Ana
const int RUN = 32;

// Insertion Sort for small subarrays
void insertionSort(vector<int> &arr, int left, int right) {
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
void merge(vector<int> &arr, int l, int m, int r) {
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

// Anastasia

// Function for putting an element from heap to the right place
void heapify(vector<int> &nums, int n, int i) {
    // Suppose the parent is i
    int largest = i;

    // Get the left and right child of i
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    // Check if the children are smaller than the parent
    // If no, the parent is the child
    if (l < n && nums[l] > nums[largest]) largest = l;
    if (r < n && nums[r] > nums[largest]) largest = r;

    // If the largest is not i
    if (largest != i) {
        // Swap them so they are in the right place
        swap(nums[i], nums[largest]);
        // Check if the parent is in the right place
        heapify(nums, n, largest);
    }
}

// Fuction which implements the heap sorting algorithm
void heap_sort(vector<int> &nums) {
    // Store the size if the vector
    int n = nums.size();

    // Construct the first heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(nums, n, i);
    }

    // Sort by extracting the largest element (the root of the heap)
    for (int i = n - 1; i > 0; i--) {
        swap(nums[0], nums[i]);
        heapify(nums, i, 0);
    }
}

//Andreea

int median_of_three(vector<int> &nums, int l, int r) {
    int m = (l + r) / 2;

    if ((nums[l] <= nums[m] && nums[m] <= nums[r]) || (nums[r] <= nums[m] && nums[m] <= nums[l]))
        return m;
    if ((nums[m] <= nums[l] && nums[l] <= nums[r]) || (nums[r] <= nums[l] && nums[l] <= nums[m]))
        return l;
    return r;
}

int partition(vector<int> &nums, int low, int high) {
    int pivot_index = median_of_three(nums, low, high);
    swap(nums[pivot_index], nums[high]);
    int pivot = nums[high];
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

//Milena
void radixSort(vector<int> &nums) {
    if (nums.empty()) return;

    int maxVal = *max_element(nums.begin(), nums.end());
    int minVal = *min_element(nums.begin(), nums.end());

    int shift = (minVal < 0) ? -minVal : 0;

    for (int &x : nums)
        x += shift;

    maxVal += shift;

    vector<int> temp(nums.size());

    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        int count[10] = {0};

        for (int x : nums)
            count[(x / exp) % 10]++;

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (int i = nums.size() - 1; i >= 0; i--) {
            int d = (nums[i] / exp) % 10;
            temp[--count[d]] = nums[i];
        }

        nums = temp;
    }

    for (int &x : nums)
        x -= shift;
}

constexpr int SYNC_SIZE = 10000;   // Minimum partition size to spawn async

// Median of three (pass by value — int is small)
int middle_of_three(int a, int b, int c)
{
    if (a < b)
    {
        if (b < c) return b;
        if (a < c) return c;
        return a;
    }
    else
    {
        if (a < c) return a;
        if (b < c) return c;
        return b;
    }
}

void qsort_async_util(vector<int>& v, int left, int right)
{
    if (left >= right)
        return;

    int i = left;
    int j = right;

    int mid = left + (right - left) / 2;
    int pivot = middle_of_three(v[left], v[mid], v[right]);

    while (i <= j)
    {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j)
        {
            swap(v[i], v[j]);
            i++;
            j--;
        }
    }

    future<void> future_task;

    // Spawn async only for large partitions
    if (j - left > SYNC_SIZE)
    {
        future_task = async(launch::async,
            [&v, left, j]() {
                qsort_async_util(v, left, j);
            });
    }
    else
    {
        if (left < j)
            qsort_async_util(v, left, j);
    }

    // Process right side in current thread
    if (i < right)
        qsort_async_util(v, i, right);

    if (future_task.valid())
        future_task.wait();
}

void parallel_sort(vector<int>& nums)
{
    if (nums.empty())
        return;

    qsort_async_util(nums, 0, nums.size() - 1);
}