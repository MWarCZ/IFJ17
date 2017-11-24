//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/ast.c                    //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef AST_C
#define AST_C

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "token.h"

TATSNode* InitASTNode(TATSNodeType type) {
  TATSNode *node = NULL;
  if( (node = malloc(sizeof(TATSNode)) ) == NULL ) {
    return NULL;
  }
  node->type = type;
  node->token1 = NULL;
  node->token2 = NULL;
  node->listPostFix = NULL;
  node->node1 = NULL;
  node->node2 = NULL;
  node->node3 = NULL;
  node->node4 = NULL;

  return node;
}

void DestroyASTNode(TATSNode *node) {
  if( node == NULL ) {
    return;
  }
  DestroyASTNode( node->node1 );
  DestroyASTNode( node->node2 );
  DestroyASTNode( node->node3 );
  DestroyASTNode( node->node4 );
  free(node);
}

void DestroyASTNodeSafely(TATSNode **node) {
  if( node == NULL ) {
    return;
  }
  if( (*node) == NULL ) {
    return;
  }
  if( (*node)->token1 != NULL ) {
    TokenDestroy( (*node)->token1 );
    (*node)->token1 = NULL;
  }
  if( (*node)->token2 != NULL ) {
    TokenDestroy( (*node)->token2 );
    (*node)->token2 = NULL;
  }
  if( (*node)->listPostFix != NULL ) {
    TListData data;
    while( ListPop( (*node)->listPostFix , &data) ) {
      TokenDestroy( ((TToken*)data.pointer) );
    }
    ListDestroy( (*node)->listPostFix );
    (*node)->listPostFix = NULL;
  }
  DestroyASTNodeSafely( &((*node)->node1) );
  DestroyASTNodeSafely( &((*node)->node2) );
  DestroyASTNodeSafely( &((*node)->node3) );
  DestroyASTNodeSafely( &((*node)->node4) );
  free( (*node) );
  (*node) = NULL;
}

#endif

// Soubor: src/ast.c
