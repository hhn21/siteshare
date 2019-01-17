#ifndef __SLLIST_H__
#define __SLLIST_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*----------------------Single linked List node struct---------------------------------*/

struct ListNode {
    void* data;
    struct ListNode *next;
};

typedef struct ListNode ListNode;

typedef struct List
{
    ListNode *root;
    ListNode *cur;
} List;

/*----------------------Single linked List methods---------------------------------*/

List* newList();                                                // create new List
void insertAtHead(List*, void*);                                // Insert new node at head of list. Params: 1. List pointer, 2. Data pointer
void insertAtTail(List*, void*);                                // Insert new node at tail of list. Params: 1. List pointer, 2. Data pointer
void insertAtPosition(List*, void*, int);                       // Insert new node at specific position. Params: 1. List pointer, 2. Data pointer, 3. Position
void deleteHead(List*);                                         // Delete list head
void deleteNode(List*, ListNode*);                              // Delete a node
void deleteAtPosition(List*, int);                              // Delete a node at specific position
void destroyList(List* List);                                   // Free list and its data

#define listTraverse(listNode, list) \
  for (listNode = list->root; listNode != NULL; listNode = listNode->next)

#endif