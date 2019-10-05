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
            list->pos->next = node;
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

int main() {
  printf("Merge Sort with Linked List\n");

  int data[58] = { 74, 21, 42, 94, 13, 24 ,80, 45, 75, 37, 96, 28, 26, 99 ,97 ,36 ,93 ,18 ,9 ,58 ,64, 47, 67, 86, 0, 35, 10, 34, 50, 39, 53, 43, 73, 89, 85, 81, 44, 33, 61, 90, 15, 71, 30, 63, 88, 70, 14, 6 ,84, 95, 8 ,59, 98, 82, 100, 52, 83, 19 };

  for(int i = 0; i < sizeof(data)/sizeof(int); i++){
    node_insertion(list, 0, data[i]);
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