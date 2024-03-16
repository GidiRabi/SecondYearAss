#include "StrList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node & List Data Structures
typedef struct _node {
  char *_data;
  struct _node *_next;
} Node;

struct _StrList {
  Node *_head;
  size_t _size;
};
//------------------------------------------------
// Node implementation
//------------------------------------------------

Node *Node_alloc(const char *data, Node *next) {
  // allocate memory for the node
  Node *p = (Node *)malloc(sizeof(Node));
  if (p == NULL) {  // if malloc fails
    return NULL;
  }
  p->_data = strdup(data);  // strdup allocates memory for the string and copies
                            // the string to the allocated memory
  if (p->_data == NULL) {   // if strdup fails
    free(p);
    return NULL;
  }

  // point to the next node that is passed as an argument
  p->_next = next;
  return p;
}

void Node_free(Node *node) {
  if (node != NULL) {
    free(node->_data);
    free(node);
  }
}

//------------------------------------------------

//------------------------------------------------
// List implementation
//------------------------------------------------

StrList *StrList_alloc() {
  // allocate memory for the list
  StrList *p = (StrList *)malloc(sizeof(StrList));
  p->_head = NULL;
  p->_size = 0;
  return p;
}

void StrList_free(StrList *list) {
  if (list == NULL) return;
  Node *p1 = list->_head;  // point to the first node
  Node *p2;                // helper pointer to free the nodes

  // free all the nodes in the list
  while (p1) {
    p2 = p1;         // point to the current node
    p1 = p1->_next;  // point to the next node
    Node_free(p2);   // free the current node (p2 points to the current node)
  }

  // after freeing all the nodes, free the list
  free(list);
}

size_t StrList_size(const StrList *list) { return list->_size; }

void StrList_insertLast(StrList *list, const char *data) {
  // allocate memory for the new node
  Node *newNode = Node_alloc(data, NULL);
  // if malloc fails, return
  if (newNode == NULL) {
    return;
  }

  // if the list is empty, point the head to the new node
  if (list->_head == NULL) {
    list->_head = newNode;
  } else {
    Node *temp = list->_head;

    while (temp->_next != NULL) {
      temp = temp->_next;
    }
    temp->_next = newNode;
  }
  list->_size++;  // increment the size of the list
}

void StrList_insertAt(StrList *list, const char *data, int index) {
  // check if the index is out of bounds
  if (index < 0 || index > list->_size) {
    return;  // TODO: check what to do
    // exit(OUT_OF_BOUNDS);
  }

  // if the index is the size of the list, the new node is inserted at the
  // end
  if (index == list->_size) {
    StrList_insertLast(list, data);
    return;
  }
  // allocate memory for the new node - for now the next pointer is NULL
  Node *newNode = Node_alloc(data, NULL);

  // check if malloc fails
  if (newNode == NULL) {
    return;  // TODO: check what to do
    // exit(ALLOCATION_FAILED);
  }

  if (index == 0) {  // if the index is 0, the new node becomes the head
    newNode->_next = list->_head;
    list->_head = newNode;
  } else {  // if the index is not 0, we need to find the node before the
            // index``
    Node *temp = list->_head;
    for (size_t i = 0; i < index - 1; i++) {  // find the node before the index
      temp = temp->_next;
    }

    // point the new node to the node after the index
    newNode->_next = temp->_next;
    // point the node before the index to the new node
    temp->_next = newNode;
  }

  list->_size++;  // increment the size of the list
}

char *StrList_firstData(const StrList *list) {
  if (list->_head == NULL) {  // if the list is empty return NULL
    return NULL;
  }

  // return the data of the first node
  return list->_head->_data;
}

void StrList_print(const StrList *list) {
  // if the list is empty, print ] and return
  if (list->_head == NULL) {
    printf("\n");
    return;
  }

  // loop through the list and print the data of each node
  Node *temp = list->_head;
  while (temp->_next != NULL) {  // loop until the second to last node
    printf("%s ", temp->_data);
    temp = temp->_next;
  }
  // print the data of the last node and ]
  printf("%s\n", temp->_data);
}

void StrList_printAt(const StrList *list, int index) {
  // check if the index is out of bounds
  if (index < 0 || index >= list->_size) {
    return;  // TODO: check what to do
    // exit(OUT_OF_BOUNDS);
  }

  // loop through the list until the index
  Node *temp = list->_head;
  for (size_t i = 0; i < index; i++) {
    temp = temp->_next;
  }
  // print the data of the node at the index
  printf("%s\n", temp->_data);
}

int StrList_printLen(const StrList *list) {
  int sum = 0;
  Node *temp = list->_head;
  while (temp != NULL) {
    sum += strlen(temp->_data);
    temp = temp->_next;
  }
  return sum;
}

int StrList_count(StrList *list, const char *data) {
  int count = 0;
  Node *temp = list->_head;
  while (temp != NULL) {
    if (strcmp(data, temp->_data) == 0) {
      count++;
    }
    temp = temp->_next;
  }
  return count;
}

