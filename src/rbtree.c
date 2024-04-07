#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  // tree 구조체 동적 할당
  rbtree *t = (rbtree *)malloc(sizeof(rbtree));

  // nil 노드 생성 및 초기화
  node_t *nil = (node_t *)malloc(sizeof(node_t));
  nil->color = RBTREE_BLACK; // nil 노드는 항상 BLACK

  // tree의 nil과 root를 nil 노드로 설정 (tree가 빈 경우 root는 nil노드여야 한다.)
  t->nil = t->root = nil;

  return t;
}

//x노드 기준으로 왼쪽 회전
void leftRotate(rbtree *t,node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;

  if(y->left != t->nil)
    y->left->parent = x;
  
  y->parent = x->parent;

  if(x->parent == t->nil)
    t->root = y;
  else if(x==x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}
//x 노드 기준으로 오른쪽 회전
void rightRotate(rbtree *t,node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;

  if(y->right != t->nil)
    y->right->parent = x;
  
  y->parent = x->parent;

  if(x->parent == t->nil)
    t->root = y;
  else if(x==x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rbInsertFixup(rbtree *t,node_t *z)
{
  while(z != t->root && z->parent->color == RBTREE_RED)
  {
    if(z->parent->parent->left == z->parent)
    {
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->right)
        {
          z = z->parent;
          leftRotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rightRotate(t,z->parent->parent);
      }
    }
    else
    {
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->left)
        {
          z = z->parent;
          rightRotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        leftRotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;

}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  node_t *addnode = (node_t*)malloc(sizeof(node_t));
  addnode->key = key;
  addnode->color = RBTREE_RED;
  addnode->left = t->nil;
  addnode->right = t->nil;

  node_t *parent = t->nil;
  node_t *p = t->root;

  while(p != t->nil){
    parent = p;
    if(p->key > key)
      p = p->left;
    else
      p = p->right;
  }

  addnode->parent = parent;

  if(parent == t->nil)
    t->root = addnode;
  else if(key < parent->key)
    parent->left = addnode;
  else
    parent->right = addnode;
  rbInsertFixup(t, addnode);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
