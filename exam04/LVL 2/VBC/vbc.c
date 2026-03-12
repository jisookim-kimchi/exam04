#include <stdio.h>
#include <stdlib.h>    // MOD: on utilise <stdlib.h> pour calloc/free au lieu de <malloc.h>
#include <ctype.h>
#include <unistd.h>

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




struct node *parse_low_priority(char **s);
struct node *parse_mid_priority(char **s);
struct node *parse_highest_priority(char **s);


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

//parsing 
//가장 높은 우선순위 숫자 또는 괄호
struct node *parse_highest_priority(char **s)
{
    if (**s == '\0')  // 입력 끝이면
    {
        unexpected(**s);
        return NULL;
    }

    if (isdigit((unsigned char)**s))
    {
        struct node n;
        n.type = VAL;
        n.val = **s - '0';
        n.l = n.r = NULL;
        (*s)++;
        return new_node(n);
    }
    
    if (**s == '(')
    {
        (*s)++;

        struct node *low = parse_low_priority(s);
        if (!low)
        {
            return NULL;
        }

        if (**s == '\0') // 괄호가 닫히지 않고 끝남
        {
            destroy_tree(low);
            unexpected(**s);
            return NULL;
        }

        if (**s != ')') 
        {
            destroy_tree(low);
            unexpected(**s);
            return NULL;
        }

        (*s)++;
        return low;
    }

    // 숫자도 '('도 아닌 이상한 문자
    unexpected(**s);
    return NULL;
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

    struct node *tree = parse_expr(&s);
    if (!tree)
        return 1;

    printf("%d\n", eval(tree));
    destroy_tree(tree);
    return 0;
}
