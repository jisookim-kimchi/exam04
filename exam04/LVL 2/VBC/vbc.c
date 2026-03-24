#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

typedef struct node
{
    enum
    {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        if (n->l)
            destroy_tree(n->l);
        if (n->r)
            destroy_tree(n->r);
    }
    free(n);
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

struct node    *parse_expr(char *s)
{
    if (!s)
        return NULL;

    struct node *ret = parse_low_priority(&s);
    if (!ret)
        return NULL;

    // After parsing, there should be nothing left
    if (*s != '\0')
    {
        destroy_tree(ret);
        unexpected(*s);
        return NULL;
    }

    return ret;
}



int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    char *s = argv[1];

    struct node *tree = parse_expr(s);
    if (!tree)
        return 1;

    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return 0;
}
