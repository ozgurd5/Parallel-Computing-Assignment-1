#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    const double seconds = tv.tv_sec;
    const double microseconds = tv.tv_usec;
    const double time = seconds + microseconds / 1000000.0;
    return time;
}

int* copy_array(const int* source, const int size)
{
    int* destination = (int*)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
    {
        destination[i] = source[i];
    }

    return destination;
}

int is_sorted(const int* arr, const int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            return 0;
        }
    }

    return 1;
}

int* bubble_sort(const int* original, const int n)
{
    int* arr = copy_array(original, n);

    for (int i = 0; i < n - 1; i++)
    {
        int swapped = 0;

        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                swapped = 1;
            }
        }

        if (swapped == 0)
        {
            break;
        }
    }

    return arr;
}

int* selection_sort(const int* original, const int n)
{
    int* arr = copy_array(original, n);

    for (int i = 0; i < n - 1; i++)
    {
        int min_idx = i;

        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }

        const int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }

    return arr;
}

void quick_sort_recursive(int* arr, const int low, const int high)
{
    if (low < high)
    {
        const int pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                const int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        const int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        const int pi = i + 1;
        quick_sort_recursive(arr, low, pi - 1);
        quick_sort_recursive(arr, pi + 1, high);
    }
}

int* quick_sort(const int* original, int n)
{
    int* arr = copy_array(original, n);
    quick_sort_recursive(arr, 0, n - 1);
    return arr;
}

void merge(int* arr, const int l, const int m, const int r)
{
    const int n1 = m - l + 1;
    const int n2 = r - m;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void merge_sort_recursive(int* arr, const int l, const int r)
{
    if (l < r)
    {
        const int m = l + (r - l) / 2;
        merge_sort_recursive(arr, l, m);
        merge_sort_recursive(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int* merge_sort(const int* original, const int n)
{
    int* arr = copy_array(original, n);
    merge_sort_recursive(arr, 0, n - 1);
    return arr;
}

void bitonic_merge(int* arr, const int low, const int cnt, const int dir)
{
    if (cnt > 1)
    {
        const int k = cnt / 2;

        for (int i = low; i < low + k; i++)
        {
            if (dir == (arr[i] > arr[i + k]))
            {
                const int temp = arr[i];
                arr[i] = arr[i + k];
                arr[i + k] = temp;
            }
        }

        bitonic_merge(arr, low, k, dir);
        bitonic_merge(arr, low + k, k, dir);
    }
}

void bitonic_sort_recursive(int* arr, const int low, const int cnt, const int dir)
{
    if (cnt > 1)
    {
        const int k = cnt / 2;
        bitonic_sort_recursive(arr, low, k, 1);
        bitonic_sort_recursive(arr, low + k, k, 0);
        bitonic_merge(arr, low, cnt, dir);
    }
}

int* bitonic_sort(const int* original, const int n)
{
    int padded_size = 1;

    while (padded_size < n)
    {
        padded_size *= 2;
    }

    int* arr = (int*)malloc(padded_size * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        arr[i] = original[i];
    }

    for (int i = n; i < padded_size; i++)
    {
        arr[i] = INT_MAX;
    }

    bitonic_sort_recursive(arr, 0, padded_size, 1);

    int* final_arr = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        final_arr[i] = arr[i];
    }

    free(arr);

    return final_arr;
}