/* Directions: complete the code by filling in "FILL" parts
 *   adding functions and variables is allowed
 *   do not change print and file load/save
 *   do not use your compiler-specific libraries
 * FILL parts: insertion and merge sort on linked lists
 */

/* Insertion sort and merge sort on Linked Lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/////////////////////////////////////////////////////////////
// common functions for handling arrays
/////////////////////////////////////////////////////////////
long *readkeys_textfile( const char infile[], int *pN )
  // returns an array of long integers, with its size stored in
  // the memory indicated by integer pointer variable pN
  // the retured memory should freed by the caller
{
  long *A, tmp;
  int i;
  FILE *fp;

  // check for input file name
  if ( infile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return NULL;
  }

  // check for file existence
  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",infile);
    return NULL;
  }
  else {
    // check for number of elements
    if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
      fprintf(stderr, "cannot read number of elements %s\n",infile);
      return NULL;
    }
    else {
      A = (long*)malloc(sizeof(long)*(*pN));
      for (i=0; i<(*pN); i++) {
	if ( fscanf(fp, "%ld", &tmp) != 1 ) {
	  fprintf(stderr, "cannot read value at %d/%d\n",i+1,(*pN));
	  *pN = i;	// read data items
	  return A;
	}
	else A[i] = tmp;
      }
    }
    fclose(fp);
    return A;
  }
}

/////////////////////////////////////////////////////////////
// functions for handling linked lists
/////////////////////////////////////////////////////////////
typedef struct LNode {	// update: item type chagned to long, variable name to key
  long key;
  struct LNode* next;
} NODE;

typedef struct {
  int count;
  NODE* front;
  NODE* rear;
  NODE* pos;
} LIST;

NODE* create_node(long data){
    NODE* node = (NODE*)malloc(sizeof(NODE));
    if(!node){
        return NULL;
    }
    node->key = data;
    node->next = NULL;

    return node;
}

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

bool node_insertion(LIST* list, unsigned int index, long data){
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

void traverse(LIST* list){
    int length = list->count;
    long data;
    list->pos = list->front;
    printf("[");
    for(int i = 0; i < length; i++){
        data = list->pos->key;
        list->pos = list->pos->next;
        printf("%ld ", data);
    }
    printf("]\n");
}

// struct LNode *generateLNode(long key)
// {
//   NODE* node = (struct LNode*) malloc(sizeof(NODE));
//   node->key = key;
//   node->next = NULL;	// bugfix on 190924
//   return node;
// }

// struct LNode *insert_head(struct LNode *head, struct LNode *newPtr)
// {
//   if ( newPtr == NULL ) return head;	// Nothing to add
//   else {
//     newPtr->next = head;
//     return newPtr;
//   }
// }

// struct LNode *insert_next(struct LNode *prev, struct LNode *newPtr)
// {
//   //struct LNode *cur;	// unnecessary 190924 
//   if ( newPtr == NULL ) return prev;	// Nothing to add
//   else if ( prev == NULL ) return insert_head(NULL, newPtr);
//   else {
//     //cur = prev->next;	// unnecessary, so removed, on 190924 
//     //newPtr->next = cur;	// unnecessary, so removed, on 190924 
//     newPtr->next = prev->next;
//     prev->next=newPtr;
//     return newPtr;
//   }
// }

// void traverse(struct LNode *L)
// {
//   struct LNode *cur;
//   printf("[");
//   for (cur=L ; cur != NULL ; cur = cur->next )
//     printf("%ld ",cur->key);
//   printf("]\n");
// }

// the below list handling codes are not used.
// so commented out, but may use them
/*
struct LNode *emptylist()
{
  return NULL;
}

struct LNode *delete_head(struct LNode *head)
{
  struct LNode *cur;
  if ( head == NULL ) return NULL;	// empty, nothing to delete
  else {
    cur = head;
    head = head->next;
    cur->next = NULL;	 // not necessary, but just left 
    free(cur);
    return head;
  }
}

struct LNode *delete_next(struct LNode *prev)
{
  struct LNode *cur;
  if ( prev == NULL ) return NULL;	// empty from prev, nothing
  else {
    cur = prev->next;
    prev->next = cur->next;
    cur->next = NULL;	// not necessary
    free(cur);
    //cur = prev->next;	// not necessary here
    return prev;
  }
}

struct LNode *lookup_recursive (long x, struct LNode *L)
{
  if (L == NULL) return NULL;
  else if (x == L->key) return L;
  else return lookup_recursive(x, L->next);
}

struct LNode *lookup_iterative (long x, struct LNode *L)
{
  struct LNode *cur;
  for (cur=L ; cur != NULL ; cur = cur->next ) {
    if ( x == cur->key ) return cur;
  }
  return NULL;
}
*/

