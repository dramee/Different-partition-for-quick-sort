#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void swap(long* i, long* j) {
    long tmp = *i;
    *i = *j;
    *j = tmp;
}



long* hoare_partition(long* first, long* last) {
    assert(first <= last);
    if (last - first < 2)
        return first; // nothing interesting to do
    --last;
    if (*first > *last)
        swap(first, last);
    long* pivot_pos = first;
    long pivot = *pivot_pos;
    for (;;) {
        ++first;
        long f = *first;
        while (f < pivot)
            f = *++first;
        long l = *last;
        while (pivot < l)
            l = *--last;
        if (first >= last)
            break;
        *first = l;
        *last = f;
        --last;
    }
    --first;
    swap(first, pivot_pos);
    return first;
}

long* lomuto_partition(long* first, long* last) {
    assert(first <= last);
    if (last - first < 2)
        return first; // nothing interesting to do
    --last;
    if (*first > *last)
        swap(first, last);
    long* pivot_pos = first;
    long pivot = *first;
    // Prelude: position first (the write head) on the first element
    // larger than the pivot.
    do {
        ++first;
    } while (*first < pivot);
    assert(first <= last);
    // Main course.
    for (long* read = first + 1; read < last; ++read) {
        long x = *read;
        if (x < pivot) {
            *read = *first;
            *first = x;
            ++first;
        }
    }
    // Put the pivot where it belongs.
    assert(*first >= pivot);
    --first;
    *pivot_pos = *first;
    *first = pivot;
    return first;
}

long* lomuto_partition_branchfree(long* first, long* last) {
//    assert(first <= last);
    if (last - first < 2)
        return first; // nothing interesting to do
    --last;
    if (*first > *last)
        swap(first, last);
    long* pivot_pos = first;
    long pivot = *first;
    do {
        ++first;
//        assert(first <= last);
    } while (*first < pivot);
    for (long* read = first + 1; read < last; ++read) {
        long x = *read;
        int smaller = -(x < pivot);
        size_t delta = smaller & (read - first);
        first[delta] = *first;
        read[-delta] = x;
        first -= smaller;
    }
//    assert(*first >= pivot);
    --first;
    *pivot_pos = *first;
    *first = pivot;
    return first;
}

void quickSortLomuto(long* low, long* high) {
    if (low < high) {
        long* pi = lomuto_partition( low, high);
        quickSortLomuto(low, pi - 1);
        quickSortLomuto( pi + 1, high);
    }
}

void quickSortHoare(long* low, long* high) {
    if (low < high) {
        long* pi = hoare_partition( low, high);
        quickSortHoare(low, pi - 1);
        quickSortHoare( pi + 1, high);
    }
}

void quickSortLomutoBranchFree(long* low, long* high){
    if (low < high) {
        long* pi = lomuto_partition_branchfree( low, high);
        quickSortLomutoBranchFree(low, pi);
        quickSortLomutoBranchFree( pi + 1, high);
    }
}

// Function to print the array
void printArray(long arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%ld ", arr[i]);
    printf("\n");
}

void arrCopy(const long* arr1, size_t size, long* arr2) {
    for (size_t i = 0; i <= size; i++) {
        arr2[i] = arr1[i];
    }
}


void shuffle ( long arr[], long n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );

    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);

        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

// Driver program
int main() {
    for (size_t i = 10000000; i < 100000000; i = i + 1000) {
        printf("%zu", i);
        printf("%c", '\n');
        long* arr = (long*) malloc(sizeof(long) * i);
        for (size_t j = 0; j < i; j++){
            arr[j] = (long)j;
        }
        shuffle(arr, i);
//        printArray(arr, i);
//        printf("%c", '\n');
        long* arrCpy = (long*) malloc(sizeof(long) * i);
        arrCopy(arr, i, arrCpy);
        clock_t start1 = clock();
        quickSortHoare(arrCpy, arrCpy + i);
        clock_t end1 = clock();
//        printArray(arrCpy, i);
//        printf("%c", '\n');
        float hoareTime = end1 - start1;
        printf("%.100f\n", hoareTime);
        arrCopy(arr, i, arrCpy);
        clock_t start2 = clock();
        quickSortLomuto(arrCpy, arrCpy + i);
//        printf("%c", '\n');
//        printArray(arrCpy, i);
//        printf("%c", '\n');
        clock_t end2 = clock();
        float lomutoTime = end2 - start2;
        printf("%.100f\n", lomutoTime);
        clock_t start3 = clock();
        quickSortLomutoBranchFree(arr, arr + i);
        clock_t end3 = clock();
        float lomutoBFTime = end3 - start3;
        printf("%.100f\n\n", lomutoBFTime);;
        free(arr);
        free(arrCpy);
    }
    return 0;
}