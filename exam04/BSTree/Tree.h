#ifndef TREE_H
#define TREE_H

typedef struct TreeNode
{
    int val;
    struct TreeNode *l;
    struct TreeNode *r;
}   TreeNode;

TreeNode    *new_node(int val);
void    destroy_tree(TreeNode *n);

#endif