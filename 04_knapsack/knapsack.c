/*
COMP319 Algorithms, Fall 2019
Assignment4
Knapsack Problem
2015112182 Park Gideok
*/

/* HEADER */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* STRUCTURE DEFINITION */
typedef struct node {
  int weight;
  int value;
  struct node *next;
} NODE;

typedef struct queue {
  int count;
  NODE *front;
  NODE *rear;
} QUEUE;

/* GLOBAL DEFINITION */
#define MAXLINE 1024
#define MAXCAPACITY 1024

/* PROTOTYPES */
void read_file(const char file[], int weight[], int value[], int *maximum, int *item_number);
int getMaxInt(int former, int latter);
double getMaxDouble(double former, double latter);
void recursive_free(NODE *node);
NODE *create_node();
QUEUE *create_queue();
void enqueue(QUEUE *queue, int data, bool type);
void clear_queue(QUEUE* queue);
void pop_queue(FILE *fp, QUEUE *queue, int type);

/* ASSIGNMENTS */
// ASSIGNMENT 1
void knapsack(FILE *fp, int capacity, int weight[], int value[], int item_count) {
  int table[item_count + 1][capacity + 1];
  int i, w;
  int max_value;

  fprintf(fp, "========================\n");
  fprintf(fp, "01. knapsack problem\n");

  for (i = 0; i <= item_count; i++) {
    for (w = 0; w <= capacity; w++) {
      if (i == 0 || w == 0) table[i][w] = 0;
      else if (weight[i - 1] <= w) {
        table[i][w] = getMaxInt(value[i - 1] + table[i - 1][w - weight[i - 1]], table[i - 1][w]);
      }
      else {
        table[i][w] = table[i - 1][w];
      }
    }
  }

  max_value = table[item_count][capacity];
  w--;

  for (i = item_count; i > 0 && max_value > 0; i--) {
    if (max_value != table[i - 1][w]) {
      fprintf(fp, "%d ", i);

      max_value -= value[i - 1];
      w -= weight[i - 1];
    }
  }

  fprintf(fp, "%d\n", table[item_count][capacity]);
}

// ASSIGNMENT 2
void knapsack_one_item_split(FILE *fp, int capacity, int weight[], int value[], int item_count) {
  double table[item_count+ 1][capacity+ 1];
  int i, w, p_number;
  double max_value;
  double max_case = 0;

  fprintf(fp, "========================\n");
  fprintf(fp, "02. knapsack with one item split\n");

  QUEUE *queue = create_queue();

  for (p_number = 1; p_number <= item_count; p_number++) {
    for (i = 0; i <= item_count; i++) {
      double real_weight = i == p_number ? (double)weight[i - 1] / 2 : (double)weight[i - 1];
      double real_value = i == p_number ? (double)value[i - 1] / 2 : (double)value[i - 1];
      
      for (w = 0; w <= capacity; w++) {
        if (i == 0 || w == 0) table[i][w] = 0;
        else if (real_weight <= w) {
          table[i][w] = getMaxDouble(real_value + table[i - 1][w - (int)ceil((double)real_weight)], table[i - 1][w]);
        }
        else {
          table[i][w] = table[i - 1][w];
        }
      }
    }

    max_value = table[item_count][capacity];
    w--;

    if (max_value > max_case) {
      max_case = max_value;
      clear_queue(queue);

      for (i = item_count; i > 0 && max_value > 0; i--) {
        double real_weight = i == p_number ? (double)weight[i - 1] / 2 : (double)weight[i - 1];
        double real_value = i == p_number ? (double)value[i - 1] / 2 : (double)value[i - 1];

        if (max_case != table[i - 1][w]) {
          if (p_number == i) {
            enqueue(queue, i, true);
          } else {
            enqueue(queue, i, false);
          }

          max_value -= real_value;
          w -= (int)ceil(real_weight);
        }
      }
    }
  }

  pop_queue(fp, queue, 2);
  fprintf(fp, "%.1f\n", max_case);
}

