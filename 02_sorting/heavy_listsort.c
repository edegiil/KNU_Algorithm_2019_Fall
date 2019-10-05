/* Directions: complete the code by filling in "FILL" parts
 *   adding functions and variables is allowed
 *   do not change print and file load/save
 *   do not use your compiler-specific libraries
 * Note: TIME in seconds is measured 
 * FILL parts: insertion and merge sort on heavy linked lists
 */

/* Insertion sort and merge sort on Heavy Linked Lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEASURE_TIME    // to measure execution time

/////////////////////////////////////////////
// to compute execution time in seconds
/////////////////////////////////////////////
#ifdef MEASURE_TIME
#include<time.h>
/* time measurement example */
/*
clock_t start, end;
double cpu_time_used;

start = clock();
... // Do the work.
end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
*/

/* from GJANG: not sure if the above works with Windows */
#endif

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
#define LOAD_SIZE	4096
typedef struct LHNode {	// update: item type chagned to long, variable name to key
  long key;
  struct LHNode* next;
  long ManyItems[LOAD_SIZE];
} NODE;

typedef struct {
  int count;
  NODE* front;
  NODE* rear;
  NODE* pos;
} LIST;

NODE* generateLHNode(long key){
  NODE* node = (NODE*)malloc(sizeof(NODE));
  if(!node){
      return NULL;
  }
  node->key = key;
  node->next = NULL;

  for (int i=0; i<LOAD_SIZE; i++) node->ManyItems[i] = key;

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

  NODE* node = generateLHNode(data);
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

void delete_node(LIST* list, NODE* node) {
  if (!list || list->count == 0) {
    return;
  }

  NODE* cur = list->front;

  if (list->count == 1) {
    list->count = 0;
    list->front = NULL;
    list->rear = NULL;
  }

  if (node == list->front) {
    list->front = node->next;
    list->count--;
  }

  while (cur != NULL) {
    if (cur->next == node) {
      cur->next = node->next;
      node->next = NULL;
      if (node == list->rear) {
        list->rear = cur;
      }
      list->count--;
      return;
    }
    cur = cur->next;
  }
}

// struct LHNode *generateLHNode(long key)
// {
//   struct LHNode *tmp;
//   int i;

//   tmp = (struct LHNode*) malloc(sizeof(struct LHNode));
//   tmp->key = key;
//   tmp->next = NULL;	// bugfix on 190924

//   // fill ManyItems with the key
//   for (i=0; i<LOAD_SIZE; i++) tmp->ManyItems[i] = key;

//   return tmp;
// }

// struct LHNode *insert_head(struct LHNode *head, struct LHNode *newPtr)
// {
//   if ( newPtr == NULL ) return head;	// Nothing to add
//   else {
//     newPtr->next = head;
//     return newPtr;
//   }
// }

// struct LHNode *insert_next(struct LHNode *prev, struct LHNode *newPtr)
// {
//   //struct LHNode *cur;	// unnecessary 190924 
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

long pick_random_item( struct LHNode *pH )
  // pick an item from HeavyItem randomly
  // to check if items are maintained correctly
  // if key is changed only, values in the itmes may not match key
{
  return pH->ManyItems[rand()%LOAD_SIZE];
}

/////////////////////////////////////////////////////////////
// functions for heavy load of items
void traverse_print_random_item(LIST* L)
{
  printf("[ ");
  for (L->pos = L->front ; L->pos != NULL ; L->pos = L->pos->next ) {
    //printf("%ld ",cur->key);
    printf("%ld ", pick_random_item(L->pos));
  }
  printf("]\n");
}

// the below list handling codes are not used.
// so commented out, but may use them
/*
struct LHNode *emptylist()
{
  return NULL;
}

struct LHNode *delete_head(struct LHNode *head)
{
  struct LHNode *cur;
  if ( head == NULL ) return NULL;	// empty, nothing to delete
  else {
    cur = head;
    head = head->next;
    cur->next = NULL;	 // not necessary, but just left 
    free(cur);
    return head;
  }
}

struct LHNode *delete_next(struct LHNode *prev)
{
  struct LHNode *cur;
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

struct LHNode *lookup_recursive (long x, struct LHNode *L)
{
  if (L == NULL) return NULL;
  else if (x == L->key) return L;
  else return lookup_recursive(x, L->next);
}

struct LHNode *lookup_iterative (long x, struct LHNode *L)
{
  struct LHNode *cur;
  for (cur=L ; cur != NULL ; cur = cur->next ) {
    if ( x == cur->key ) return cur;
  }
  return NULL;
}
*/

void free_list_iterative (LIST* L)
{
  struct LHNode *tmp;
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
  struct LHNode *head, *cur, *prev;
  int i;

  LIST* list = create_list();

  head = NULL; prev = NULL;
    for (i=0; i<N; i++) {
    node_insertion(list, i, A[i]);
  }
  
  return list;
}

LIST* readkeys_textfile_linkedlist( const char infile[], int *pN )
  // read text file of integers:
  // number_of_intergers integer1 integer2 ...
  // then convert it to a linked list 
  // returns a list list of struct LHNode, with its size stored in
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
LIST* list_insertion_sort(LIST* list) {
  LIST* sorted_list = create_list();

  while (list->count != 0) {
    list->pos = list->front;
    NODE* least = list->pos;
    for (;list->pos != NULL; list->pos = list->pos->next) {
      if (list->pos->key < least->key) {
        least = list->pos;
      }
    }
    delete_node(list, least);
    if (sorted_list->front == NULL) {
      sorted_list->front = least;
      sorted_list->rear = least;
      sorted_list->pos = sorted_list->front;
      sorted_list->count++;
    } else {
      sorted_list->rear->next = least;
      sorted_list->rear = least;
      sorted_list->count++;
    }
  }
  free(list);

  return sorted_list;
}


/////////////////////////////////////////////////////////////
// linked list merge sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */

void list_array_merge_sort( struct LHNode **head_array, int N )
{
  /* GJ: FILL */
}
/////////////////////////////////////////////////////////////

/* File name by Argument input -> scanf read
 * Input file name can be typed in the terminal
 * Update on 9/24: element size is changed to long,
 * to distinguish array index easily
 * in struct Node, item -> key, Node -> LHNode 
 * Update on 9/25: file reading is moved to a function readkeys_textfile()
 */
int main()
{
  int i, numElements;
  LIST* head;

  /* for file name, max length 1023 including path */
  // char infile[1024], outfile[1024];
  char infile[1024] = "input/input12345.txt";
  char outfile[1024] = "output/heavy_arraysort-insertion-12345.txt";
  int method;	// which sorting method

  // required for mergesort
  struct LHNode **H;

#ifdef MEASURE_TIME
  clock_t start, end;
  double cpu_time_used;

  start = clock();
#endif

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
      traverse_print_random_item(head);
      printf("%d Loaded\n", numElements);
    }

    switch ( method ) {
      case 1:	// insertion sort
	      head = list_insertion_sort(head);
	      break;
      case 2:	// merge sort
	// change linked list to individual list items,
	// and save their pointers to an array of LHNode* types
        // H = (struct LHNode**) malloc(sizeof(struct LHNode*)*numElements);
        // for (i=0; i<numElements; i++, head=head->next) H[i] = head;

        // // have to isolate list items later
        // for (i=0; i<numElements; i++) H[i]->next=NULL;

        // head = list_array_merge_sort(H,numElements);
        // break;
      default:
	      break;
    }

    // print out results, if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
      traverse_print_random_item(head);

    // save results
    savekeys_textfile_linkedlist(outfile, head, numElements);

    // once allocated, should be freed after use
    free_list_iterative(head);
  }

#ifdef MEASURE_TIME
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("TIME %.5f seconds\n",cpu_time_used);
#endif

  return 0;
}
