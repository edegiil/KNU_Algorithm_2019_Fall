/* Binary search tree using doubly-linked lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MEASURE_TIME	// to measure time

/////////////////////////////////////////////////////////////
// DATA STRUCTURE:
// binary tree node definition using doubly linked lists
// key is a string of a fixed length
// KEYLENGTH	length of the key
// BULK_SIZE	key is hidden in the "BULK"
//	to retrieve key, we have to dig into the "BULK"
//	so accessing key takes average "BULK_SIZE"/2 searches
//	this is INTENTIONALLY to add extra overhead for search
//	your program should reduce the number of key accesses at your best
/////////////////////////////////////////////////////////////
#define KEYLENGTH	3
#define BULK_SIZE	4096
//#define BULK_SIZE	65536
typedef struct BTNode {	
  char bulk[BULK_SIZE];	// null character to be added
  struct BTNode *left, *right;	// binary tree: left and right children
} NODE;


/////////////////////////////////////////////////////////////
// GIVEN: functions for binary tree node 
// name and parameters only
// implementations are moved after "main" function
/////////////////////////////////////////////////////////////

const char* getkey(struct BTNode *a);
// return value: char array of KEYLENGTH+1 (+1 for '\0' character)
//  key is hidden in "bulk", so use the following function to
//  read key string of length KEYLENGTH
//  it will make BULK_SIZE/2 searches on average
//  so try to use it as rarely as possible

int setKey(struct BTNode *a, const char kw[]);
// return value: 0 for failure (NULL a), 1 for success
//  the following function hides a string "kw" of KEYLENGTH
//  by randomly selecting the location to save key

int copykey(struct BTNode *dst, struct BTNode *src) 
{ return setKey(dst,getkey(src)); }
// copies the key of one node to the other
// very simple, single line, so implementation is given here

int comparekey(struct BTNode *a, struct BTNode *b);
// return value: (by character comparison)
//  -1 if a's key < b's key
//  0 if a's key == b's key
//  +1 if a's key > b's key
//  may be needed for binary search tree search and build-up

struct BTNode *generate_btnode(const char kw[]);
// return value: pointer to a single BTNode (left/right are NULL)
//  generates a node for binary tree

void free_bt_recursive (struct BTNode *bt);
// frees a binary tree

struct BTNode *copy_bt_recursive (struct BTNode *bt);
// return value: pointer to the root of the copy of the given binary tree "bt"

struct BTNode *insert_left_bcnode(
    struct BTNode *parent, struct BTNode *newPtr );
//  adds a node to the left of a BTNode parent
//  it will be used to generate a left-half binary tree
//  (LHBT, all rights are NULL)
// pre-condition: left pointer to the new node should be NULL
// to store the left pointer to the parent node
// return value: parent if the given parent is not NULL; newPtr if parent NULL

struct BTNode *readkeys_textfile_LHBT( const char infile[], int *pN );
// File I/O: read key words from the given file
// and generate a binary tree which is left-half
// (all right children are NULL)

void bst_to_array(NODE *bst, NODE *arr[], int *count);

int find_root_index(int numNodes, int start);

NODE *partition(NODE* head, NODE* end, NODE **new_head, NODE **new_end);

NODE* getEnd(NODE* lhbt);

/////////////////////////////////////////////////////////////
// FILL 1: generate a binary search tree using insertion
/////////////////////////////////////////////////////////////
struct BTNode *insert_to_BST_leaf(struct BTNode *bst, struct BTNode *newPtr)
{
  if ( bst == NULL ) return newPtr;	// new bst as the input node
  else if ( newPtr == NULL ) return bst;	// nothing to add
  else {
    if ( comparekey(bst, newPtr) < 0 ) {
      /* FILL */
      bst->right = insert_to_BST_leaf(bst->right, newPtr);
      return bst;
    }
    else {
      /* FILL */
      bst->left = insert_to_BST_leaf(bst->left, newPtr);
      return bst;
    }
  }
}