// ASSIGNMENT 3
void knapsack_one_item_duplicate(FILE *fp, int capacity, int weight[], int value[], int item_count) {
  int table[item_count + 1][capacity + 1];
  int i, w;
  int max_value = 0;
  int real_value, real_weight;
  int max_case = 0;

  fprintf(fp, "========================\n");
  fprintf(fp, "03. knapsack with one duplicated item\n");

  QUEUE *queue = create_queue();

  for (int p_number = 1; p_number <= item_count; p_number++) {
    for (i = 0; i <= item_count; i++) {
      for (w = 0; w <= capacity; w++) {
        real_weight = i == p_number ? weight[i - 1] * 2 : weight[i - 1];
        real_value = i == p_number ? value[i - 1] * 2 : value[i - 1];

        if (i == 0 || w == 0) table[i][w] = 0;
        else if (real_weight <= w) {
          table[i][w] = getMaxInt(real_value + table[i - 1][w - real_weight], table[i - 1][w]);
        }
        else {
          table[i][w] = table[i - 1][w];
        }
      }
    }

    if (p_number == 1) printf("%d\n", table[2][4]);
    max_value = table[item_count][capacity];
    w--;

    if (max_value > max_case) {
      max_case = max_value;
      clear_queue(queue);

      for (i = item_count; i > 0 && max_value > 0; i--) {
        real_weight = i == p_number ? weight[i - 1] * 2 : weight[i - 1];
        real_value = i == p_number ? value[i - 1] * 2 : value[i - 1];

        if (max_case != table[i - 1][w]) {
          if (p_number == i) {
            enqueue(queue, i, true);
          } else {
            enqueue(queue, i, false);
          }

          max_value -= real_value;
          w -= real_weight;
        }
      }
    }
  }

  pop_queue(fp, queue, 3);
  fprintf(fp, "%d\n", max_case);
}

// ASSIGNMENT 4
void knapsack_two_identical_knapsack(FILE *fp) {
  fprintf(fp, "========================\n");
  fprintf(fp, "04. knapsack with two identical knapsacks\n");
}

/* MAIN */
int main() {
  // get file name
  char infile[MAXLINE] = "input.txt";
  char outfile[MAXLINE] = "output.txt";
  char line[MAXLINE];
  FILE *fp;

  fp = fopen(outfile, "w");

  int weight[MAXCAPACITY];
  int value[MAXCAPACITY];
  int maximum, item_number;

  read_file(infile, weight, value, &maximum, &item_number);

  // 1. knapsack problem
  knapsack(fp, maximum, weight, value, item_number);

  // 2. knapsack with one item split
  knapsack_one_item_split(fp, maximum, weight, value, item_number);

  // 3. knapsack with one duplicated item
  knapsack_one_item_duplicate(fp, maximum, weight, value, item_number);

  // 4. knapsack with two identical knapsacks (challenge)
  knapsack_two_identical_knapsack(fp);

  return 0;
}

/* FUNCTIONS */
void read_file(const char file[], int weight[], int value[], int *maximum, int *item_number) {
  FILE *fp;
  int first, second;
  NODE *buffer = create_node();
  NODE *pos = buffer;

  if (file == NULL) {
    fprintf(stderr, "NULL file name\n");
  }

  fp = fopen(file, "r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",file);
  }

  for (int i = 0; second != -1; i++) {
    fscanf(fp, "%d %d\n", &first, &second);

    if (second == -1) {
      *maximum = first;
      *item_number = i;
      break;
    }

    NODE* new = create_node();

    pos->weight = first;
    pos->value = second;
    pos->next = new;

    pos = pos->next;
  }

  pos = buffer;

  for (int i = 0; i <= *item_number; i++) {
    weight[i] = pos->weight;
    value[i] = pos->value;

    pos = pos->next;
  }

  recursive_free(buffer);
  fclose(fp);
}

int getMaxInt(int former, int latter) {
  if (former > latter) return former;
  else return latter;
}

double getMaxDouble(double former, double latter) {
  if (former > latter) return former;
  else return latter;
}

void recursive_free(NODE *node) {
  NODE *pos;
  while (node != NULL) {
    pos = node;
    node = node->next;
    free(pos);
  }
}

NODE *create_node() {
  NODE *node = (NODE*)malloc(sizeof(NODE));
  
  node->weight = -1;
  node->value = -1;
  node->next = NULL;

  return node;
}

QUEUE *create_queue() {
  QUEUE* queue;
  queue = (QUEUE*)malloc(sizeof(QUEUE));
  if (!queue) return NULL;

  queue->count = 0;
  queue->front = NULL;
  queue->rear = NULL;
  return queue;
}

void enqueue(QUEUE *queue, int data, bool type) {
  NODE *node;
  node = (NODE*)malloc(sizeof(NODE));
  if (!node) return;
  node->value = data;
  node->weight = type ? 1 : 0;
  node->next = NULL;

  if (queue->count == 0) {
    queue->front = node;
  } else {
    queue->rear->next = node;
  }
  queue->rear = node;
  queue->count++;
}

void clear_queue(QUEUE *queue) {
  NODE *cursor = queue->front;

  while (cursor) {
    NODE* pos = cursor;
    cursor = cursor->next;
    queue->count--;
    free(pos);
  }

  queue->front = NULL;
  queue->rear = NULL;
}

void pop_queue(FILE *fp, QUEUE *queue, int type) {
  NODE *cursor = queue->front;

  while (cursor) {
    if (cursor->weight) {
      if (type == 2) {
        fprintf(fp, "%dx0.5 ", cursor->value);
      } else if (type == 3) {
        fprintf(fp, "%dx2 ", cursor->value);
      }
    } else {
      fprintf(fp, "%d ", cursor->value);
    }
    cursor = cursor->next;
  }
}
