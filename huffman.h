#ifndef HUFFMAN_H
#define HUFFMAN_H
#define MAX_CHAR 256
typedef struct Node 
{
    unsigned char ch;
    int f;

    struct Node *left;
    struct Node *right;

}Node;


int* freq_calc(char *text);
void freq_print();
int less_than_with_tie_breaker(Node *a, Node *b);
void min_heap(Node *new_node);
void build_min_heap(int *freq);
void heapify(int i);
Node *minimum();
Node *huffman_tree();
void huffman_encode(Node *root, char *code, int depth);
char *huffman_encode_text(char *text);
void huffman_decode(Node *root, const char *encoded);

#endif