#include "TrieTree.h"
#include <stdlib.h>
#include <stdio.h>

struct TrieNode *create_node(void)
{
    struct TrieNode *ret = calloc(1, sizeof(struct TrieNode));
    if (!ret)
        return (NULL);
    return (ret);
}

void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *curr = root;
    printf("%p, %p\n", root ,curr);
    while (*word)
    {
        if (!curr->children[*word - 'a'])
            curr->children[*word - 'a'] = create_node();
        curr = curr->children[*word - 'a'];
        word++;
    }
    curr->is_end = true;
}

bool search(struct TrieNode *root, char *word)
{
    struct TrieNode *curr = root;
    while (*word)
    {
        if (!curr->children[*word - 'a'])
            return (false);
        curr = curr->children[*word - 'a'];
        word++;
    }
    return (curr->is_end);
}

void delete_word(struct TrieNode *root, char *word)
{
    struct TrieNode *curr = root;
    while (*word)
    {
        if (!curr->children[*word - 'a'])
            return;
        curr = curr->children[*word - 'a'];
        word++;
    }
    curr->is_end = false;
}

void destroy_tree(struct TrieNode *root)
{
    if (!root)
        return;
    for (int i = 0; i < 26; i++)
    {
        if (root->children[i])
            destroy_tree(root->children[i]);
    }
    free(root);
}

int main(int ac, char *av[])
{
    if (ac < 2)
        return -1;
    struct TrieNode *root = create_node();
    if (!root)
        return -1;
    insert(root, av[1]);
    if (search(root, "hello"))
        printf("found\n");
    else
        printf("not found\n");

    insert(root, av[2]);
    if(search(root, "world"))
        printf("found\n");
    else
        printf("not found\n");

    if (search(root, "hal"))
        printf("found\n");
    else
        printf("not found\n");
    destroy_tree(root);
    return 0;
}