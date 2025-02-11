#ifndef COMMON_H
#define COMMON_H


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct list{
	char **names;
	int count;
};


void create_list(struct list *, int);
void delete_list(struct list *);
bool find_string_in_list(struct list *, const char *);


#endif