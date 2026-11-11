/*
 * Лабораторна робота №1
 * Тема: Побудова і аналіз алгоритмів. Аналіз алгоритмів сортування і пошуку.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef int Data;

/* Процедура генерації випадкового масиву */
void generate_random_array(Data *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}

/* 1. (BublSort) — Алгоритм B */
void bubble_sort(Data *a, int n) {
    Data b;
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 0; j < i; j++) {
            if (a[j] > a[j + 1]) {
                b = a[j + 1];
                a[j + 1] = a[j];
                a[j] = b;
            }
        }
    }
}

/* 2. (InsSort) — Алгоритм I */
void insertion_sort(Data *A, int n) {
    int k, l, r, i, j;
    Data b;
    for (k = 0; k < n - 1; k++) {
        b = A[k + 1];
        if (b < A[k]) {
            l = 0;
            r = k;
            do {
                j = (l + r) / 2;
                if (b < A[j]) {
                    r = j;
                } else {
                    l = j + 1;
                }
            } while (l != j);

            for (i = k; i >= j; i--) {
                A[i + 1] = A[i];
            }
            A[j] = b;
        }
    }
}

/* процедура обміну елементів для алгоритму Хоара */
void swap(Data *a, int i, int j) {
    Data b = a[i];
    a[i] = a[j];
    a[j] = b;
}

/* 3. (QuickSort / Hoare) — Алгоритм H */
void hoare(Data *A, int L, int R) {
    if (L < R) {
        Data x = A[(L + R) / 2];
        int left = L;
        int right = R;
        do {
            while (A[left] < x) left++;
            while (A[right] > x) right--;
            if (left <= right) {
                swap(A, left, right);
                left++;
                right--;
            }
        } while (left <= right);
        hoare(A, L, right);
        hoare(A, left, R);
    }
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int sizes[] = {1000, 2000, 3000, 4000, 5000, 6000};
    int num_sizes = (int)(sizeof(sizes) / sizeof(sizes[0]));

    /* Кількість повторень для точного заміру часу через clock() */
    int runs_slow = 20;  
    int runs_fast = 500; 

    srand(42);

    printf("=========================================================================================\n");
    printf(" %6s | %15s | %15s | %15s | %10s | %10s\n",
           "N", "Обмінами B (с)", "Вставками I (с)", "Хоара H (с)", "I / B", "H / B");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        Data *orig = (Data *)malloc((size_t)n * sizeof(Data));
        Data *work = (Data *)malloc((size_t)n * sizeof(Data));

        if (orig == NULL || work == NULL) {
            fprintf(stderr, "Помилка виділення пам'яті!\n");
            free(orig);
            free(work);
            return 1;
        }

        generate_random_array(orig, n);

        /* 1. Замір часу для сортування обмінами (B) */
        clock_t start_b = clock();
        for (int r = 0; r < runs_slow; r++) {
            memcpy(work, orig, (size_t)n * sizeof(Data));
            bubble_sort(work, n);
        }
        clock_t end_b = clock();
        double time_b = ((double)(end_b - start_b) / CLOCKS_PER_SEC) / runs_slow;

        /* 2. Замір часу для сортування вставками (I) */
        clock_t start_i = clock();
        for (int r = 0; r < runs_slow; r++) {
            memcpy(work, orig, (size_t)n * sizeof(Data));
            insertion_sort(work, n);
        }
        clock_t end_i = clock();
        double time_i = ((double)(end_i - start_i) / CLOCKS_PER_SEC) / runs_slow;

        /* 3. Замір часу для швидкого сортування Хоара (H) */
        clock_t start_h = clock();
        for (int r = 0; r < runs_fast; r++) {
            memcpy(work, orig, (size_t)n * sizeof(Data));
            hoare(work, 0, n - 1);
        }
        clock_t end_h = clock();
        double time_h = ((double)(end_h - start_h) / CLOCKS_PER_SEC) / runs_fast;

        double ratio_ib = (time_b > 0.0) ? (time_i / time_b) : 0.0;
        double ratio_hb = (time_b > 0.0) ? (time_h / time_b) : 0.0;

        printf(" %6d | %15.6f | %15.6f | %15.6f | %10.4f | %10.6f\n",
               n, time_b, time_i, time_h, ratio_ib, ratio_hb);

        free(orig);
        free(work);
    }

    printf("=========================================================================================\n");
    return 0;
}