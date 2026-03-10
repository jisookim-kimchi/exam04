#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H


typedef struct TreeNode
{
    enum
    {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct TreeNode *l;
    struct TreeNode *r;
};

struct TreeNode *createNode(struct TreeNode n);
void destroyTree(struct TreeNode *root);
int eval_tree(struct TreeNode *tree);
void    unexpected(char c);
int    accept(char **s, char c);
int    expect(char **s, char c);
struct TreeNode *parse_low_priority(char **s);
struct TreeNode *parse_mid_priority(char **s);
struct TreeNode *parse_highest_priority(char **s);
struct TreeNode *parse_expr(char *s);

#endif