struct BTNode *generate_BST_by_insertion(struct BTNode *lhbt)
{
  /* FILL */
  /* (hint: use insert_to_BST_leaf repeatedly) */
  NODE *bst = NULL;

  if (!lhbt) {
    return NULL;
  }

  bst = generate_BST_by_insertion(lhbt->left);

  lhbt->left = NULL;
  bst = insert_to_BST_leaf(bst, lhbt);

  return bst;
}

/////////////////////////////////////////////////////////////
// FILL 2: PRINT
/////////////////////////////////////////////////////////////
int print_LHBT(FILE *fp, struct BTNode *lhbt)
  // prints left-half binary tree
  // ___-___-___
  // INPUT
  //   fp: file pointer for the output file, stdout for monitor output
  //   lhbt: left-half binary tree (right pointers are all null)
  // RETURNs number of NODES in the list
{
  int num_nodes;

  num_nodes=0;
  while ( lhbt ) {
    if ( lhbt->right != NULL ) {	// check if left-half
      fprintf(stderr,"Non-left-half binary tree for %s\n",__FUNCTION__);
      break;
    }
    else {
      fprintf(fp, "%s", getkey(lhbt));
      if ( lhbt->left != NULL ) fprintf(fp, "-");
    }
    num_nodes++;
    lhbt = lhbt->left;
  }

  fprintf(fp, "\n");	// change the line at the end

  return num_nodes;
}

int print_BST_sortedorder(FILE *fp, struct BTNode *bst, int level)
  // prints a binary search tree nodes by a single line
  // in a SORTED ORDER
  // (hint: inorder traversal)
  // INPUT
  //   fp: file pointer for the output file, stdout for monitor output
  //   bst: root node of the BST, should satisfy the property of
  //      binary search tree, left <= center < right
  //   level: level of the root node, starting from 0 (empty)
  //      if it is unnecessary, do not have to use it
  // RETURNs number of NODES in the list
{
  /* GJ: you may fill out the *FILL* lines, or completely rewrite */
  int count;	// to count the number of nodes

  count = 0;
  if ( bst != NULL ) {
    level++;	// root node of the current subtree exists

    /* FILL: print left subtree */
    count += print_BST_sortedorder(fp, bst->left, level);

    // center node
    fprintf(fp, "%s ",getkey(bst));
    count++;

    /* FILL: print right subtree */
    count += print_BST_sortedorder(fp, bst->right, level);
  }

  // change the line once - only at the bst node
  if ( level <= 1 ) fprintf(fp, "\n");

  return count;
}

int print_BST_right_center_left(FILE *fp, struct BTNode *bst, int level)
  // prints a binary search tree, rotated by 270 degrees
  // Note: key's length is fixed to KEYLENGTH, so there are
  // (KEYLENGTH+1)*level spaces. For examples,
  //         999
  //     777
  //         555
  // 333
  //     222
  //         111
  // INPUT
  //   (same as print_BST_sortedorder)
  // RETURNs HEIGHT-1 of the printed tree (2 in the above example)
  //   (hint: printing order is right -> center -> left
  //    carefully count the number of spaces)
{
  /* FILL */
  int height = 0;

  if (bst) {
    int tmp;
    level++;
    height = level;

    tmp = print_BST_right_center_left(fp, bst->right, level);
    if (height < tmp) height = tmp;

    for (int i = 1; i < level; i++) {
      fprintf(fp, "   ");
    }
    fprintf(fp, "%s\n", getkey(bst));

    tmp = print_BST_right_center_left(fp, bst->left, level);
    if (height < tmp) height = tmp;
  }

  return height;
}