void free_list_iterative (LIST* L)
{
  struct LNode *tmp;
  L->pos = L->front;
  while ( L->pos != NULL ) {
    tmp = L->pos;
    L->pos = L->pos->next;	// every node access should be done before free()
    free(tmp);
  }
}

/////////////////////////////////////////////////////////////
// read a linked list from textfile
/////////////////////////////////////////////////////////////
LIST* array2linkedlist(long A[], int N)
{
  struct LNode *head, *cur, *prev;
  int i;

  LIST* list = create_list();

  // head = NULL; prev = NULL;
  for (i=0; i<N; i++) {
    node_insertion(list, i, A[i]);
  }
  
  return list;
}

LIST* readkeys_textfile_linkedlist( const char infile[], int *pN )
  // read text file of integers:
  // number_of_intergers integer1 integer2 ...
  // then convert it to a linked list 
  // returns a list list of struct LNode, with its size stored in
  // the memory indicated by integer pointer variable pN
{
  LIST* head;
  long *A;

  A = readkeys_textfile(infile, pN);
  if ( A != NULL && *pN > 0 ) {
    head = array2linkedlist(A,*pN);
    free(A);
    return head;
  }
  else return NULL;	// failed
}

void savekeys_textfile_linkedlist( const char outfile[],
    LIST* head, int N )
  // save the given linked list to file whose name given by outfile[]
{
  FILE *fp;

  // check for output filename
  if ( outfile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return;
  }

  // check for file existence
  fp = fopen(outfile,"w");
  if ( !fp ) {
    fprintf(stderr, "cannot open file for write %s\n",outfile);
  }
  else {
    // save number of elements
    fprintf(fp, "%d\n",N);
    for (head->pos = head->front; head->pos; head->pos = head->pos->next ) {
      fprintf(fp, "%ld ", head->pos->key);
    }
    fprintf(fp,"\n");
    fclose(fp);
  }
}


/////////////////////////////////////////////////////////////
// linked list insertion sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */

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

void list_insertion_sort(LIST* list) {
  int i, j;
  NODE* pos = list->front;
  NODE* least = NULL;

  for (i = 1; i < list->count; i++) {
    NODE* cursor = pos->next;
    least = pos;

    for (j = 1; j <= list->count - i; j++) {
      if (least->key > cursor->key) {
        least = cursor;
      }
      cursor = cursor->next;
    }

    // printf("pos: %d, least: %d\n", pos->key, least->key);
    if (pos != least) {
      swap_node(list, pos, least);
      pos = least;
    }
    pos = pos->next;
  }
}

// struct LNode *list_insertion_sort ( struct LNode *head )
// {
//   /* GJ: FILL */
// }


/////////////////////////////////////////////////////////////
// linked list merge sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */

void list_array_merge_sort( struct LNode **head_array, int N )
{
  /* GJ: FILL */
}

/////////////////////////////////////////////////////////////

/* File name by Argument input -> scanf read
 * Input file name can be typed in the terminal
 * Update on 9/24: element size is changed to long,
 * to distinguish array index easily
 * in struct Node, item -> key, Node -> LNode 
 * Update on 9/25: file reading is moved to a function readkeys_textfile()
 */
int main()
{
  int i, numElements;
  LIST* head;

  /* for file name, max length 1023 including path */
  // char infile[1024], outfile[1024];
  char infile[1024] = "input/input58.txt";
  char outfile[1024] = "output/heavy_arraysort-merge-58.txt";
  int method;	// which sorting method

  // required for mergesort
  struct LNode **H;

  /* file name given by keyboard */
  printf("Input file name? ");
  // scanf("%s",infile);
  printf("Output file name? ");
  // scanf("%s",outfile);

  printf("Method (1: insertion, 2: merge)? ");
  scanf("%d",&method);

  /* read text file of integers:
   * number_of_intergers integer1 integer2 ...
   * then convert it to a linked list */
  head = readkeys_textfile_linkedlist(infile, &numElements);

  if ( head != NULL ) {
    // print all if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
    {
      traverse(head);
      printf("%d Loaded\n", numElements);
    }

    switch ( method ) {
      case 1:	// insertion sort
	list_insertion_sort(head);
	break;
      case 2:	// merge sort
	// change linked list to individual list items,
	// and save their pointers to an array of LNode* types
	// H = (struct LNode**) malloc(sizeof(struct LNode*)*numElements);
	// for (i=0; i<numElements; i++, head=head->next) H[i] = head;

	// // have to isolate list items later
	// for (i=0; i<numElements; i++) H[i]->next=NULL;

	// head = list_array_merge_sort(H,numElements);
	break;
      default:
	break;
    }

    // print out results, if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
      traverse(head);

    // save results
    savekeys_textfile_linkedlist(outfile, head, numElements);

    // once allocated, should be freed after use
    free_list_iterative(head);
  }

  return 0;
}
