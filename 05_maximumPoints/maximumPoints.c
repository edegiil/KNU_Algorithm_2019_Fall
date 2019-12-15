/*
COMP319 Algorithms, Fall 2019
Assignment5
Visit maximum points
2015112182 Park Gideok
*/

/* HEADERS */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* STRUCTURE DEFINITIONS */
typedef struct node {
  int data;
  struct node *next;
} NODE;

typedef struct {
  int count;
  NODE *front;
  NODE *rear;
  NODE *pos;
} LIST;

typedef struct buffer {
  int x;
  int y;
  struct buffer *next;
} BUFFER;

/* GLOBAL DEFINITIONS */
#define MAX_STRING_LENGTH 1024

/* PROTOTYPES */
void create_map(const char file[], LIST *map[]);
NODE* create_node();
LIST* create_list();
bool append_node(LIST *list, int data);
double get_distance(int former_x, int former_y, int latter_x, int latter_y);
void free_list(LIST *list);
void free_map(LIST *map[]);
BUFFER* create_buffer(int x, int y);
BUFFER* save_at_buffer(BUFFER* buffer, int x, int y);

/* ASSIGNMENT */
void visit_maximum_points(FILE *fp, LIST *map[], int fuel_limit) {
  double fuel_capacity = 0;
  double fuel_check = fuel_limit;
  double distance = 0;
  int index = 0;
  int former_x = 0, former_y = 0;

  fprintf(fp, "%d ", fuel_limit);

  BUFFER *buffer = NULL;

  while (index <= 100) {
    int latter_x, latter_y;

    if (map[index]->count == 0) {
      index++;
      continue;
    }

    if (map[index]->count == 1) {
      fuel_capacity += fuel_limit / 100;
  
      latter_x = index;
      latter_y = map[index]->front->data;

      double d = get_distance(former_x, former_y, latter_x, latter_y);

      bool check = d < fuel_capacity && get_distance(latter_x, latter_y, 100, 100) + d < fuel_check;

      if (check) {
        distance += d;
        fuel_capacity -= d;
        fuel_check -= d;
        former_x = latter_x;
        former_y = latter_y;
        buffer = save_at_buffer(buffer, former_x, former_y);
      }
    } else {
      map[index]->pos = map[index]->front;

      while (map[index]->pos) {
        fuel_capacity += fuel_limit / 100;
    
        latter_x = index;
        latter_y = map[index]->pos->data;

        double d = get_distance(former_x, former_y, latter_x, latter_y);

        bool check = d < fuel_capacity && get_distance(latter_x, latter_y, 100, 100) + d < fuel_check;

        if (check) {
          distance += d;
          fuel_capacity -= d;
          fuel_check -= d;
          former_x = latter_x;
          former_y = latter_y;
          buffer = save_at_buffer(buffer, former_x, former_y);
        }

        map[index]->pos = map[index]->pos->next;
      }
    }
    index++;
  }
  
  fprintf(fp, "%.1f\n", distance);

  while (buffer) {
    BUFFER *tmp = buffer;
    fprintf(fp, "%d %d\n", buffer->x, buffer->y);
    tmp = buffer;
    buffer = buffer->next;
    free(tmp);
  }

  fprintf(fp, "\n");
}

/* MAIN */
int main() {
  char infile[MAX_STRING_LENGTH];
  char outfile[MAX_STRING_LENGTH];
  char line[MAX_STRING_LENGTH];
  FILE *fp;

  memset(line, 0, sizeof(char) * MAX_STRING_LENGTH);
  fprintf(stderr,"Input file name? ");
  fgets(line, MAX_STRING_LENGTH, stdin);
   if (strlen(line) == 0 || sscanf(line, "%s", infile) != 1) {
    fprintf(stderr, "cannot read input file name from '%s'\n", line);
    exit(0);
  }

  memset(line, 0, sizeof(char) * MAX_STRING_LENGTH);
  fprintf(stderr,"Output file name? ");
  fgets(line, MAX_STRING_LENGTH, stdin);
   if (strlen(line) == 0 || sscanf(line, "%s", outfile) != 1) {
    fprintf(stderr, "cannot read output file name from '%s'\n", line);
    exit(0);
  }

  LIST *map[101];

  fp = fopen(outfile, "w");

  create_map(infile, map);

  visit_maximum_points(fp, map, 200);
  visit_maximum_points(fp, map, 400);
  visit_maximum_points(fp, map, 600);
  visit_maximum_points(fp, map, 800);
  visit_maximum_points(fp, map, 1000);
  visit_maximum_points(fp, map, 1500);

  free_map(map);

  return 0;
}

/* FUNCTIONS */
void create_map(const char file[], LIST *map[]) {
  FILE *fp;
  int index, value;

  for (int j = 0; j <= 100; j++) {
    map[j] = create_list();
  }

  if (file == NULL) fprintf(stderr, "NULL file name\n");
  fp = fopen(file, "r");
  if (!fp) fprintf(stderr, "unable to open file %s\n", file);

  fscanf(fp, "%d\n", &value);

  for (int i = 0; index != 100; i++) {
    fscanf(fp, "%d %d", &index, &value);
    append_node(map[index], value);
  }
}

NODE* create_node(int data) {
  NODE* node = (NODE*)malloc(sizeof(NODE));
  if(!node) return NULL;

  node->data = data;
  node->next = NULL;

  return node;
}

LIST* create_list() {
  LIST* list = (LIST*)malloc(sizeof(LIST));
  if(!list) return NULL;

  list->count = 0;
  list->front = NULL;
  list->rear = NULL;
  list->pos = NULL;

  return list;
}

bool append_node(LIST* list, int data) {
    if(!list) return false;

    NODE* node = create_node(data);

    if(list->count == 0){
      list->front = node;
      list->rear = node;
    } else {
      list->pos = list->front;
      while (list->pos->next) {
        list->pos = list->pos->next;
      }
      list->pos->next = node;
      list->rear = node;
    }
    
    list->count++;
    return true;
}

double get_distance(int former_x, int former_y, int latter_x, int latter_y) {
  double x_diffrence = (double)(latter_x - former_x);
  double y_diffrence = (double)(latter_y - former_y);

  double distance = sqrt(pow(x_diffrence, 2) + pow(y_diffrence, 2));

  return distance;
}

void free_list(LIST *list) {
  while (list->pos) {
    list->pos = list->front->next;
    free(list->front);
    list->front = list->pos;
  }

  free(list);
}

void free_map(LIST *map[]) {
  for (int i = 0; i <= 100; i++) {
    if (map[i]) {
      free_list(map[i]);
    }
  }
}

BUFFER* create_buffer(int x, int y) {
  BUFFER* buffer = (BUFFER*)malloc(sizeof(BUFFER));
  if(!buffer) return NULL;

  buffer->x = x;
  buffer->y = y;
  buffer->next = NULL;

  return buffer;
}

BUFFER* save_at_buffer(BUFFER* buffer, int x, int y) {
  if (!buffer) {
    buffer = create_buffer(x, y);
    return buffer;
  } else {
    BUFFER *pos = buffer;
    while (pos->next) {
      pos = pos->next;
    }
    pos->next = create_buffer(x, y);
    return buffer;
  }
}