int print_BST_1(FILE *fp, struct BTNode *bst, int level, int new_line)
  // prints a binary search tree, rotated by 270 degrees, with less lines
  //  1) center and right are in the same line
  //  2) left subtree is below the center
  //  3) right is connected by '/' and left by '+'
  // Note: key's length is fixed to KEYLENGTH,
  // so left and right begins at (KEYLENGTH+1)*level+1
  // 333/777/999
  //        +555
  //    +222
  //        +111
  // (or)
  // 100/400/800/900
  //                +900
  //            +800
  //                +500/700
  //    +000
  // INPUT and OUPUT
  //   (same as print_BST_right_center_left)
  //   (hint: printing order is center -> right -> left)
{
  /* FILL */
  int height = 0;

  if (bst) {
    int tmp, a;
    level++;
    height = level;

    if (level > 1 && !new_line) {
      fprintf(fp, "   ");
      for (int i = 2; i < level; i++) {
        fprintf(fp, "    ");
      }
      fprintf(fp, "+");
    }

    fprintf(fp, "%s", getkey(bst));

    if (!bst->right) {
      fprintf(fp, "\n");
    } else {
      fprintf(fp, "/");
    }

    tmp = print_BST_1(fp, bst->right, level, 1);
    if (height < tmp) height = tmp;

    tmp = print_BST_1(fp, bst->left, level, 0);
    if (height < tmp) height = tmp;
  }

  return height;
}

int print_BST_2(FILE *fp, struct BTNode *bst, int level, int new_line, int arr[])
  // same as print_BST_1 except vertical line from center to left
  // 100/400/800/900
  //    |       |   +900
  //    |       +800
  //    |           +500/700
  //    +000
  // Hint: stack or some extra variables may help.
  //       static variable can be used as well
  //       You may add additional parameter to the function if necessary
{
  /* FILL */
  int height = 0;

  if (bst) {
    int tmp, a;
    level++;
    height = level;

    if (!arr) {
      int new_arr[10000] = { 0, };
      arr = new_arr;
    }

    if (bst->left) {
      arr[level] = 1;
    } else {
      arr[level] = 0;
    }

    if (level > 1 && !new_line) {
      fprintf(fp, "   ");
      for (int i = 2; i < level; i++) {
        if (arr[i]) fprintf(fp, "|   ");
        else fprintf(fp, "    ");
      }
      fprintf(fp, "+");
      arr[level] = 0;
    }

    fprintf(fp, "%s", getkey(bst));

    if (!bst->right) {
      fprintf(fp, "\n");
    } else {
      fprintf(fp, "/");
    }

    tmp = print_BST_2(fp, bst->right, level, 1, arr);
    if (height < tmp) height = tmp;

    tmp = print_BST_2(fp, bst->left, level, 0, arr);
    if (height < tmp) height = tmp;
  }

  return height;
}


/////////////////////////////////////////////////////////////
// FILL 3: Conversion of an BST to a complete BST
/////////////////////////////////////////////////////////////

struct BTNode *BST_to_completeBST(NODE* arr[], int numNodes, int start)
  // convert a BST to complete BST (minimum height, filling in left first)
  // INPUT
  //   bst: root node of the BST, should satisfy the property of
  //      binary search tree, left <= center < right
  //   numNodes: number of nodes in the bst
  //      if not necessary in your implementation, do not have to use it
  // RETURNs a COMPLETE BST 
  // (hint: using extra memory (arrays or lists) may help,
  //  array's rule for parent-child, sorted list, etc.)
{
  /* FILL */
  NODE *bst;
  int mid_pos;

  if (numNodes < 1) return NULL;

  mid_pos = find_root_index(numNodes, start);

  bst = arr[mid_pos];

  int left_nodes = mid_pos - start - 1;
  bst->left = BST_to_completeBST(arr, left_nodes, start);

  int right_nodes = numNodes + start - mid_pos;
  bst->right = BST_to_completeBST(arr, right_nodes, mid_pos);

  return bst;
}


