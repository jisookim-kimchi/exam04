#include <stdio.h>
#include <stdlib.h>    // MOD: on utilise <stdlib.h> pour calloc/free au lieu de <malloc.h>
#include <ctype.h>

struct node *parse_mid_priority(char **s);

struct node 
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
};

struct node *new_node(struct node n)
{
    struct node *ret = calloc(1, sizeof(*ret));
    if (!ret)
        return NULL;
    *ret = n;
    return ret;
}

void destroy_tree(struct node *n)
{
    if (!n)
        return;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

// 덧셈 우선순위

struct node *parse_low_priority(char **s)
{
    struct node *left = parse_mid_priority(s);
    if (!left)
        return NULL;
    
    while (**s == '+')
    {
        (*s)++;
        
        struct node *right = parse_mid_priority(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        struct node n = {.type = ADD, .l = left, .r = right};
        left = new_node(n);
        if (!left)
            return NULL;
    }
    
    return left;
}

//parsing 
//가장 높은 우선순위 숫자 또는 괄호
struct node *parse_highest_priority(char **s)
{
    if (isdigit((unsigned char)**s))
    {
        struct node n = {.type = VAL, .val = **s - '0', .l = NULL, .r = NULL};
        (*s)++;
        return new_node(n);
    }
    
    if (**s == '(')
    {
        (*s)++;

        struct node *low = parse_low_priority(s);
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


// 곱셈 우선순위
struct node *parse_mid_priority(char **s)
{
    // 1. 첫 번째 인수 가져오기
    struct node *left = parse_highest_priority(s);
    if (!left)
        return NULL;
    
    // 2. '*' 가 있는 동안 반복
    while (**s == '*')
    {
        (*s)++;
        
        struct node *right = parse_highest_priority(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        struct node n = {.type = MULTI, .l = left, .r = right};
        left = new_node(n);
        if (!left)
            return NULL;
    }
    
    return left;
}