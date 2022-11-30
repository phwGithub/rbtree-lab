#include "rbtree.h"
#include <stdio.h>

static void print_rbtree(rbtree *t, node_t *root) {
    if(root == t->nil) {
        return;
    }

    print_rbtree(t, root->left);
    printf("%d %d\n", root->key, root->color);
    print_rbtree(t, root->right);
}

static void left_rotate(rbtree *t, node_t *node_x) {
  node_t *node_y = node_x->right;
  node_x->right = node_y->left;

  if(node_y->left != t->nil) {
    node_y->left->parent = node_x;
  }

  node_y->parent = node_x->parent;

  if(node_x->parent == t->nil) {
    t->root = node_y;
  } else if(node_x == node_x->parent->left) {
    node_x->parent->left = node_y;
  } else {
    node_x->parent->right = node_y;
  }

  node_y->left = node_x;
  node_x->parent = node_y;
}

static void right_rotate(rbtree *t, node_t *node_x) {
  node_t *node_y = node_x->left;
  node_x->left = node_y->right;

  if(node_y->right != t->nil) {
    node_y->right->parent = node_x;
  }

  node_y->parent = node_x->parent;

  if(node_x->parent == t->nil) {
    t->root = node_y;
  } else if(node_x == node_x->parent->left) {
    node_x->parent->left = node_y;
  } else {
    node_x->parent->right = node_y;
  }

  node_y->right = node_x;
  node_x->parent = node_y;
}

int main(int argc, char *argv[]) {
    rbtree *t = new_rbtree();

    rbtree_insert(t, 10);
    rbtree_insert(t, 15);
    rbtree_insert(t, 5);
    rbtree_insert(t, 25);
    rbtree_insert(t, 18);

    // rbtree_erase(t, rbtree_find(t, 10));
    // print_rbtree(t, t->root);

    // node_t *temp = rbtree_max(t);
    // if(temp != t->nil) {
    //     printf("%d\n", temp->key);
    // }

    // node_t *temp = rbtree_min(t);
    // if(temp != t->nil) {
    //     printf("%d\n", temp->key);
    // }

    // node_t *temp = rbtree_find(t, 10);
    // if(temp != NULL) {
    //     printf("%d\n", temp->key);
    //     if(temp->left != t->nil) {
    //         printf("%d\n", temp->left->key);
    //     }
    //     if(temp->right != t->nil) {
    //         printf("%d\n", temp->right->key);
    //     }
    // }

    // print_rbtree(t, t->root);

    // printf("%d\n", t->root->key);
    // left_rotate(t, t->root);
    // printf("%d\n", t->root->key);
    // right_rotate(t, t->root);
    // printf("%d\n", t->root->key);

    delete_rbtree(t);
    printf("Deleted!\n");
}