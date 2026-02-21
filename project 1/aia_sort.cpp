#include "aia_sort.h"
using namespace std;

// TODO: Implement sorting algorithm
void sort_adelina(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_ana(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_anastasia(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_andreea(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_diana(vector<int> &nums);

// TODO: Implement sorting algorithm
void sort_magda(vector<int> &nums);

// TODO: Implement sorting algorithm
//Milena
void radixSort(int array[], int n){
    //Allocate temporary array for sorting by digits
    int *temp=new int[n];

    //Find minimum and maximum values in the array
    int minVal=array[0], maxVal=array[0];
    for(int i=1; i<n; i++){
    if(array[i]>maxVal) maxVal=array[i];
    if(array[i]<minVal) minVal=array[i];
    }

    //Shift values if there are negative numbers
    //This makes all numbers non-negative for radix sort
    int shift=(minVal<0)? -minVal:0;
    for(int i=0; i<n; i++)
        array[i]+=shift;
    maxVal+=shift;

    //Perform counting sort for each digi
    for(int exp=1; maxVal/exp>0; exp*=10){
        int count[10]={0};

        //Count occurrences of each digit at current position
        for(int i=0; i<n; i++)
            count[(array[i]/exp)%10]++;

        //Convert count[] to prefix sum (cumulative count)
        for(int i=0; i<10; i++)
            count[i]+=count[i-1];

        //Build the sorted array for this digit (stable sort)
        for(int i=n-1; i>=0; i--){
            int d=(array[i]/exp)%10; //Current digit
            temp[--count[d]]=array[i];
        }

        //Copy sorted values back to original array
        for(int i=0; i<n; i++)
            array[i]=temp[i];
    }

    //Undo the shift to restore original values
    for(int i=0; i<n; i++)
        array[i]-=shift;

    //Free dynamically allocated memory
    delete[] temp;
}
