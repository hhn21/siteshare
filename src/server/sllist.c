#include "sllist.h"

List* newList(){
    List *list = malloc(sizeof(List));
    list->root = NULL;
    return list;
}

ListNode* makeNewNode(void* data){
    ListNode* new = (ListNode*) malloc(sizeof(ListNode));
    new->data = data;
    new->next = NULL;
    return new;
}

void insertAtHead(List *list, void* e){
    ListNode* new = makeNewNode(e);
    new->next = list->root;
    list->root = new;
}

void insertAtTail(List *list, void* e){
    ListNode* new = makeNewNode(e);
    if(list->root == NULL) {
        list->root = new;
    } else {
        ListNode *tmp = list->root;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new;
    }
}

void insertAtPosition(List *list, void* e, int p){
    if(p < 0) {
        printf("Vi tri khong hop le!\n");
    }
    else if (p == 0) {
        ListNode* new = makeNewNode(e);
        new->next = list->root;
        list->root = new;
        printf("Them thanh cong!\n");
    }
    else {
        int i;
        ListNode* new = makeNewNode(e);
        ListNode *tmp = list->root;
        ListNode *prev = NULL;
        for(i = 0; i < p; i++) {
            if(tmp == NULL) break;
            prev = tmp;
            tmp = tmp->next;
        }
        if(i == p){
            prev->next = new;
            new->next = tmp;
            printf("Them thanh cong\n"); 
        }
        else printf("Het danh sach! Them khong thanh cong\n");
    }
}

void deleteHead(List *list){
    if(list->root != NULL) {
        ListNode *tmp = list->root->next;
        free(list->root);
        list->root = tmp;
    }
}

void deleteNode(List *list, ListNode *node){
    ListNode *tmp = list->root;
    ListNode *prev = NULL;

    while(tmp != NULL) {
        if(tmp == node) {
            if(prev != NULL) prev->next = tmp->next;
            free(tmp);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void deleteAtPosition(List *list, int p){
    ListNode *tmp = list->root;
    ListNode *prev = NULL;
    if(p < 0) {
        printf("Vi tri khong hop le!\n");
    }
    else if (p == 0) {
        deleteHead(list);
        printf("Xoa thanh cong!\n");
    }
    else {
        int i;
        for(i = 0; i < p; i++) {
            if(tmp->next == NULL) break;
            prev = tmp;
            tmp = tmp->next;
        }
        if(i == p){
            prev->next = tmp->next;
            free(tmp);
            printf("Xoa thanh cong\n"); 
        }
        else printf("Het danh sach! Xoa khong thanh cong\n");   
    }
}

void destroyList(List *list){
    while(list->root != NULL) deleteHead(list);
}