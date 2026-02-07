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