/////////////////////////////////////////////////////////////
// FILL 4: generate binary search tree from a left-half binary tree
// using quick sort
/////////////////////////////////////////////////////////////
struct BTNode *generate_BST_quicksort_basic(NODE* bst, struct BTNode *lhbt, NODE* end)
  // gerate a BST using quick sort algorithm
  // the resultant tree should be identical to generate_BST_by_insertion
{
  /* FILL */
  if (!lhbt || lhbt == end) {
    return lhbt;
  }

  NODE *new_lhbt = NULL;
  NODE *new_end = NULL;

  NODE *pivot = partition(lhbt, end, &new_lhbt, &new_end);

  if (new_lhbt != pivot) {
    NODE *tmp = new_lhbt;
    while (tmp->left != pivot) {
      tmp = tmp->left;
    }
  
    tmp->left = NULL;

    new_lhbt = generate_BST_quicksort_basic(bst ,new_lhbt, tmp);

    while (new_lhbt && new_lhbt->left) {
      new_lhbt = new_lhbt->left;
    }
    tmp->left = pivot;
  }

  pivot->left = generate_BST_quicksort_basic(bst, pivot->left, new_end);
  
  insert_to_BST_leaf(bst, pivot);

  return new_lhbt;
}

struct BTNode *generate_BST_quicksort_advanced(struct BTNode *lhbt)
  // challenge: try to reduce the height using quick sort algorithm
{
  /* FILL */
  return lhbt;
}

/////////////////////////////////////////////////////////////
// main function 
/////////////////////////////////////////////////////////////
#define MAXLINE	1024
int main()
{
  int numWords;	// number of words 
  //int wordLen;	// word length: number of characters per word
  struct BTNode *root, *bst1, *bst2;
  int numNodes, lev;	// level of the tree

  /* for file name, max length 1023 including path */
  char line[MAXLINE];
  char infile[MAXLINE], outfile[MAXLINE];
  FILE *fp;

#ifdef MEASURE_TIME
  clock_t start, end;
  double cpu_time_used;
#endif

  /* input file name given by keyboard */
  memset(line,0,sizeof(char)*MAXLINE);	// clear the buffer
  fprintf(stderr,"Input file name? ");
  fgets(line,MAXLINE,stdin);
  if ( strlen(line) == 0 || sscanf(line,"%s",infile) != 1 ) {
    fprintf(stderr,"cannot read input file name from '%s'\n",line);
    exit(0);
  }

  /* output file name: enter for standard out */
  memset(line,0,sizeof(char)*MAXLINE);	// clear the buffer
  fprintf(stderr,"Output file name? ");
  fgets(line,MAXLINE,stdin);
  if ( strlen(line) == 0 || sscanf(line,"%s",outfile) != 1 ) {
    fprintf(stderr,"cannot read output file name from '%s'\n",line);
    fprintf(stderr,"output to stdout\n");
    fp = stdout;
    memset(outfile,0,sizeof(char)*MAXLINE);	// clear the buffer
  }
  else {
    /* open output file pointer */
    fp = fopen(outfile,"w");
    if ( fp == NULL ) {
      fprintf(stderr, "cannot open file '%s' for write\n",outfile);
      fprintf(stderr,"output to stdout\n");
      fp = stdout;
    }
  }

  /* read text file of integers:
   * number_of_intergers integer1 integer2 ...
   * then convert it to a linked list */
  root = readkeys_textfile_LHBT(infile, &numWords);

#ifdef MEASURE_TIME
  start = clock();
#endif

  if ( root != NULL ) {

    // prints input
    fprintf(fp,"=====================================\n");
    numNodes = print_LHBT(fp, root);
    fprintf(fp,"total %d nodes\n",numNodes);

    // BST construction by simple insertion
    // keep root unchanged
    bst1 = generate_BST_by_insertion(copy_bt_recursive(root));

    fprintf(fp,"=====================================\n");
    numNodes = print_BST_sortedorder(fp,bst1,0);
    fprintf(fp,"total %d nodes (sorted)\n",numNodes);
    fprintf(fp,"=====================================\n");
    lev = print_BST_right_center_left(fp,bst1,0);
    fprintf(fp,"BST height %d\n",lev);
    fprintf(fp,"=====================================\n");
    lev = print_BST_1(fp,bst1,0, 1);
    fprintf(fp,"BST height %d\n",lev);
    fprintf(fp,"=====================================\n");
    lev = print_BST_2(fp,bst1,0, 1, NULL);
    fprintf(fp,"BST height %d\n",lev);
    fprintf(fp,"=====================================\n");
    NODE* arr[numNodes + 1];
    int* count = (int*)malloc(sizeof(int));
    *count = 0;
    bst_to_array(bst1, arr, count);
    bst1 = BST_to_completeBST(arr, numNodes, 0);
    lev = print_BST_2(fp,bst1,0, 1, NULL);
    fprintf(fp,"Complete BST height %d\n",lev);
    fprintf(fp,"=====================================\n");

    // BST using quick sort, pivot as left, basic
    NODE *lhbt = copy_bt_recursive(root);
    bst2 = NULL;
    generate_BST_quicksort_basic(bst2, lhbt, getEnd(lhbt));
    lev = print_BST_2(fp,bst2,0,1 ,NULL);
    fprintf(fp,"BST by QUICKSORT, height %d\n",lev);
    fprintf(fp,"=====================================\n");

    // BST using quick sort, advanced, to reduce height
    bst2 = generate_BST_quicksort_advanced(copy_bt_recursive(root));
    lev = print_BST_2(fp,bst2,0,1, NULL);
    fprintf(fp,"BST by QUICKSORT (advanced), height %d\n",lev);
    fprintf(fp,"=====================================\n");
      
    free_bt_recursive(root);
    free_bt_recursive(bst1);
    free_bt_recursive(bst2);
  }

#ifdef MEASURE_TIME
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  fprintf(fp,"TIME %.5f seconds\n",cpu_time_used);
#endif

  if ( fp != NULL && fp != stdout ) fclose(fp);
  return 0;
}


