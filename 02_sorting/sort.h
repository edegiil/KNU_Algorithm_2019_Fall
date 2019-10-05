#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
  int data;
  struct node* next;
} NODE;

typedef struct {
  int count;
  NODE* front;
  NODE* rear;
  NODE* pos;
} LIST;
