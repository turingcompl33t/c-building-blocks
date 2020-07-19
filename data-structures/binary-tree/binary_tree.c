// binary_tree.c
// Generic binary search tree data structure.

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "binary_tree.h"

// ----------------------------------------------------------------------------
// Internal Declarations

typedef struct treenode
{
	// User provided key / value pair
	void* key; 
	void* value;

	// Pointers to parent, left / right child nodes. 
	struct treenode* parent;
	struct treenode* left;
	struct treenode* right;
} treenode_t;

static treenode_t* treenode_new(void* key, void* value); 
static void treenode_delete(treenode_t* node);

treenode_t* tree_find_helper(
	treenode_t* find_at, 
	void* key, 
	comparator_f comparator); 
void tree_insert_helper(
	treenode_t* insert_at, 
	treenode_t* new_node, 
	comparator_f comparator);
bool tree_remove_helper(
	treenode_t* remove_at,
	void* key,
	binary_tree_t* tree); 
void tree_destroy_helper(
	treenode_t *destroy_at, 
	deleter_f deleter);
void tree_iterator(treenode_t* current_node, iterator_f fn);

void tree_repair(treenode_t* repair_at, binary_tree_t* tree);
void tree_repair_helper0(treenode_t* repair_at, binary_tree_t* tree);
void tree_repair_helper1(treenode_t* repair_at, binary_tree_t* tree);
void tree_repair_helper2(treenode_t* repair_at, binary_tree_t* tree);
void tree_repair_helper3(treenode_t* repair_at, binary_tree_t* tree);

// ----------------------------------------------------------------------------
// Exported

binary_tree_t* tree_new(
	comparator_f comparator, 
	deleter_f deleter)
{
	binary_tree_t* tree = malloc(sizeof(binary_tree_t));
	if (NULL == tree)
	{
		return NULL;
	}
	
	tree->comparator = comparator;
	tree->deleter    = deleter;

	tree->count = 0;

	tree->root = NULL;

	return tree; 
}

void tree_delete(binary_tree_t *tree)
{
	if (NULL == tree)
	{
		return;
	}

	if (tree->root != NULL) 
	{
		// nonempty tree
		tree_destroy_helper(tree->root, tree->deleter); 
	} 

	free(tree);
	tree = NULL;
}

bool tree_insert(
	binary_tree_t* tree, 
	void* key, 
	void* value, 
	void** out)
{
	if (out != NULL)
	{
		*out = NULL;
	}

	if (NULL == tree)
	{
		return false;
	}

	// attempt to find the key in the tree prior to creating new node
	// no need to construct a new node if we don't need it for insert
	treenode_t* existing = tree_find_helper(tree->root, key, tree->comparator);
	if (existing != NULL)
	{
		// the key is already present in the tree;
		// update the associated value and return previous
		*out = existing->value;
		existing->value = value;
		return true;
	}

	// the key does not exist in the tree, must insert

	treenode_t* node = treenode_new(key, value);
	if (NULL == node)
	{
		return false;
	}
		
	if (NULL == tree->root) 
	{
		// tree is empty, insert at root 
		tree->root = node; 
	}
	else
	{
		// otherwise, need to recurse
		tree_insert_helper(tree->root, node, tree->comparator); 
	}

	tree->count++;	
	return true;
}

bool tree_remove(binary_tree_t* tree, void* key)
{
	if (NULL == tree || NULL == tree->root)
	{
		// invalid or empty tree, nothing to do
		return false;
	}

	return tree_remove_helper(tree->root, key, tree); 
}

void* tree_find(binary_tree_t* tree, void* key)
{
	if (NULL == tree || NULL == tree->root)
	{
		// invalid or empty tree
		return NULL;
	}

	treenode_t* found = tree_find_helper(tree->root, key, tree->comparator);

	return (found != NULL) ? found->value : NULL;
}

size_t tree_count(binary_tree_t* tree)
{
	return (NULL == tree) ? 0 : tree->count;
}

