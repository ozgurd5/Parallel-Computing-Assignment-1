#ifndef PARALLEL_COMPUTING_ASSIGNMENT_COMMON_H
#define PARALLEL_COMPUTING_ASSIGNMENT_COMMON_H

#define N 1000000

double get_time();
int* copy_array(const int* source, const int size);
int is_sorted(const int* arr, const int n);

int* bubble_sort(const int* original, const int n);
int* selection_sort(const int* original, const int n);

void quick_sort_recursive(int* arr, const int low, const int high);
int* quick_sort(const int* original, int n);

void merge(int* arr, const int l, const int m, const int r);
void merge_sort_recursive(int* arr, const int l, const int r);
int* merge_sort(const int* original, const int n);

void bitonic_merge(int* arr, const int low, const int cnt, const int dir);
void bitonic_sort_recursive(int* arr, const int low, const int cnt, const int dir);
int* bitonic_sort(const int* original, const int n);

#endif //PARALLEL_COMPUTING_ASSIGNMENT_COMMON_H