void StrList_remove(StrList *list, const char *data) {
  // remove all the firsts elements that equals to data
  Node *temp = list->_head;
  while (temp != NULL && strcmp(temp->_data, data) == 0) {
    Node *toDelete = temp;  // save the node to delete
    temp = temp->_next;     // go to the next node
    Node_free(toDelete);    // free the node that was saved
    list->_size--;          // decrement the size of the list
  }
  list->_head = temp;  // update the head

  // remove all the elements that equals to data after the firsts elements that
  // equals to data

  Node *prev = temp;  // save the previous node so we can point to the next node
  // temp can be NULL if the list is empty after removing the firsts elements
  // that equals to data
  if (temp != NULL) {
    temp = temp->_next;
  }

  // loop through the list and remove the elements that equals to data
  while (temp != NULL) {
    // if the data of the current node equals to data
    if (strcmp(temp->_data, data) == 0) {
      prev->_next =
          temp->_next;  // point the previous node to the current next node
      Node_free(temp);  // free the current node

      temp = prev->_next;  // point to the next node

      list->_size--;  // decrement the size of the list
    } else {
      // if the data of the current node is different from data
      // we need the else because we update the temp pointer in the if
      // statement, and maybe he equal to data, so we need to check again
      prev = temp;
      temp = temp->_next;
    }
  }
}

void StrList_removeAt(StrList *list, int index) {
  // check if the index is out of bounds
  if (list == NULL || list->_head == NULL || index < 0 ||
      index >= list->_size) {
    return;  // TODO: check what to do
    // exit(OUT_OF_BOUNDS);
  }

  // if the index is 0, remove the head and point the head to the next node
  if (index == 0) {
    Node *toDelete = list->_head;      // save the node to delete
    list->_head = list->_head->_next;  // point the head to the next node
    Node_free(toDelete);               // free the node that was saved
    list->_size--;                     // decrement the size of the list
    return;                            // exit the function
  }

  // loop through the list until the we get to the parent of StrList[index]
  Node *temp = list->_head;
  for (size_t i = 0; i < index - 1; i++) {
    temp = temp->_next;
  }

  Node *toDelete = temp->_next;      // save the node to delete
  temp->_next = temp->_next->_next;  // point the previous node to the next node
  Node_free(toDelete);               // free the node that was saved
  list->_size--;                     // decrement the size of the list
}

int StrList_isEqual(const StrList *list1, const StrList *list2) {
  if (list1 == NULL || list2 == NULL) return 0;
  // if the sizes are different, the lists are different
  if (list1->_size != list2->_size) {
    return 0;
  }

  // loop through the lists and compare the data of each node
  Node *temp1 = list1->_head;
  Node *temp2 = list2->_head;

  while (temp1 != NULL && temp2 != NULL) {
    // if the data of the current node is different, the lists are different
    if (strcmp(temp1->_data, temp2->_data) != 0) return 0;

    temp1 = temp1->_next;
    temp2 = temp2->_next;
  }

  // if we get to the end of the lists, the lists are equal
  return 1;
}

StrList *StrList_clone(const StrList *list) {
  StrList *clone = StrList_alloc();  // allocate memory for the clone

  // set the size of the clone to the size of the list
  clone->_size = list->_size;

  // temp pointer to loop through the list
  const Node *old = list->_head;

  // copy pointer to loop through the clone
  Node **copy = &(clone->_head);

  // loop through the list and copy the data of each node to the clone
  while (old != NULL) {
    // allocate memory for the new node and copy the data of the current node
    *copy = Node_alloc(old->_data, NULL);
    // if malloc fails, free the memory allocated to the nodes and the list
    if (*copy == NULL) {
      StrList_free(clone);
      return NULL;
    }
    // continue to the next node
    old = old->_next;

    // point to the next node of the clone
    copy = &((*copy)->_next);
  }
  return clone;
}

void StrList_reverse(StrList *list) {
  if (list == NULL || list->_head == NULL) return;
  Node *current = list->_head;
  Node *next = NULL;
  Node *prev = NULL;
  while (current != NULL) {
    next = current->_next;
    current->_next = prev;
    prev = current;

    current = next;
  }

  list->_head = prev;
}

int StrList_isSorted(StrList *list) {
  if (list == NULL || list->_head == NULL) return 0;
  Node *temp = list->_head;

  if (temp == NULL) return 1;
  while (temp->_next != NULL) {
    if (strcmp(temp->_data, temp->_next->_data) > 0) {
      return 0;
    }
    temp = temp->_next;
  }
  return 1;
}

void swapNodes(Node *a, Node *b) {
  char *temp = a->_data;
  a->_data = b->_data;
  b->_data = temp;
}

void bubbleSort(StrList *list) {
  int swapped;
  Node *ptr1;
  Node *lptr = NULL;

  if (list->_head == NULL) return;

  do {
    swapped = 0;
    ptr1 = list->_head;

    while (ptr1->_next != lptr) {
      if (strcmp(ptr1->_data, ptr1->_next->_data) > 0) {
        swapNodes(ptr1, ptr1->_next);
        swapped = 1;
      }
      ptr1 = ptr1->_next;
    }
    lptr = ptr1;
  } while (swapped);
}
void StrList_sort(StrList *list) {
  if (list == NULL || list->_head == NULL) return;
  bubbleSort(list);
}