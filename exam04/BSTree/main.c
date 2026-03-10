#include "Tree.h"
#include <stdlib.h>
#include <stdio.h>

TreeNode    *new_node(int val)
{
    TreeNode    *new = malloc(sizeof(TreeNode));
    if (!new)
        return NULL;
    new->val = val;
    new->l = NULL;
    new->r = NULL;
    return new;
}

TreeNode    *insert(TreeNode **root, int val)
{
    if (!*root)
    {
        *root = new_node(val);
        return *root;
    }

    if (val <= (*root)->val)
    {
        insert(&(*root)->l, val);
    }
    else if (val > (*root)->val)
    {
        insert(&(*root)->r, val);
    }
    return *root;
}

void printBST(TreeNode *root)
{
    if (!root)
        return;
    printBST(root->l);
    printf("%d ", root->val);
    printBST(root->r);
}

void    destroy_tree(TreeNode *n)
{
    if (!n)
        return;
    destroy_tree(n->l);
    destroy_tree(n->r);
    free(n);
}

int main(int ac, char **av)
{
    TreeNode    *root = NULL;
    int i = 1;
    while (i < ac)
    {
        insert(&root, atoi(av[i]));
        i++;
    }
    printBST(root);
    printf("\n");
    destroy_tree(root);
}