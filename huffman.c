#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 256
#include "huffman.h"

int freq[MAX_CHAR] = {0};
char *codes[256] = {0};

// int et_pos;




// typedef struct Node 
// {
//     unsigned char ch;
//     int f;

//     struct Node *left;
//     struct Node *right;

// }Node;

Node *heap[MAX_SIZE];
int heapsize = 0;

int *freq_calc(char *text){

    for(int i =0; text[i]; i++)
    {
        freq[(unsigned char)text[i]] ++;
        // printf("Character '%c' (%d): %d times\n", text[i], text[i], freq[(unsigned char)text[i]]);
    }
    return freq;
}

void freq_print()
{
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) 
        {
            printf("Character '%c' (%d): %d times\n", i, i, freq[i]);
        }
    }
}

int less_than_with_tie_breaker(Node *a, Node *b)
{
    if(a->f != b->f)
    {
        return a->f < b->f;
    }
    return a->ch < b->ch;
}


void min_heap(Node *new_node)
{
    int i = heapsize++;
    while(i && less_than_with_tie_breaker(new_node, heap[(i-1)/2])) // heap[(i-1)/2] --> Returns the parent node of the element at index i
    {
        heap[i]  = heap[(i-1)/2];
        i = (i-1)/2;
    }

    heap[i] = new_node;
}

void build_min_heap(int *freq)
{
    for(int i =0; i<256; i++)
    {
        if(freq[i]>0)
        {
            Node *new_node = (Node *)malloc(sizeof(Node));
            new_node->ch = (unsigned char)i;
            new_node->f = freq[i];
            new_node->left = NULL;
            new_node->right = NULL;
            min_heap(new_node);
        }
    }
}


void heapify(int i)
{
    int least = i;
    int l = 2*i+1;
    int r = 2*i+2;

    if(l<heapsize && less_than_with_tie_breaker(heap[l], heap[least]))
    {
        least = l;
    }

    if(r<heapsize && less_than_with_tie_breaker(heap[r], heap[least]))
    {
        least = r;
    }



    if(least != i)
    {
        Node *temp = heap[i];
        heap[i] = heap[least];
        heap[least] = temp;
        heapify(least);
    }

}


Node *minimum()
{
    Node *min = heap[0];
    heap[0] = heap[--heapsize];
    heapify(0);
    return min;

}




Node *huffman_tree()
{
    while(heapsize>1)
    {
        Node *left = minimum();
        Node *right = minimum();


        Node *new_node = (Node*)malloc(sizeof(Node));
        new_node->ch = '\0'; // Null Character
        new_node->f = left->f + right->f;
        new_node->left = left;
        new_node->right = right;


        min_heap(new_node);
    }

    return minimum();
}


void huffman_encode(Node *root, char *code, int depth)
{
    
    if(root->left == NULL && root->right == NULL)
    {
        code[depth] = '\0';
        // printf("'%c' -> %s\n", root->ch, code);
        codes[(unsigned char)root->ch] = strdup(code);
        return ;
    }


    if(root->left)
    {
        code[depth] = '0';
        huffman_encode(root->left, code, depth+1);
    }


    if(root->right)
    {
        code[depth] = '1';
        huffman_encode(root->right, code, depth+1);
    }

}


char *huffman_encode_text(char *text)
{
    int et_pos = 0;
    char *encoded_text;
    int et_buffer_size = 128;
    encoded_text = malloc(et_buffer_size);
    for(int i =0; i<strlen(text); i++)
    {
        const char *code = codes[(unsigned char)text[i]];
        int len = strlen(code);

        if(et_pos + len >= et_buffer_size)
        {
            et_buffer_size *=2;
            encoded_text = realloc(encoded_text, et_buffer_size);
        }
        strcpy(&encoded_text[et_pos], code);
        et_pos += len;
    }

    encoded_text[et_pos] = '\0';
    // printf("\n\n\n Encoded text is : %s\n\n", encoded_text);
    return encoded_text;
}


void huffman_decode(Node *root, const char *encoded)
{
   
    Node *current = root;
    printf("\n");
    for(int i =0; encoded[i] !=0 ; i++)
    {
        if(encoded[i] == '0')
        {
            current = current->left;
        }

        if(encoded[i] == '1')
        {
            current = current->right;
        }


        if(!current->left && !current->right)
        {
            printf("%c", current->ch);
            current = root; 
        }
    }
    printf("\n\n");
}


// void main()
// {
//     // encoded_text = malloc(et_buffer_size);
//     // et_pos = 0;

//     char *text = "AAAABBBCCD";

//     freq_calc(text);
//     // freq_print();
//     build_min_heap();

//     char code[256];
//     Node *node = huffman_tree();


//     printf("Huffman Codes are !!\n");
//     huffman_encode(node, code, 0);


//     printf("Enocoding text... ...");
//     char *encoded_text = huffman_encode_text(text);
//     printf("\n\nEncoded Text: %s \n\n",encoded_text);


//     printf("Decoding Text ... ... ...");
//     huffman_decode(node, encoded_text);

//     // return 0;
// }


