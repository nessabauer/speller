// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Guardando o tamanho para retornar na função SIZE
unsigned int SIZE = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    //Obtendo o hash value
    int h = hash(word);

    //Criando cursor para aquela linha
    node *cursor = table[h];

    while (cursor != NULL)
    {
        //comparando as palavras
        if (strcasecmp(word, cursor->word) == 0)
        {
            //Iguais retorna TRUE
            return true;
        }

        //Movendo o cursor conforme video explicativo
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Função HASH copiada de: http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c = 0;

    while (c == *word++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Abrindo o dicionário
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return false;
    }

    //Variável para onde vai a palavra
    char word[LENGTH + 1];

    //Leitura do dicionario para achar a palavra até o fim do arquivo (EOF = fim do arquivo)
    while (fscanf(input, "%s", word) != EOF)
    {
        //Alocando memório para o novo nó
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //Copiando a palavra
        strcpy(n->word, word);

        //Obtendo o hash value
        int h = hash(word);

        //Verificando se já tem alguma palavra nesse nó
        if (table[h] == NULL)
        {
            table[h] = n;
            n->next = NULL;
        }
        else
        {
            n->next = table[h];
            table[h] = n;
        }

        SIZE++;
    }


    //Fechando o dicionário
    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return SIZE;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Varrendo todos os buckets
    for (int i = 0 ; i < N ; i++)
    {
        //Lendo até o último nó
        while (table[i] != NULL)
        {
            //Salvando o link a ser excluido
            node *temp = table[i];
            //salvando o proximo link
            table[i] = table[i]->next;
            //liberando a memoria
            free(temp);
        }
    }

    return true;
}
