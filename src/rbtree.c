#include "rbtree.h"
#include <stdlib.h>

void rbtree_transplant(rbtree *t, node_t *first, node_t *second);
node_t *rbtree_successor(const rbtree *t, node_t *right);
int rbtree_erase(rbtree *t, node_t *p);

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
void left_rotate(rbtree *t,node_t *x)
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
void right_rotate(rbtree *t,node_t *x)
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

void rbtree_insert_fixup(rbtree *t,node_t *z)
{
  while(z != t->root && z->parent->color == RBTREE_RED)       // 삽입 노드가 루트가 아니면서 삽입 노드의 부모가 레드이면
  {
    if(z->parent->parent->left == z->parent)                  // 삽입 노드의 할배의 왼쪽이 부모면
    {
      node_t *y = z->parent->parent->right;                   // 삼촌이 할배의 오른쪽
      if(y->color == RBTREE_RED)                              // 만약 삼촌이 레드이면 (case 1)
      {
        z->parent->color = RBTREE_BLACK;                      // 부모를 검정으로 바꾸고
        y->color = RBTREE_BLACK;                              // 삼촌도 검정으로 바꾸고
        z->parent->parent->color = RBTREE_RED;                // 할배를 레드로 바꾸고
        z = z->parent->parent;                                // 할배 노드를 현재 노드로 지정
      }
      else                                                    // 삼촌이 검정이면 (case 2)
      {
        if(z == z->parent->right)                             // 삽입 노드가 부모의 오른쪽이면 (부모key 보다 값이 크면)
        {
          z = z->parent;                                      // 현재 노드를 부모 노드로 지정하고
          left_rotate(t,z);                                    // 왼쪽으로 회전
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
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
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;

}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  node_t *addnode = (node_t*)malloc(sizeof(node_t));            // 새로 추가하는 노드 메모리 할당
  addnode->key = key;                                           // key 초기화
  addnode->color = RBTREE_RED;                                  // 처음 삽입하는 노드는 빨강
  addnode->left = t->nil;                                       // 왼쪽 오른쪽 자식 nil로 초기화
  addnode->right = t->nil;

  node_t *parent = t->nil;                                      // 부모 nil로 초기화 (현재 노드 루트로 지정하기 때문에)
  node_t *p = t->root;                                          // 현재 노드 루트로 지정

  while(p != t->nil){                                           // 현재 노드가 nil이 아닐 동안
    parent = p;                                                 // 부모를 현재 노드로 지정
    if(p->key > key)                                            // 현재 노드의 값이 들어온 값보다 큰 경우
      p = p->left;                                              // 현재 노드의 왼쪽으로 이동
    else                                                        // 현재 노드의 값보다 들어온 값이 더 큰 경우
      p = p->right;                                             // 현재 노드의 오른쪽으로 이동
  }

  addnode->parent = parent;                                     // 자식이 없는 노드를 만나서 while문이 종료되면 그 노드를 부모 노드로 지정

  if(parent == t->nil)                                          // 부모가 nil이면 (빈 트리)
    t->root = addnode;                                          // 새로운 노드가 트리의 루트
  else if(key < parent->key)                                    // 새로운 노드의 값이 부모의 값보다 작으면
    parent->left = addnode;                                     // 부모의 왼쪽에 삽입
  else                                                          // 부모의 값보다 크면
    parent->right = addnode;                                    // 부모의 오른쪽에 삽입
  rbtree_insert_fixup(t, addnode);                                    // 새로운 노드 삽입 후 불균형 교정 작업
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;

  while(x != t->nil){                     // 루트가 nil이 아닐 동안
    if(key == x->key)                     // 찾는 key가 현재 노드의 key와 같을 경우
      return x;                           // 찾았다!
    else if (x->key > key)                // 현재 노드의 key가 찾는 key보다 클 경우
      x = x->left;                        // 현재 노드의 왼쪽으로 이동
    else
      x = x->right;                       // 아니면 현재 노드의 오른쪽으로 이동
  }
  return NULL;                            // while을 돌았는데 못 찾았으면 null 리턴
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

void rbtree_transplant(rbtree *t, node_t *first, node_t *second){
  if(first->parent == t->nil)
    t->root = second;
  else if(first == first->parent->left)
    first->parent->left = second;
  else
    first->parent->right = second;
  second->parent = first->parent;
}

node_t *rbtree_successor(const rbtree *t, node_t *right){
  node_t *x = right;
  while(x->left != t->nil){
      x = x->left;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *successor = p;
  color_t successor_color = successor->color;
  node_t *x;
  if(p->left == t->nil){
    node_t *x = p->right;
    rbtree_transplant(t, p, p->right);
    free(p);
  }
  else if(p->right == t->nil){
    node_t *x = p->left;
    rbtree_transplant(t, p, p->left);
    free(p);
  }
  else{
    successor = rbtree_successor(t, p->right);
    color_t successor_color = successor->color;
    node_t *x = successor->right;
    if(successor->parent == p)
      x->parent = successor;
    else{
      rbtree_transplant(t, successor, successor->right);
      successor->right = p->right;
      successor->right->parent = successor;
    }
    rbtree_transplant(t, p, successor);
    successor->left = p->left;
    successor->left->parent = successor;
    successor->color = p->color;
    free(p);
  }
  if(successor_color == RBTREE_BLACK)
    rbtree_delete_fixup(t, x);
  return 0;
}

void rbtree_delete_fixup(const rbtree *t, node_t *x){

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
