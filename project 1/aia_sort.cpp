#include "aia_sort.h"
#include <future>
#include <thread>
#include <bits/stdc++.h>
#include <math.h>
using namespace std;

// Adelina - IntroSort
// Swap two integers
// A utility function to swap the values pointed by the two pointers
void swapValue(int *a, int *b)
{
    int *temp = a;
    a = b;
    b = temp;
    return;
}

/* Function to sort an array using insertion sort*/
void InsertionSort(int arr[], int *begin, int *end)
{
    // Get the left and the right index of the subarray to be sorted
    int left = begin - arr;
    int right = end - arr;

    for (int i = left+1; i <= right; i++)
    {
        int key = arr[i];
        int j = i-1;

        /* Move elements of arr[0..i-1], that are
           greater than key, to one position ahead
           of their current position */
        while (j >= left && arr[j] > key)
        {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
    }

    return;
}

// A function to partition the array and return the partition point
int* Partition(int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot)
        {
            // increment index of smaller element
            i++;

            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (arr + i + 1);
}


// A function that find the middle of the  values pointed by the pointers
// a, b, c and return that pointer
int *MedianOfThree(int * a, int * b, int * c)
{
    if (*a < *b && *b < *c)
        return (b);

    if (*a < *c && *c <= *b)
        return (c);

    if (*b <= *a && *a < *c)
        return (a);

    if (*b < *c && *c <= *a)
        return (c);

    if (*c <= *a && *a < *b)
        return (a);

    if (*c <= *b && *b <= *a)
        return (b);
}

// A Utility function to perform intro sort
void IntrosortUtil(int arr[], int * begin,
                   int * end, int depthLimit)
{
    // Count the number of elements
    int size = end - begin;

    // If partition size is low then do insertion sort
    if (size < 16)
    {
        InsertionSort(arr, begin, end);
        return;
    }

    // If the depth is zero use heapsort
    if (depthLimit == 0)
    {
        make_heap(begin, end+1);
        sort_heap(begin, end+1);
        return;
    }

    // Else use a median-of-three concept to find a good pivot
    int * pivot = MedianOfThree(begin, begin+size/2, end);

    // Swap the values pointed by the two pointers
    swapValue(pivot, end);

    // Perform Quick Sort
    int * partitionPoint = Partition(arr, begin-arr, end-arr);
    IntrosortUtil(arr, begin, partitionPoint-1, depthLimit - 1);
    IntrosortUtil(arr, partitionPoint + 1, end, depthLimit - 1);

    return;
}

/* Implementation of introsort*/
void introsort(vector<int> &nums)
{
    int n = sizeof(nums) / sizeof(nums[0]);
    int depthLimit = 2 * log(n);

    // Perform a recursive Introsort
    IntrosortUtil(nums, 0, n, depthLimit);

    return;
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

// Magda
// Bubble Sort - sorts the vector in ascending order
void bubble_sort(vector<int> &nums) {

    int n = nums.size();   // number of elements
    bool swapped;          // checks if a swap happened

    // Repeat passes through the array
    for (int i = 0; i < n - 1; i++) {

        swapped = false;   // reset for this pass

        // Compare adjacent elements
        for (int j = 0; j < n - i - 1; j++) {

            // Swap if they are in the wrong order
            if (nums[j] > nums[j + 1]) {
                swap(nums[j], nums[j + 1]);
                swapped = true;
            }
        }

        // Stop early if no swaps were made
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
