/*a node for use in an LRU stack, which isa doubly linked list */
typedef struct s_node{
	struct s_node* prev;
	struct s_node* next;
	unsigned long pagenum;
}node;