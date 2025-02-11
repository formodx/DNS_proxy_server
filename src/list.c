#include "list.h"


void create_list(struct list *l, int cnt){
    l->names = malloc(cnt * sizeof(char *));
    l->count = cnt;

    for(int i=0; i<cnt; ++i) l->names[i] = NULL;
}


void delete_list(struct list *l){
    for(int i=0; i<l->count; ++i) free(l->names[i]);

    free(l->names);
    l->count = 0;
}


bool find_string_in_list(struct list *l, const char *s){
    for(int i=0; i<l->count; ++i){
        if(!strcmp(l->names[i], s)){
            return true;
        }
    }

    return false;
}