/////////////////////////////////////////////////////////////
// implementation: functions for binary tree node 
/////////////////////////////////////////////////////////////

const char* getkey(struct BTNode *a)
{
  int i;
  for (i=0; i<BULK_SIZE-KEYLENGTH; i++) {
    if ( a->bulk[i] != '\0' ) return a->bulk+i;
  }
  return NULL;	// not found
}

int setKey(struct BTNode *a, const char kw[])
{
  int pos;
  if ( a != NULL ) {
    // fill with 0
    memset(a->bulk,0,sizeof(char)*BULK_SIZE);

    // find position randomly to store KEYLENGTH+1 characters
    pos = rand()%(BULK_SIZE-KEYLENGTH);
    if ( kw != NULL ) memcpy(a->bulk+pos,kw,sizeof(char)*KEYLENGTH);
    a->bulk[pos+KEYLENGTH] = '\0';	// to make it a C string

    // success
    return 1;
  }
  else return 0;
}

struct BTNode *generate_btnode(const char kw[])
{
  struct BTNode *tmp;

  tmp = (struct BTNode*)malloc(sizeof(struct BTNode));
  setKey(tmp,kw);

  // initial left and right children for the generated leaf node
  tmp->left = tmp->right = NULL;

  return tmp;
}

void free_bt_recursive (struct BTNode *bt)
{
  if ( bt != NULL ) {
    free_bt_recursive(bt->left);
    free_bt_recursive(bt->right);
    free(bt);
  }
}

struct BTNode *copy_bt_recursive (struct BTNode *bt)
{
  struct BTNode *dup;

  if ( bt != NULL ) {
    dup = (struct BTNode*) malloc(sizeof(struct BTNode));
    copykey(dup,bt);
    dup->left = copy_bt_recursive(bt->left);
    dup->right = copy_bt_recursive(bt->right);
  }
  else dup = NULL;
  return dup;
}

struct BTNode *insert_left_bcnode(struct BTNode *parent, struct BTNode *newPtr)
{
  if ( parent == NULL ) return newPtr;	// no parent
  else if ( newPtr == NULL ) return parent;	// Nothing to add
  else if ( newPtr->left != NULL ) {
    fprintf(stderr,"cannot add a node with non-null left tree\n");
    return parent;
  }
  else {
    newPtr->left = parent->left;
    parent->left=newPtr;
    return newPtr;	// returning new node as a new parent
  }
}

