#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"




void encode_in_file(const char *bitstring, char *op_file_name, int *freq)
{
    strcat(op_file_name, ".hc");
    int len = strlen(bitstring);
    int b_code_len = (len + 7) / 8; // Stores the length of the code when connvrerted to binary


    unsigned char *b_code = malloc(b_code_len);
    memset(b_code, 0, b_code_len);

    for(int i=0; i<len; i++)
    {
        int byte_index = i/8;
        int bit_index = 7 - (i % 8);

        if(bitstring[i] == '1')
        {
            b_code[byte_index] |= (1 << bit_index);
        }
    }

    int count =0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) 
        {
            // printf("Character '%c' (%d): %d times\n", i, i, freq[i]);
            count++;
        }
    }


    FILE *op_file  = fopen(op_file_name, "wb");
    if(!op_file)
    {
        perror("Error: The file cannot be created !");
        exit(1);
    }

    fwrite(&count, sizeof(int), 1, op_file);
    for(int i =0; i<MAX_CHAR; i++)
    {
        if(freq[i]>0)
        {
            fputc(i, op_file); // The character
            fwrite(&freq[i], sizeof(int), 1, op_file);// Its Frequency
        }
    }
    
    fwrite(&len, sizeof(int), 1, op_file);
    fwrite(b_code, sizeof(unsigned char), b_code_len, op_file);
    fclose(op_file);
    free(b_code);


    printf("\nCompressed file saved to %s\n", op_file_name);
}



void compress(const char * ip_file, char* op_file)
{
    printf("Initiating File Cmpression ...\n");
    FILE *in_file = fopen(ip_file, "r");    
    if(!in_file)
    {
        perror("Error: Unable to open the input file, check if the file exists");
        return;
    }

    fseek(in_file, 0, SEEK_END);
    long length = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    char *data = malloc(length+1);
    fread(data, 1, length, in_file);
    data[length] = '\0';
    fclose(in_file);


    int * freq = freq_calc(data);
    
    
    build_min_heap(freq);
    Node *node = huffman_tree();
    
    char code[256];
    huffman_encode(node, code, 0);



    char *encrypted;
    encrypted = huffman_encode_text(data);
    if (encrypted == NULL)
    {
        perror("Error: Encrypted text is NULL\n");
        exit(1);
    }
    // printf("\n\nEncrypted : %s", encrypted);
    free(node);
    encode_in_file(encrypted, op_file, freq);


}

void decompres_to_file(const char *bit_string, Node *root, const char *op_file_name)
{
    FILE *op = fopen(op_file_name, "w");
    if(!op)
    {
        perror("Error: Cannot open or create file");
        exit(1);
    }

    Node *current = root;
    for(int i =0; bit_string[i] != '\0'; i++)
    {
        current = (bit_string[i] == '0' ? current->left : current->right);

        if(!current->left && !current->right)
        {
            fputc(current->ch, op);
            current = root;
        }
    }


    fclose(op);
    printf("The file is decompressed and saved to %s.", op_file_name);
}

void decompress_from_file(const char *ip_file_name, const char* op_file_name)
{
    FILE *fp = fopen(ip_file_name, "rb");
    if (!fp)
    {
        perror("Error: Error in openning file");
        exit(1);
    }

    // --------------Tree Reconstruction from the file header ---------------------------
    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    int freq[MAX_CHAR]; 
    memset(freq, 0, sizeof(freq));

    for(int i=0; i<count; i++)
    {
        unsigned char ch = fgetc(fp);
        int f ;
        fread(&f, sizeof(int), 1, fp);
        freq[ch] = f;
    }


    build_min_heap(freq);

    Node *tree_root = huffman_tree();
    // ------------------End Tree Reconstruction, the tree is stored in tree_root -----------------
    int bit_len;
    fread(&bit_len, sizeof(int), 1, fp);

    int byte_len = (bit_len + 7) / 8;
    unsigned char *bin_code = malloc(byte_len);

    fread(bin_code, sizeof(unsigned char), byte_len, fp);
    fclose(fp);

    // printf("The binary code is %x", bin_code);

    if(!bit_len)
    {
        perror("Error: Counn't read Bit Length from file. Possibly a corroupted file");
        exit(1);
    }    
    char *code = malloc(bit_len+1);
    int bit_pos = 0;


    for(int i =0; i<byte_len; i++)
    {
        for(int j=7; j>=0; j--)
        {
            // printf("Byte %d: %02X\n", i, bin_code[i]);
            if(bit_pos < bit_len)
            {
                code[bit_pos++] = (bin_code[i] & (1 << j) ? '1' : '0');
            }
        }
    }


    code[bit_pos] = '\0';
    free(bin_code);

    // printf("The string code is %s", code);

    // *str_code = code;
    // *bit_num = bit_len;

    decompres_to_file(code, tree_root, op_file_name);
    free(tree_root);
}


int main(int argc, char *argv[])
{


    if(argc<4)
    {
        printf("Enough Data was not provided...\n");
        printf("Usasge:\n");
        printf(" %s -c <input_file.txt> <output_file_without_extension>\t\t # Compress \n", argv[0]);
        printf(" %s -d <input_file.hc> <ouput_file.txt>\t\t # Decompress\n", argv[0]);
        return(1);

    }
        if (strcmp(argv[1], "-c") == 0)
        {
            // char op_file[100] = argv[3];
            compress(argv[2], argv[3]);
        }
        else if(strcmp(argv[1], "-d") == 0)
        {
            decompress_from_file(argv[2], argv[3]);
        }
        else
        {
            printf("Unknown Comamnd\n");
        }
    

    return 0;
}

// int main(int argc, char *argv[]) {
//     if (argc < 3) {
//         printf("Usage:\n");
//         printf("  %s -c <input_file> <output_file>   # Compress\n", argv[0]);
//         printf("  %s -d <input_file> <output_file>   # Decompress\n", argv[0]);
//         return 1;
//     }

//     if (strcmp(argv[1], "-c") == 0) {
//         compress_file(argv[2], argv[3]);
//     } else if (strcmp(argv[1], "-d") == 0) {
//         decompress_file(argv[2], argv[3]);
//     } else {
//         printf("Unknown command '%s'\n", argv[1]);
//     }

//     return 0;
// }