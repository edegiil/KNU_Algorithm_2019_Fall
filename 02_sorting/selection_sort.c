#include "sort.h"

LIST* create_list() {
  LIST* list = (LIST*)malloc(sizeof(LIST));
  if (!list) {
    return NULL;
  }

  list->count = 0;
  list->front = NULL;
  list->rear = NULL;
  list->pos = NULL;

  return list;
}

NODE* create_node(int data){
    NODE* node = (NODE*)malloc(sizeof(NODE));
    if(!node){
        return NULL;
    }
    node->data = data;
    node->next = NULL;

    return node;
}

bool node_insertion(LIST* list, unsigned int index, int data){
  if(index > list->count || !list){
      return false;
  }

  printf("%d %d\n", index, data);

  NODE* node = create_node(data);

  if(list->count == 0){
      list->front = node;
      list->rear = node;
      node->next = NULL;
      list->count++;
      return true;
  }

  int iteration = 0;
  if(index != 0){
      list->pos = list->front;
      while(iteration != index){
          list->pos = list->pos->next;
          iteration++;
      }
      if(index != list->count){
          node->next = list->pos->next;
          list->pos->next = node;
      }else{
        list->rear->next = node;
        list->rear = node;
      }
  }else{
      node->next = list->front;
      list->front = node;
  }
  
  list->count++;
  return true;
}

void show_list(LIST* list){
  int length = list->count;
  int data;
  list->pos = list->front;
  for(int i = 0; i < length; i++){
      data = list->pos->data;
      list->pos = list->pos->next;
      printf("%d ", data);
  }
  printf("\n");
}

void swap_node(LIST* list, NODE* former_node, NODE* latter_node) {
  int iter = 0;
  list->pos = list->front;

  if (former_node->next == latter_node) {
    former_node->next = latter_node->next;
    latter_node->next = former_node;

    if (former_node == list->front) {
      list->front = latter_node;
    } else {
      while(iter < list->count) {
        if (list->pos->next == former_node) {
          list->pos->next = latter_node;
          break;
        }
        list->pos = list->pos->next;
        iter++;
      }
    }
  } else {
    while(iter < list->count) {
      if (list->pos->next == latter_node) {
        list->pos->next = former_node;
        break;
      }
      list->pos = list->pos->next;
      iter++;
    }
    
    NODE* temp = former_node->next;
    former_node->next = latter_node->next;
    latter_node->next = temp;

    iter = 0;
    list->pos = list->front;

    if (former_node == list->front) {
      list->front = latter_node;
    } else {
      while(iter < list->count) {
        if (list->pos->next == former_node) {
          list->pos->next = latter_node;
          break;
        }
        list->pos = list->pos->next;
        iter++;
      }
    }
  }
  list->pos = list->pos->next;
}

void selection_sort(LIST* list) {
  int i, j;
  NODE* pos = list->front;
  NODE* least = NULL;

  for (i = 1; i < list->count; i++) {
    NODE* cursor = pos->next;
    least = pos;
    printf("--- iteration %d ---\n", i);

    for (j = 1; j <= list->count - i; j++) {
      if (least->data > cursor->data) {
        least = cursor;
      }
      cursor = cursor->next;
    }

    printf("pos: %d, least: %d\n", pos->data, least->data);
    if (pos != least) {
      printf("swap start...\n");
      swap_node(list, pos, least);
      pos = least;
      show_list(list);
      printf("swap end!\n");
    }
    pos = pos->next;
  }
}

int main() {
  printf("*****Selection Sort with Linked List*****\n");

  LIST* list = create_list();

  int data[14] = { 16, 25, 2, 66, 6, 10, 8, 3, 7, 9, 4, 1, 11, 5 };

  for(int i = 0; i < sizeof(data)/sizeof(int); i++){
    printf("%d\n", i);
    node_insertion(list, i, data[i]);
  }

  printf("initial list : ");
  show_list(list);
  printf("==================\n");

  selection_sort(list);

  printf("==================\n");
  printf("sorted list : ");
  show_list(list);

  return 0;
}