// static: internal use only
static int _compare_n_char(const char a[], const char b[], int L)
{
  int i;
  for (i=0; i<L; i++) {
    if ( a[i] < b[i] ) return -1;
    else if ( a[i] > b[i] ) return 1;
    else continue;	// to next character
  }
  return 0;
}

int comparekey(struct BTNode *a, struct BTNode *b)
{
  return _compare_n_char(getkey(a),getkey(b), KEYLENGTH);
}

/////////////////////////////////////////////////////////////
// File I/O
/////////////////////////////////////////////////////////////
struct BTNode *readkeys_textfile_LHBT( const char infile[], int *pN )
  // read key words from the given file
  // and generate a binary tree which is skewed to the left 
  // (all right children are NULL)
{
  struct BTNode *root, *cur, *tmp;
  char word[1024];
  FILE *fp;
  int i;

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

  // check for number of keys
  if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
    fprintf(stderr, "File %s: ",infile);
    fprintf(stderr, "number of keys cannot be read or or wrong\n");
    fclose(fp);
    return NULL;
  }

  /*
  // check for number of characters per key
  if ( fscanf(fp, "%d", pL) != 1 || *pL <= 0 ) {
    fprintf(stderr, "File %s: ",infile);
    fprintf(stderr, "number of characters per key cannot be read or or wrong\n");
    fclose(fp);
    return NULL;
  }
  */

  // reading keys
  root = cur = tmp = NULL;
  for (i=0; i<(*pN); i++) {
    if ( fscanf(fp, "%s", word) != 1 ) {
      fprintf(stderr, "cannot read a word at %d/%d\n",i+1,(*pN));
      *pN = i;	// number of read keys so far
      break;
    }
    else {
      //check_and_correct_word(word, KEYLENGTH);

      // generate a new node
      tmp = generate_btnode(word);

      if ( root == NULL ) root = cur = tmp;
      else cur = insert_left_bcnode(cur,tmp);
    }
  }

  return root;
}

void bst_to_array(NODE *bst, NODE *arr[], int *count) {
  if (bst) {
    bst_to_array(bst->left, arr, count);


    *count = *count + 1;
    arr[*count] = bst;

    bst_to_array(bst->right, arr, count);
  }
}

int find_root_index(int numNodes, int start) {
  int height = ceil(log10(numNodes) / log10(2));
  int mid_pos;

  // printf("total : %d, height : %d start: %d\n", numNodes, height, start);

  if (numNodes - pow(2, (double)height - 1) + 1 < pow(2, (double)height - 2)) {
    mid_pos = (int)(numNodes - pow(2, (double)height - 1) + 1 + pow(2, (double)height - 2));
  } else {
    mid_pos = (int)pow(2, (double)height - 1);
  }

  if (numNodes == 1) mid_pos = 1;

  mid_pos = start + mid_pos;
  // printf("mid : %d\n\n", mid_pos);
  return mid_pos;
}

NODE *partition(NODE* lhbt, NODE* end, NODE **new_lhbt, NODE **new_end) {
  NODE *pivot = end;
  NODE *prev = NULL;
  NODE *cur = lhbt;
  NODE *last = pivot;

  while (cur != pivot) {
    if (comparekey(cur, pivot) < 1) {
      if (*new_lhbt == NULL) *new_lhbt = cur;
      prev = cur;
      cur = cur->left;
    } else {
      if (prev) prev->left = cur->left;
      NODE *tmp = cur->left;
      cur->left = NULL;
      last->left = cur;
      last = cur;
      cur = tmp;
    }
  }

  if (*new_lhbt == NULL) *new_lhbt = pivot;

  *new_end = last;

  return pivot;
}

NODE *getEnd(NODE *lhbt) {
  NODE *cur = lhbt;
  while (cur && cur->left) {
    cur = cur->left;
  }
  return cur;
}
