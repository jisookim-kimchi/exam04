#ifndef TRIE_TREE_H
#define TRIE_TREE_H

#include <stdbool.h>

struct TrieNode
{
    struct TrieNode *children[26];
    bool is_end;
};

struct TrieNode *create_node(void);
void insert(struct TrieNode *root, char *word);
bool search(struct TrieNode *root, char *word);
void delete_word(struct TrieNode *root, char *word);
void destroy_tree(struct TrieNode *root);

#endif