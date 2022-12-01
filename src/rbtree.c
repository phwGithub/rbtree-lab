#include "rbtree.h"

#include <stdlib.h>

// rb_tree 구조체 p를 할당하여 초기화 후 리턴
// parameters : void
// return : rbtree p
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  if (p != NULL) {
    node_t *nil = (node_t *)calloc(1, sizeof(node_t));
    if(nil != NULL) {
      nil->color = RBTREE_BLACK;
      p->nil = nil;
      p->root = p->nil;
    }
  }

  return p;
}

// rbtree t에 대해서 후위 순회하면서 노드를 할당 해제
// parameters : rbtree t, node_t root
// return : void
static void free_traverse(rbtree* t, node_t* root) {
    if (root->left != t->nil) {
      free_traverse(t, root->left);
    }
    if (root->right != t->nil) {
      free_traverse(t, root->right);
    }
    free(root);
}

// rbtree t에 대해 모든 노드를 할당 해제한 후
// nil 노드와 rbtree t 할당 해제
// parameters : rbtree t
// return : void
void delete_rbtree(rbtree *t) {
  if(t->root != t->nil) {
    free_traverse(t, t->root);
  }
  free(t->nil);
  free(t);
}

// rbtree t에 대해 node_x를 기준으로 좌회전
// parameters : rbtree t, node_t node_x
// return : void
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

// rbtree t에 대해 node_x를 기준으로 우회전
// parameters : rbtree t, node_t node_x
// return : void
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

// rbtree t에 대해 z 노드를 삽입한 후
// t가 rbtree에 해당하는지 확인
// parameter : rbtree t, node_t z
// return : void
static void rb_insert_fixup(rbtree *t, node_t *z) {
  while(z->parent->color == RBTREE_RED) {
    node_t *y = NULL;

    if(z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;

      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if(z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z); 
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } else {
      y = z->parent->parent->left;

      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if(z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z); 
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

// rbtree t에 대해 입력받은 key_t key값을 가지는 노드를 삽입
// parameters : rbtree t, key_t key
// return : node_t new_node
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = t->nil;

  node_t* node_y = t->nil;
  node_t* node_x = t->root;

  while(node_x != t->nil) {
    node_y = node_x;

    if(key < node_x->key) {
      node_x = node_x->left;
    } else {
      node_x = node_x->right;
    }
  }

  new_node->parent = node_y;

  if(node_y == t->nil) {
    t->root = new_node;
  } else {
    if(key < node_y->key) {
      node_y->left = new_node;
    } else {
      node_y->right = new_node;
    }
  }

  rb_insert_fixup(t, new_node);
   
  return new_node;
}

// rbtree t에 대해 key_t key 값을 가지는 노드를 검색한 후
// 있다면 찾은 노드를 리턴, 없다면 NULL을 리턴
// parameters : rbtree t, key_t key
// return : node_t x or NULL
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;

  while(x != t->nil) {
    if(x->key == key) {
      return x;
    }
    if(x->key > key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  return NULL;
}

// rbtree t에 대해 가장 작은 값의 key를 가지는 노드를 반환
// parameters : rbtree t
// return : node_t y
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  node_t *y = t->nil;

  while(x != t->nil) {
    y = x;
    x = x->left;
  }

  return y;
}

// rbtree t에 대해 root를 node_t z 노드로 가지는 서브트리에서
// 가장 작은 값의 key를 가지는 노드를 반환
// parameters : rbtree t, node_t z
// return : node_t y
static node_t *node_min(const rbtree *t, node_t *z) {
  node_t *x = z;
  node_t *y = t->nil;

  while(x != t->nil) {
    y = x;
    x = x->left;
  }

  return y;
}

// rbtree t에 대해 가장 최대값의 key를 가지는 노드를 반환
// parameters : rbtree t
// return : node_t y
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  node_t *y = t->nil;

  while(x != t->nil) {
    y = x;
    x = x->right;
  }

  return y;
}

// rbtree t에 대해 node_t u의 자리에 node_t v를 설정
// u의 부모를 v의 부모로, u의 부모의 자식을 v로
// parameters : rbtree t, node_t u, node_t v
// return : void
static void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if(u->parent == t->nil) {
    t->root = v;
  } else if(u == u->parent->left) {
    u->parent->left =v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

// rbtree t에 대해 node_x가 있던 자리의 노드가 삭제됐을 때
// t가 rbtree로 성립하는지 확인
// parameters : rbtree t, node_t
// return : void
static void rb_delete_fixup(rbtree *t, node_t *x) {
  while(x != t->root && x->color == RBTREE_BLACK) {
    node_t *w = t->nil;

    if(x == x->parent->left) {
      w = x->parent->right;
      
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if(w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    } else {
      w = x->parent->left;
      
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if(w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

// rbtree t에 대해 node_t p가 있다면 삭제
// parameters : rbtree t, node_t p
// return : 성공 시 1, 실패 시 0
int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *x;
  int y_origin_color = y->color;

  if(p == NULL || p == t->nil) {
    return 0;
  }

  if(p->left == t->nil) {
    x = p->right;
    rb_transplant(t, p, p->right);
  } else if(p->right == t->nil) {
    x = p->left;
    rb_transplant(t, p, p->left);
  } else {
    y = node_min(t, p->right);
    y_origin_color = y->color;
    x = y->right;

    if(y->parent == p) {
      x->parent = y;
    } else {
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rb_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if(y_origin_color == RBTREE_BLACK) {
    rb_delete_fixup(t, x);
  }

  free(p);

  return 1;
}

// rbtree t애 대해 중위 순회하면서 이 순서대로 key_t* arr에 입력
// parameters : rbtree t, node_t root, key_t *arr, int *idx
// return : void
static void inorder_traversal(const rbtree *t, node_t *root, key_t *arr, int *idx) {
  if(root->left != t->nil) {
    inorder_traversal(t, root->left, arr, idx);
  }
  arr[*idx] = root->key; *idx += 1;
  if(root->right != t->nil) {
    inorder_traversal(t, root->right, arr, idx);
  }
}

// rbtree t를 중위 순회하여 size_t n의 사이즈를 가지는 key_t *arr로 반환
// parameters : rbtree t, key_t *arr, size_t n
// return : 성공 시 1, 실패 시 0
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int *idx;
  int idx_val = 0;
  idx = &idx_val;

  inorder_traversal(t, t->root, arr, idx);

  int result = 0;
  if((sizeof(arr) / sizeof(key_t)) == n) {
    result = 1;
  }

  return result;
}