void tree_for_each(binary_tree_t* tree, iterator_f iter)
{
	if (NULL == tree || NULL == iter)
	{
		return;
	}

	tree_iterator(tree->root, iter);
}

// ----------------------------------------------------------------------------
// Internal

static treenode_t* treenode_new(void* key, void* value)
{
	treenode_t* node = malloc(sizeof(treenode_t));
	if (NULL == node)
	{
		return NULL;
	}

	node->key    = key;
	node->value  = value;
	node->parent = NULL;
	node->left   = NULL;
	node->right  = NULL;

	return node; 
}

static void treenode_delete(treenode_t* node)
{
	free(node);
}

treenode_t* tree_find_helper(
	treenode_t* find_at, 
	void* key, 
	comparator_f comparator) 
{
	if (NULL == find_at)
	{
		return NULL;
	}

	const ordering_t cmp = comparator(key, find_at->key);
	if (EQUAL == cmp)
	{
		// match
		return find_at;
	}
	else if (LESS == cmp) 
	{
		// query less than the current key; attempt recurse on left child
		if (NULL == find_at->left) 
		{
			return NULL;
		} 
		else 
		{
			return tree_find_helper(find_at->left, key, comparator); 
		}
	} 
	else 
	{
		// query greater than current key; attempt recurse on right child
		if (NULL == find_at->right) 
		{
			return NULL;
		} 
		else 
		{
			return tree_find_helper(find_at->right, key, comparator); 
		}
	}
}

void tree_insert_helper(
	treenode_t* insert_at, 
	treenode_t* new_node, 
	comparator_f comparator) 
{
	const ordering_t cmp = comparator(new_node->key, insert_at->key);
	assert(cmp != EQUAL);

	if (LESS == cmp) 
	{
		// new key is less than current key
		if (insert_at->left == NULL) 
		{
			// current node has no left child, found our insert location
			insert_at->left = new_node;
			new_node->parent = insert_at;
		} 
		else 
		{
			// otherwise, recurse on left subtree
			tree_insert_helper(insert_at->left, new_node, comparator); 
		}
	} 
	else 
	{
		// new node is greater than current key 
		if (insert_at->right == NULL) 
		{
			// current node has no right child, found our insert location
			insert_at->right = new_node;
			new_node->parent = insert_at; 
		} 
		else 
		{
			tree_insert_helper(insert_at->right, new_node, comparator); 
		}
	}
}

bool tree_remove_helper(
	treenode_t* remove_at,
	void* key,
	binary_tree_t* tree)
{
	if (NULL == remove_at)
	{
		return false;
	}

	const ordering_t cmp = tree->comparator(key, remove_at->key);
	if (EQUAL == cmp) 
	{
		// found a matching key for query;
		// remove node from tree and repair structure 
		tree_repair(remove_at, tree); 

		// destroy value and node
		tree->deleter(remove_at->value);
		treenode_delete(remove_at);
		tree->count--;
		return true; 
	}

	if (LESS == cmp) 
	{
		if (remove_at->left == NULL) 
		{
			// key not found 
			return false;
		} 
		else 
		{
			// recurse on the left subtree
			return tree_remove_helper(remove_at->left, key, tree);
		}
	} 
	else 
	{
		if (remove_at->right == NULL) 
		{
			// key not found 
			return false;
		} 
		else 
		{
			// recurse on the right subtree
			return tree_remove_helper(remove_at->right, key, tree); 
		}
	}
}

void tree_repair(treenode_t* repair_at, binary_tree_t* tree) 
{
	if (NULL == repair_at->left && NULL == repair_at->right) 
	{
		// easy case, no repair to be done
		tree_repair_helper0(repair_at, tree); 
	} 
	else if (repair_at->left != NULL && NULL == repair_at->right) 
	{
		// has valid left child, no right child 
		tree_repair_helper1(repair_at, tree);
	} 
	else if (NULL == repair_at->left && repair_at->right != NULL) 
	{
		// has no left child, valid right child 
		tree_repair_helper2(repair_at, tree);
	} 
	else 
	{
		// hard case, removed node has valid left and right children 
		tree_repair_helper3(repair_at, tree);
	}
}

