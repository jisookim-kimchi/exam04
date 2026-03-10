#include "Expression_Tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct TreeNode    *new_node(struct TreeNode n)
{
    struct TreeNode *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(struct TreeNode *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

int eval_tree(struct TreeNode *tree)
{
    if (!tree)
        return 0;
    if (tree->type == VAL)
        return tree->val;
    if (tree->type == ADD)
        return eval_tree(tree->l) + eval_tree(tree->r);
    if (tree->type == MULTI)
        return eval_tree(tree->l) * eval_tree(tree->r);
    return 0;
}

struct TreeNode *parse_low_priority(char **s)
{
    struct TreeNode *Left = parse_mid_priority(s);
    if (!Left)
        return NULL;
    while (**s == '+')
    {
        (*s)++;
        struct TreeNode *Right = parse_mid_priority(s);
        if (!Right)
        {
            destroy_tree(Left);
            return NULL;
        }
        struct TreeNode n = {.type = ADD, .l = Left, .r = Right};
        Left = new_node(n);
        if (!Left)
            return NULL;
    }
    return Left;
}

struct TreeNode *parse_mid_priority(char **s)
{
    struct TreeNode *Left = parse_highest_priority(s);
    if (!Left)
        return NULL;
    while (**s == '*')
    {
        (*s)++;
        struct TreeNode *Right = parse_highest_priority(s);
        if (!Right)
        {
            destroy_tree(Left);
            return NULL;
        }
        struct TreeNode n = {.type = MULTI, .l = Left, .r = Right};
        Left = new_node(n);
        if (!Left)
            return NULL;
    }
    return Left;
}


struct TreeNode *parse_highest_priority(char **s)
{
    if (isdigit(**s))
    {
        struct TreeNode n = {.type = VAL, .val = **s - '0', .l = NULL, .r = NULL};
        (*s)++;
        return new_node(n);
    }
    if (**s == '(')
    {
        (*s)++;
        struct TreeNode *low = parse_low_priority(s);
        if (!low)
            return NULL;
        if (**s != ')')
        {
            destroy_tree(low);
            return NULL;
        }
        (*s)++;
        return low;
    }
    return NULL;
}

struct TreeNode *parse_expr(char *s)
{
    if (s == NULL)
        return NULL;
    if (*s)
    {
        struct TreeNode *low = parse_low_priority(&s);
        if (!low)
            return NULL;
        if (*s)
        {
            unexpected(*s);
            destroy_tree(low);
            return NULL;
        }
        return low;
    }
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

int main (int ac, char *av[])
{
    if (ac != 2)
        return 1;
    struct TreeNode *tree = parse_expr(av[1]);
    if (!tree)
        return 1;
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return 0;
}