#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_NAME_LEN 50

typedef struct {
    char writer[MAX_NAME_LEN];    
    char book_name[MAX_NAME_LEN];  
    char company[MAX_NAME_LEN];    
    int pub_year;                 
    int page_count;               
} LibraryItem;

// положительное число, если first > second, отрицательное — если меньше, 0 — равны
int compare_items(const LibraryItem* first, const LibraryItem* second, int order) {
    int result = strcmp(first->writer, second->writer); 
    if (!result) result = strcmp(first->book_name, second->book_name); 
    if (!result) result = strcmp(first->company, second->company);
    if (!result) result = first->pub_year - second->pub_year;      
    if (!result) result = first->page_count - second->page_count;   
    return order ? result : -result;
}


void sort_by_insertion(LibraryItem* collection, int count, int order) {
    for (int current = 1; current < count; current++) {
        LibraryItem current_item = collection[current];
        int position = current - 1;

        while (position >= 0 && compare_items(&collection[position], &current_item, order) > 0) {
            collection[position + 1] = collection[position];
            position--;
        }
        collection[position + 1] = current_item;
    }
}

/**
 * Шейкерная сортировка (сортировка перемешиванием).
 */
void cocktail_sort(LibraryItem* collection, int count, int order) {
    int start = 0, end = count - 1;
    char swapped = 1;

    while (swapped&& start < end) {
        swapped = 0;

        // Проход слева направо
        for (int i = start; i < end; i++) {
            if (compare_items(&collection[i], &collection[i + 1], order) > 0) {
                LibraryItem temp = collection[i];
                collection[i] = collection[i + 1];
                collection[i + 1] = temp;
                swapped = 1;
            }
        }
        end--;

        // Проход справа налево
        for (int i = end; i > start; i--) {
            if (compare_items(&collection[i - 1], &collection[i], order) > 0) {
                LibraryItem temp = collection[i];
                collection[i] = collection[i - 1];
                collection[i - 1] = temp;
                swapped = 1;
            }
        }
        start++;
    }
}

/**
 * Быстрая сортировка (QuickSort).
*/
void partition_and_sort(LibraryItem* collection, int left, int right, int order) {
    if (left >= right) return;

    // Выбор опорного элемента (медиана)
    LibraryItem pivot = collection[(left + right) / 2];
    int i = left, j = right;

    // Разделение массива
    while (i <= j) {
        while (compare_items(&collection[i], &pivot, order) < 0) i++;
        while (compare_items(&collection[j], &pivot, order) > 0) j--;

        if (i <= j) {
            LibraryItem temp = collection[i];
            collection[i] = collection[j];
            collection[j] = temp;
            i++;
            j--;
        }
    }

    // Рекурсивная сортировка подмассивов
    partition_and_sort(collection, left, j, order);
    partition_and_sort(collection, i, right, order);
}

void generate_test_data(LibraryItem* items, int count) {
    const char* writers[] = { "Orwell", "Hemingway", "Fitzgerald", "Steinbeck" };
    const char* titles[] = { "1984", "Old Man", "Gatsby", "Grapes" };
    const char* companies[] = { "Penguin", "Simon", "Harper", "Random" };

    for (int i = 0; i < count; i++) {
        strncpy(items[i].writer, writers[rand() % 4], MAX_NAME_LEN - 1);
        strncpy(items[i].book_name, titles[rand() % 4], MAX_NAME_LEN - 1);
        strncpy(items[i].company, companies[rand() % 4], MAX_NAME_LEN - 1);

        items[i].writer[MAX_NAME_LEN - 1] = '\0';
        items[i].book_name[MAX_NAME_LEN - 1] = '\0';
        items[i].company[MAX_NAME_LEN - 1] = '\0';

        items[i].pub_year = 1900 + rand() % 120;  // Год от 1900 до 2019
        items[i].page_count = 150 + rand() % 500; // Страницы от 150 до 649
    }
}


void show_options() {
    printf("\nAvailable sorting methods:\n");
    printf("[1] Insertion method\n");
    printf("[2] Shaker method\n");
    printf("[3] Quick method\n");
    printf("Choose method (1-3): ");
}


void run_performance_test(int method, int order) {
    const int test_sizes[7] = { 100, 1000, 10000, 50000, 100000, 200000, 300000 };

    // Открытие файла для записи
    FILE* file = fopen("results.csv", "a");  // 'a' — добавление к существующему или создание нового
    if (!file) {
        printf("Failed to open file for writing.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "method,order,size,time\n");
    }

    for (int i = 0; i < 7; i++) {
        int current_size = test_sizes[i];
        LibraryItem* test_set = (LibraryItem*)malloc(current_size * sizeof(LibraryItem));

        if (!test_set) {
            printf("Memory allocation failed for size %d\n", current_size);
            continue;
        }

        generate_test_data(test_set, current_size);

        clock_t begin = clock();

        switch (method) {
        case 1: sort_by_insertion(test_set, current_size, order); break;
        case 2: cocktail_sort(test_set, current_size, order); break;
        case 3: partition_and_sort(test_set, 0, current_size - 1, order); break;
        default:
            printf("Invalid method\n");
            free(test_set);
            fclose(file);
            return;
        }

        clock_t end = clock();
        double elapsed = (double)(end - begin) / CLOCKS_PER_SEC;


        printf("Elements: %7d - Time: %.6f seconds\n", current_size, elapsed);


        fprintf(file, "%d,%d,%d,%.6f\n", method, order, current_size, elapsed);

        free(test_set);
    }

    fclose(file);
}


int main() {
    srand((unsigned)time(NULL));  

    int selected_method, sort_order;

    show_options();
    if (scanf("%d", &selected_method) != 1 || selected_method < 1 || selected_method > 3) {
        printf("Invalid selection\n");
        return 1;
    }

    printf("Sort direction (1 - Ascending, 0 - Descending): ");
    if (scanf("%d", &sort_order) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    printf("\nStarting performance measurements...\n");
    
    run_performance_test(selected_method, sort_order);

    return 0;
}