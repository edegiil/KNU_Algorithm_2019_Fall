#include "sort.h"

void show_list(int list[], unsigned int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", list[i]);
  }
  printf("\n");
}

void merge(int list[], unsigned int left, unsigned int right) {
  unsigned int mid = (left + right) / 2;
  int i, j;

  int temp_left[mid - left + 1], temp_right[right - mid];
  printf("left: %d, mid: %d, right: %d\n\n", left, mid, right);

  // copy data to temp array
  printf("temp left : ");
  for (i = 0; i < mid - left + 1; i++) {
    temp_left[i] = list[left + i];
    printf("%d ", temp_left[i]);
  }
  printf("\ntemp right : ");
  for (j = 0; j < right - mid; j++) {
    temp_right[j] = list[mid + 1 + j];
    printf("%d ", temp_right[j]);
  }
  printf("\n\n");

  // sort
  i = 0;
  j = 0;
  int index = left;
  while (i < mid - left + 1 && j < right - mid) {
    if (temp_left[i] <= temp_right[j]) {
      list[index] = temp_left[i];
      i++;
    } else {
      list[index] = temp_right[j];
      j++;
    }
    printf("%d ", list[index]);
    index++;
  }

  // put rest data
  while (i < mid - left + 1) {
    list[index] = temp_left[i];
    printf("%d ", list[index]);
    i++;
    index++;
  }

  while (j < right - mid) {
    list[index] = temp_right[j];
    printf("%d ", list[index]);
    j++;
    index++;
  }
  printf("\n----------\n");
}

void merge_sort(int list[], unsigned int left, unsigned int right) {
  if (left < right) {
    unsigned int mid = (left + right) / 2;
    // recursive merge sort
    merge_sort(list, left, mid);
    merge_sort(list, mid + 1, right);
    // merge divided array
    merge(list, left, right);
  }
}

int main() {
  printf("Merge Sort with Array\n");

  int data[58] = { 74, 21, 42, 94, 13, 24 ,80, 45, 75, 37, 96, 28, 26, 99 ,97 ,36 ,93 ,18 ,9 ,58 ,64, 47, 67, 86, 0, 35, 10, 34, 50, 39, 53, 43, 73, 89, 85, 81, 44, 33, 61, 90, 15, 71, 30, 63, 88, 70, 14, 6 ,84, 95, 8 ,59, 98, 82, 100, 52, 83, 19 };

  printf("original array : \n");
  show_list(data, sizeof(data)/sizeof(int));

  merge_sort(data, 0, 57);

  printf("merge sorted array : \n");
  show_list(data, sizeof(data)/sizeof(int));

  return 0;
}