// repair Case: left->NULL, right->NULL
void tree_repair_helper0(treenode_t* repair_at, binary_tree_t* tree)
{
	if (repair_at->parent != NULL) 
	{
		if (repair_at->parent->left == repair_at) 
		{
			// current is left child of parent 
			repair_at->parent->left = NULL;
		} 
		else 
		{
			// current is right child of parent
			repair_at->parent->right = NULL; 
		}	
	} 
	else 
	{
		// repairing at root node
		// no children, tree is now empty 
		tree->root = NULL;  
	}
}

// repair Case: left->VALID, right->NULL
void tree_repair_helper1(treenode_t* repair_at, binary_tree_t* tree) 
{
	if (repair_at->parent != NULL) 
	{
		repair_at->left->parent = repair_at->parent; 
		if (repair_at->parent->left == repair_at) 
		{
			// current is left child of parent 
			repair_at->parent->left = repair_at->left;
		} 
		else 
		{
			// current is right child of parent
			repair_at->parent->right = repair_at->left; 
		}
	} 
	else 
	{
		// repairing at root node 
		tree->root = repair_at->left; 
		repair_at->left->parent = NULL; 
	}
}

// repair Case: left->NULL, right->VALID
void tree_repair_helper2(treenode_t* repair_at, binary_tree_t* tree) 
{
	if (repair_at->parent != NULL) 
	{
		repair_at->right->parent = repair_at->parent; 
		if (repair_at->parent->left == repair_at) 
		{
			// current is left child of parent 
			repair_at->parent->left = repair_at->right;
		} 
		else 
		{
			// current is right child of parent
			repair_at->parent->right = repair_at->right; 
		}
	} 
	else 
	{
		// repairing at root node 
		tree->root = repair_at->right;
		repair_at->right->parent = NULL; 
	}
}

// repair Case: left->VALID, right->VALID
void tree_repair_helper3(treenode_t* repair_at, binary_tree_t* tree) 
{
	// locate the largest node in subtree rooted at left child of removed node 
	treenode_t* swap = repair_at->left;
	while (swap->right != NULL)
	{
		swap = swap->right; 
	} 

	if (swap != repair_at->left) 
	{
		// non-direct left child of the removed node 
		// if it is a direct child, we dont want to make self-reference 
		swap->left = repair_at->left; 
	} 
	
	// now we have the swap node located, splice it in 
	swap->right = repair_at->right; 

	if (repair_at->parent != NULL) 
	{
		swap->parent = repair_at->parent;
		if (repair_at->parent->left == repair_at) 
		{
			repair_at->parent->left = swap;
		} 
		else 
		{
			repair_at->parent->right = swap; 
		}
	} 
	else 
	{
		// repairing at root node 
		tree->root = swap; 
		swap->parent = NULL; 
	}
}

void tree_destroy_helper(treenode_t* destroy_at, deleter_f deleter) 
{
	if (NULL == destroy_at)
	{
		return;
	}

	// recurse on the left subtree
	tree_destroy_helper(destroy_at->left, deleter);

	// recurse on the right subtree
	tree_destroy_helper(destroy_at->right, deleter);

	// destroy the current node 
	deleter(destroy_at->value);
	free(destroy_at);
	destroy_at = NULL; 
}

void tree_iterator(treenode_t* current_node, iterator_f fn)
{
	if (NULL == current_node)
	{
		return;
	}

	// inorder tree traversal

	// recurse on the left subtree
	tree_iterator(current_node->left, fn);

	// process current node
	fn(current_node->key, current_node->value);

	// recurse on the right subtree
	tree_iterator(current_node->right, fn); 
}