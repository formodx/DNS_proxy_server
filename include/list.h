#ifndef COMMON_H
#define COMMON_H


#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


struct list{
	char **names;
	size_t count;
};


void create_list(struct list *, size_t);
void delete_list(struct list *);
bool find_string_in_list(struct list *, const char *);


#endif