#include "argo.h"
#include <stdlib.h>  // malloc, realloc, free

/* 
Assignment name: argo
Expected files: argo.c
Allowed functions: getc, ungetc, printf, malloc, calloc, realloc, free, isdigit, fscanf, write
-----------------
Write a function argo that will parse a json file in the structure declared in argo.h:

int	argo(json *dst, FILE *stream);

	dst	- is the pointer to the AST that you will create
	stream	- is the file to parse (man FILE)

Your function will return 1 for success and -1 for failure.
If an unexpected token is found you will print the following message in stdout:
"Unexpected token '%c'\n"
or if the token is EOF:
"Unexpected end of input\n"

Only handle numbers, strings and maps.
Numbers will only be basic ints like in scanf("%d")
Handle escaping in the strings only for backslashes and quotation marks (no \n \u ...)
Don't handle spaces -> consider them as invalid tokens.

In case of doubt how to parse json, read rfc8259. But you won't need it as the format is simple. Tested with the main, the output should be exactly the same as the input (except for errors).
There are some functions in argo.c that might help you.

Examples that should work:

$> echo -n '1' | ./argo /dev/stdin | cat -e
1$
$> echo -n '"bonjour"' | ./argo /dev/stdin | cat -e
"bonjour"$
$> echo -n '"escape! \" "' | ./argo /dev/stdin | cat -e
"escape! \" "$
$> echo -n '{"tomatoes":42,"potatoes":234}' | ./argo /dev/stdin | cat -e
{"tomatoes":42,"potatoes":234}$
$> echo -n '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' | ./argo /dev/stdin | cat -e
{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}$
$> echo -n '"unfinished string' | ./argo /dev/stdin | cat -e
unexpected end of input$
$> echo -n '"unfinished string 2\"' | ./argo /dev/stdin | cat -e
unexpected end of input$
$> echo -n '{"no value?":}' | ./argo /dev/stdin | cat -e
unexpected token '}'$-
 */

 // 읽고(getc) 되돌리기(ungetc)
int	peek(FILE *stream)
{
  int	c = getc(stream);
  ungetc(c, stream);
  return c;
}

void	unexpected(FILE *stream)
{
  if (peek(stream) != EOF)
    printf("Unexpected token '%c'\n", peek(stream));
  else
    printf("Unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
    (void)getc(stream);
    return 1;
  }
  return 0;
}

int	expect(FILE *stream, char c)
{
  if (accept(stream, c))
    return 1;
  unexpected(stream);
  return 0;
}

int	parse_int(json *dst, FILE *stream)
{
  int n = 0;

  if (fscanf(stream, "%d", &n) != 1)
    return -1;
  dst->type = INTEGER;
  dst->integer = n;
  return (1);
}

char *get_str(FILE *stream)
{
  char *res = calloc(4096, sizeof(char));
  int i = 0;
  (void)getc(stream); // 여는 따옴표 '"' 소비

  while (1)
  {
    int c = getc(stream);

    if (c == '"')
      break;
    if (c == EOF)
	{
      unexpected(stream);
      free(res);
      return NULL;
    }
    if (c == '\\')
	{
      c = getc(stream);
      if (c == EOF)
	  {
        unexpected(stream);
        free(res);
        return NULL;
      }
    }
    res[i++] = c;
  }
  return (res);
}

/* parser 선언 (상호 참조 대비) */
int parser(json *dst, FILE *stream);

int parse_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.size = 0;
	dst->map.data = NULL;
	char c = getc(stream);

	if (peek(stream) == '}')
		return 1;

	while (1)
	{
		c = peek(stream);
		if (c != '"')
		{
			unexpected(stream);
			return -1;
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		pair *current = &dst->map.data[dst->map.size];
		current->key = get_str(stream);
		if (current->key == NULL)
			return -1;
		dst->map.size++;
		if (expect(stream, ':') == 0)
			return -1;
		if (argo(&current->value, stream) == -1)
			return -1;
		c = peek(stream);
		if (c == '}')
		{
			accept(stream ,c);
			break ;
		}
		if (c == ',')
			accept(stream, ',');
		else
		{
			unexpected(stream);
			return -1;
		}
	}
	return 1;
}

int parser(json *dst, FILE *stream)
{
  int c = peek(stream);

	if (c == EOF)
	{
    unexpected(stream);
    return -1;
  }
  if (isdigit(c) || c == '-')
    return (parse_int(dst, stream));
  else if (c == '"')
  {
    dst->type = STRING;
    dst->string = get_str(stream);
    if (dst->string == NULL)
      return (-1);
    return (1);
  } 
  else if (c == '{')
    return (parse_map(dst, stream));
  else
  {
    unexpected(stream);
    return -1;
  }
}

int argo(json *dst, FILE *stream)
{
  if (parser(dst, stream) == -1)
    return -1;

  if (peek(stream) != EOF) {
    unexpected(stream);
    free_json(*dst);
    return -1;
  }
  return 1;
}
