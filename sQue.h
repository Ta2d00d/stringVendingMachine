#include <stdlib.h>

typedef struct node
{
	char *data;
	struct node *next;
	struct node *prev;
}node;

node *queLine = NULL;

void enqueue(char *data)
{
	node *tmp;
	node *mov;
	tmp = malloc(sizeof(node));
	tmp->data = data;
	tmp->next = NULL;
	if(queLine == NULL)
	{
		queLine = tmp;
		tmp->prev = NULL;
	}
	else
	{
		mov = queLine;
		while(mov->next != NULL)
		{
			mov = mov->next;
		}
		mov->next = tmp;
		tmp->prev = mov;
	}
}

char* dequeue()
{
	char *d;
	node *tmp;
	if(queLine == NULL)
	{
		return NULL;
	}
	else
	{
		d = queLine->data;
		tmp = queLine;
		queLine = queLine->next;
		free(tmp);
	}
	return d;
}

char* inLine()
{
	char *d;
	if(queLine == NULL)
	{
		return NULL;
	}
	else
	{
		d = queLine->data;
	}
	return d;
}

char* endLine()
{
	char *d;
	node *tmp;

	if(queLine == NULL)
	{
		return NULL;
	}
	else
	{
		tmp = queLine;
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		d = tmp->data;
	}
	return d;
}
