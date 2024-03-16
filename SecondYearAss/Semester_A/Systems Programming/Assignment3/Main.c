#include <stdio.h>

#include "StrList.h"
/**
 * This function reads a word from the standard input and returns it.
 * The word is read from the standard input and stored in a dynamically
 * allocated memory. The function should return a pointer to the first character
 * of the word.
 * we know that the word is ended by a space or a newline character.
 */
char *get_word() {
  int capacity = 10;
  char *word = (char *)malloc(capacity * sizeof(char));
  int i = 0;
  char c;
  while (1) {
    c = getchar();
    if (c == ' ' || c == '\n') {
      if (i == 0) {
        continue;
      }
      word[i] = '\0';
      break;
    }
    word[i] = c;
    i++;
    if (i >= capacity) {
      capacity *= 2;
      word = realloc(word, capacity * sizeof(char));
    }
  }
  return word;
}

int main(void) {
  int input;
  int index;
  int num_elements;
  char *data;
  StrList *list = StrList_alloc();
  do {
    if (scanf("%d", &input) != 1) {
      printf("Invalid input\n");
      break;
    }
    switch (input) {
      case 1:
        if (scanf("%d", &num_elements) != 1) {
          printf("Invalid input\n");
          break;
        }
        for (int i = 0; i < num_elements; i++) {
          data = get_word();
          if (data == NULL) {
            printf("Invalid input\n");
            break;
          }
          StrList_insertLast(list, data);
          free(data);
        }
        break;
      case 2:
        if (scanf("%d", &index) != 1) {
          printf("Invalid input\n");
          break;
        }
        data = get_word();
        if (data == NULL) {
          printf("Invalid input\n");
          break;
        }
        StrList_insertAt(list, data, index);
        free(data);
        break;
      case 3:
        StrList_print(list);
        break;
      case 4:
        printf("%ld\n", StrList_size(list));
        break;
      case 5:
        if (scanf("%d", &index) != 1) {
          printf("Invalid input\n");
          break;
        }
        StrList_printAt(list, index);
        break;
      case 6:
        printf("%d\n", StrList_printLen(list));
        break;
      case 7:
        data = get_word();
        if (data == NULL) {
          printf("Invalid input\n");
          break;
        }
        printf("%d\n", StrList_count(list, data));
        free(data);
        break;
      case 8:
        data = get_word();
        if (data == NULL) {
          printf("Invalid input\n");
          break;
        }
        StrList_remove(list, data);
        free(data);
        break;
      case 9:
        if (scanf("%d", &index) != 1) {
          printf("Invalid input\n");
          break;
        }
        StrList_removeAt(list, index);
        break;
      case 10:
        StrList_reverse(list);
        break;
      case 11:
        StrList_free(list);
        list = StrList_alloc();
        break;
      case 12:
        StrList_sort(list);
        break;
      case 13:
        printf(StrList_isSorted(list) ? "true\n" : "false\n");
    }

  } while (input != 0);

  StrList_free(list);
  return 0;
}