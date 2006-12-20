#ifndef LIST_H_
#define LIST_H_

typedef struct list_s list_t;
typedef struct list_node_s list_node_t;

struct list_node_s {
        void *next;
        void *element;
};

struct list_s {
        int nb_elt;
        list_node_t *node;
};

int list_init (list_t **li);
int list_uninit (list_t *li);
int list_eol (list_t *li, int i);
void * list_get (list_t *li, int pos);
int list_remove (list_t *li, int pos);
int list_add (list_t *li, void *el, int pos);

#endif /*LIST_H_*/
