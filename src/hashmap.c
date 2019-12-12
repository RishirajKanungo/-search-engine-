#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAXCHAR 2048

struct hashmap* hm_create(int num_buckets){
  int i;
  //ask for memory based on size of buckets
  struct hashmap* hm = (struct hashmap*)malloc(sizeof(struct hashmap)); //declare a hashmap object

  //check if hm is not given memory
  if(hm == NULL){
    printf("Hashmap not allocated properly");
  }

  //set the values to our hashmap from our arguments
  hm->num_buckets = num_buckets;
  hm->num_elements = 0;

  //allocate memory to the map which is a double pointer
  hm->map = (struct llnode**) malloc (sizeof(struct llnode*) * num_buckets); //1-d array

  //check if map is not allocated memory
  if(hm->map == NULL){
    printf("Map not allocated properly");
    return NULL;
  }

  //iterate through the 1d array and and allocate each one to be null
  for (i = 0; i < num_buckets; i++)
  {
    hm->map[i] = NULL;
  }

  //return the hashmap ptr
  return hm;

}
void hm_put(struct hashmap* hm, char* word, char* document_id){
    //edge cases for passing in invalid values into hm_put
  if(hm == NULL){
      printf("Hashmap is not allocated.\n");
      return;
  }
  if(word == NULL){
      printf("Word is not allocated.\n");
      return;
  }
  if(document_id == NULL){
      printf("Document_ID is not allocated.\n");
      return;
  }

  //this gets the index to which bucket to go to
  int index = hash(hm, word);

  //check if the first occurrence of the bucket is null
  if(hm->map[index] == NULL){
      //if it is, create a pointer with values and assign to the head
      struct llnode* ptr = (struct llnode*)malloc(sizeof(struct llnode));
      //malloc the proper amount of space and +1 for the null terminator
      ptr->word = (char*)malloc(sizeof(char)*strlen(word) + 1);
      //perform deep copy of word into the destination = ptr->word
      strcpy(ptr->word,word);
      ptr->document_id = (char*)malloc(sizeof(char)*strlen(document_id) + 1);
      //perform deep copy and set values of the new node and hashmap then return
      strcpy(ptr->document_id, document_id);
      ptr->num_occurrences = 1;
      ptr->next = NULL;
      hm->map[index] = ptr;
      hm->num_elements++;
      return;
  }

  //the case where it is the first node in the linked list. check those values
  else if(strcmp(hm->map[index]->word, word) == 0 && strcmp(hm->map[index]->document_id, document_id) == 0){
      hm->map[index]->num_occurrences++;
      return;
  }

  //if it not the first node, iterate through the linked list and check as you go
  //current node pointer, starts off at the head of the list
  struct llnode* current = hm->map[index];

  //iterate through the linked list by looking one ahead and checking the current node
  while(current != NULL){
      if(strcmp(current->word, word) == 0 && strcmp(current->document_id, document_id) == 0){
          current->num_occurrences++;
          return;
      }
      if(current->next == NULL){
        break;
      }
      current = current->next;
  }

  //check the very last node if it is the word and docuement
  if(strcmp(current->word, word) == 0 && strcmp(current->document_id, document_id) == 0){
      current->num_occurrences++;
      return;
  }

  //if it has not been able to update the list, add a new node to the end
  struct llnode* new_node = (struct llnode*)malloc(sizeof(struct llnode));
  new_node->word = (char*)malloc(sizeof(char)*strlen(word) + 1);
  strcpy(new_node->word, word);
  new_node->document_id = (char*)malloc(sizeof(char)*strlen(document_id) + 1);
  strcpy(new_node->document_id, document_id);
  new_node->num_occurrences = 1;
  current->next = new_node;
  new_node->next = NULL;
  hm->num_elements++;

  return;
}
void hm_remove(struct hashmap* hm, char* word, char* document_id){
    //edge cases for passing in invalid values into hm_put
  if(hm == NULL){
      printf("Hashmap is not allocated.\n");
      return;
  }
  if(word == NULL){
      printf("Word is not allocated.\n");
      return;
  }
  if(document_id == NULL){
    printf("Document is not allocated.\n");
    return;
  }


  //get the index of which bucket to enter
  int index = hash(hm, word);
  printf("%d\n", index);
  //create current ptr to act as the head at first
  //create a previous ptr to point to the node before current
  struct llnode* current = hm->map[index];
  struct llnode* previous = hm->map[index];

  //check if the first value of that index is null
  if(current == NULL){
    //if it is, then you cannot remove anything from there
    printf("Trying to remove from empty bucket.\n");
    return;
  }

  //check if the instance is the first one of the list
  if(strcmp(current->word, word) == 0){
    previous = current->next;
    free(current->word);
    free(current->document_id);
    free(current);
    hm->map[index] = previous;
    return;
  }

  //iterate through the linked list by looking one ahead and checking the current node
  while(current->next != NULL){
      //if it is the same word
      if(strcmp(current->word, word) == 0 && strcmp(current->document_id, document_id) == 0){
          //hm->num_elements = hm->num_elements - current->num_occurrences;
          //set previous' next to be current's next
          previous->next = current->next;
          //free the current node and return
          free(current->word);
          free(current->document_id);
          free(current);
          return;
      }
      //if it is not the node we want, set previous to be current
      //and increment current node
      previous = current;
      current = current->next;
    }
    //check the very last node if it is the word and docuement
  if(strcmp(current->word, word) == 0 && strcmp(current->document_id, document_id) == 0){
    hm->num_elements = hm->num_elements - current->num_occurrences;
    previous->next = current->next;
    free(current->word);
    free(current->document_id);
    free(current);
    return;
  }
  return;
}
void hm_destroy(struct hashmap* hm){
   //initialize var
  int index;

  //iterate through the hashmap table
  for(index = 0; index < hm->num_buckets; index++){
    //is the head NULL
    if(hm->map[index] == NULL){
      //if it is just continue the for loop
      continue;
    }
    else{
      //create a current ptr
      struct llnode* prev;
      //while the map ptr next is not null
      while(hm->map[index]->next != NULL){
        //set the current to to be the index
        prev = hm->map[index];
        //set the head ptr to be the next one
        hm->map[index] = hm->map[index]->next;
        //free the prev
        free(prev->word);
        free(prev->document_id);
        free(prev);
      }
      free(hm->map[index]->word);
      free(hm->map[index]->document_id);
      free(hm->map[index]);
    }
  }

  free(hm->map);
  free(hm);
}
int hash(struct hashmap* hm, char* word){
  if(word == NULL){
    return 0;
  }

  int hash = 0;
  char* ptr = word;

  while(*ptr != '\0'){
    hash += *ptr;
    ptr++;
  }
   //returns the hash value within the bounds of the possible indices
  return hash % hm->num_buckets;
}
double compute_idf_of_word(char* word, unsigned long totalDocuments, struct hashmap* hm){
  double idf = 0;
  //gets the doc freq of that word
  double df_of_word;
  df_of_word = (double) get_df(word, hm);

  //check if df is 0, if it is, add one to denominator
  if(df_of_word == 0){
    //printf("df of word = 0\n");
    //printf("Calculating: log(%f/%f)\n", (double)totalDocuments, (double)df_of_word);
    idf = log10((double)totalDocuments/(double)(df_of_word + 1.0));
    //printf("IDF: %f\n", idf);
  }
  else{
    //printf("Calculating: log(%f/%f)\n", (double)totalDocuments, (double)df_of_word);
    idf = log10((double)totalDocuments/(double)df_of_word);
    //printf("IDF: %f\n", idf);
  }

  return idf;
}
void stop_word(struct hashmap* hm){
  int totalDocuments;
  char** document_list;

  //glob structure to get information
  glob_t glob_struct;
  int glob_val = glob("./documents/*.txt", 0, NULL, &glob_struct);

  if(glob_val == 1){
    return;
  }
  totalDocuments = glob_struct.gl_pathc;
  document_list = glob_struct.gl_pathv;

  int i;
  double check;


  //iterate through all of the buckets
  for(i = 0; i < hm->num_buckets; i++){
    //create a pointer
    struct llnode* current_word;
    //if the bucket is empty, go onto the next one
    if(hm->map[i] == NULL){
      continue;
    }
    //otherwise there is at least one word in the bucket
    else{
      //set ptr to first node
      current_word = hm->map[i];
      //iterate until last word node
      while(current_word != NULL){
        printf("On word: %s\n", current_word->word);
        //compute the idf of that word
        check = compute_idf_of_word(current_word->word, totalDocuments, hm);
        //if idf of that word is 0, remove that word
        if(check == 0){
          printf("Removing stop word: %s\n", current_word->word);
          //get rid of all words in list that are a stop word
          char* wordtoremove = (char*)malloc(sizeof(char)*strlen(current_word->word) + 1);
          strcpy(wordtoremove, current_word->word);
          for(i = 0; i < totalDocuments; i++){
            hm_remove(hm, wordtoremove, document_list[i]);
          }
        }
        else{
          //move to next word
          current_word = current_word->next;
        }

      }
      printf("\n");
    }

  }

  return;
}
int get_tf(char* word, char* document, struct hashmap* hm){
  //edge cases for passing in invalid values into hm_put
  if(hm == NULL){
      printf("Hashmap is not allocated.\n");
      return -1;
  }
  if(word == NULL){
  printf("Word is not allocated.\n");
    return -1;
  }
  if(document == NULL){
    printf("Document_ID is not allocated.\n");
    return -1;
  }

  //this gets the index to which bucket to go to
  int index = hash(hm, word);

  //check if the bucket is NULL
  if(hm->map[index] == NULL){
    //if it is null, it should return -1
    //printf("Bucket is empty.\n");
    return 0;
  }

  //if it not the first node, iterate through the linked list and check as you go
  //current node pointer, starts off at the head of the list
  struct llnode* current = hm->map[index];

  //iterate through the linked list by looking one ahead and checking the current node
  while(current->next != NULL){
    if(strcmp(current->word, word) == 0 && strcmp(current->document_id, document) == 0){
      return current->num_occurrences;
    }
    current = current->next;
  }

  //check the very last node if it is the word and docuement
  if(strcmp(current->word, word) == 0 && strcmp(current->document_id, document) == 0){
    return current->num_occurrences;
  }

  //if the node does not exist in the linked list, return 0
  return 0;


}
int get_df(char* word, struct hashmap* hm){
  //edge cases for passing in invalid values into hm_put
  if(hm == NULL){
      printf("Hashmap is not allocated.\n");
      return 0;
  }
  if(word == NULL){
  printf("Word is not allocated.\n");
    return 0;
  }

  //this gets the index to which bucket to go to
  int index = hash(hm, word);
  int df = 0;

  //check if the bucket is NULL
  if(hm->map[index] == NULL){
    //if it is null, it should return -1
    //printf("Bucket is empty.\n");
    return 0;
  }

  //if it not the first node, iterate through the linked list and check as you go
  //current node pointer, starts off at the head of the list
  struct llnode* current = hm->map[index];

  //iterate through the linked list by looking one ahead and checking the current node
  while(current->next != NULL){
    if(strcmp(current->word, word) == 0){
      df++;
    }
    current = current->next;
  }

  //check the very last node if it is the word and docuement
  if(strcmp(current->word, word) == 0){
    //printf("At the end of the list and it is the same word\n");
    df++;
  }
  //if the node does not exist in the linked list, return 0
  return df;
}

struct rankedllist* createllist(void) {
  /* Create an empty linked list. */
  struct rankedllist* list = malloc(sizeof(struct rankedllist));
  list->head = NULL;

  return list;
}

void add_by_idf(struct rankedllist *list, char* document_id, double tf_idf_score)
{
  //allocate memory for new song to add
  struct ranked_node* node = (struct ranked_node*)malloc(sizeof(struct ranked_node));
  node->document_id = document_id;
  node->tf_idf = tf_idf_score;
  node->next = NULL;

  //edge cases
  if (list->head == NULL)
  {
    list->head = node;
    return;
  }

  //create pointer to the head
  struct ranked_node* p = list->head;

  if(list->head->tf_idf < node->tf_idf)
  {
    list->head = node;
    node->next = p;
    return;
  }

  // //if the new node is smaller than the first node in the list
  // if(p->tf_idf > node->tf_idf)
  // {
  //   printf("in here\n");
  //   p->next = node;
  //   return;
  // }

  //iterate
  while (p->next != NULL)
  {
    //if the new node is in-between two nodes it can slip into
    if ((p->tf_idf > node->tf_idf) && (p->next->tf_idf < node->tf_idf))
    {
      p->next = node;
      node->next = p->next->next;
    }
    //if the next node has the same tfidf as the node we want to add
    if (p->next->tf_idf == node->tf_idf)
    {
      //while the next node afterwards is the same tfidf and not null an empty node
      while (p->next->tf_idf == node->tf_idf && p->next != NULL)
      {
        p = p->next;
      }

      node->next = p->next;
      p->next = node;

    }

    p = p->next;
  }
  if(p->tf_idf > node->tf_idf){
    p->next = node;
  }

  return;
}

void training(struct hashmap* hm){
  int i;

  int totalDocuments;
  char** document_list;

  //glob structure to get information
  glob_t glob_struct;
  int glob_val = glob("./documents/*.txt", 0, NULL, &glob_struct);

  if(glob_val == 1){
    return;
  }
  totalDocuments = glob_struct.gl_pathc;
  document_list = glob_struct.gl_pathv;

  FILE *filepointer;
  char string[MAXCHAR];

  //iterate through the total number of documents
  for(i = 0; i < totalDocuments; i++){
    //open file
    filepointer = fopen(document_list[i], "r");

    //if the file is empty
    if(filepointer == NULL){
      printf("File cannot be opened. Check again.");
      exit(1);
    }
    while(fscanf(filepointer, "%s", string) == 1){
    //if the word does not exist, put the word in with an occurrence of 1
    //suggested by Antonio during lab
    if(get_df(string, hm) == 0){
        hm_put(hm,string,document_list[i]);
    }
    else{
      hm_put(hm,string,document_list[i]);
    }
  }
  }
  //close the filepointer
  fclose(filepointer);

  //printhashmap(hm);

  //clean hashmap by removing all stop words
  //stop_word(hm);
  //printhashmap(hm);
  //free the glob stuff
  for(i = 0; i < totalDocuments; i ++){
    free(document_list[i]);
  }
  free(document_list);
  //hm_destroy(hm);
}

void printhashmap(struct hashmap* hm){
  int i;
  printf("NUMBER OF BUCKETS: %d\n", hm->num_buckets);
  for(i = 0; i < hm->num_buckets; i++){
    printf("Bucket: %d:\n", i);
    struct llnode* current = hm->map[i];
    if(current == NULL){
      printf("Empty\n");
      continue;
    }
    else{
      while(current->next != NULL){
        printf("Word: '%s' and Document: %s\n", current->word, current->document_id);
        current = current->next;
      }
      printf("Word: %s and Document: %s\n", current->word, current->document_id);
    }
  }
  return;
}


void search_query(struct hashmap* hm, unsigned long totalDocuments,  char *argv[]){
  unsigned long i = 0;

  //create a ranked llist to store the nodes to print
  struct rankedllist* ranked_docs = createllist();

  //total documents and list of docs
  char** document_list;
  char* current_doc;
  char* string;
  char* token;
  double tfidf;
  char* word;

  //glob structure to get information
  glob_t glob_struct;
  int glob_val = glob("./documents/*.txt", 0, NULL, &glob_struct);

  if(glob_val == 1){
    return;
  }
  totalDocuments = glob_struct.gl_pathc;
  document_list = glob_struct.gl_pathv;

  //iterate through all of the documents
  while(i < totalDocuments){
    tfidf = 0;
    //get the current document
    current_doc = document_list[i];
    //set string equal to queried search
    //perform deep copy of the string
    string = (char*)malloc(sizeof(char)*strlen(argv[2]) + 1);
    strcpy(string, argv[2]);
    //set first word in the string to be in token
    token = strtok(string, " ");
    //iterate through each word
    while(token != NULL){
      //malloc space for the word
      word = (char*)malloc(sizeof(char)*strlen(token) + 1);
      //deep copy of token into word
      strcpy(word, token);
      //sums up all of the tfidfs for one document
      tfidf += get_tf(word, current_doc, hm) * compute_idf_of_word(word, totalDocuments, hm);
      //get next word
      free(word);
      token = strtok(NULL, " ");
    }
    //add the node by tfidf score
    add_by_idf(ranked_docs, current_doc, tfidf);
    //move onto next document
    i++;
  }
  //print_llist(ranked_docs);
  write_to_doc(ranked_docs);
  printf("%s : %f\n", ranked_docs->head->document_id, ranked_docs->head->tf_idf);
  return; 
}

void write_to_doc(struct rankedllist* rankedlist){
  struct ranked_node* ptr =  rankedlist->head;

  FILE *filepointer;

  //iterate through the total number of documents
  //open file in write and read mode
  filepointer = fopen("search_scores.txt", "w+");

  //if the file is empty
  if(filepointer == NULL){
    printf("File cannot be opened. Check again.");
    exit(1);
  }

  //iterate until you cannot anymore
  while(ptr != NULL){
    //write to the file with the sorted ranked llist
    fprintf(filepointer, "%s " , ptr->document_id);
    fprintf(filepointer, ": %f\n", ptr->tf_idf);
    ptr = ptr->next;
  }
}

void print_llist(struct rankedllist* rankedlist){
  struct ranked_node* current = rankedlist->head;

  if(current == NULL){
    printf("List is empty.\n");
    return;
  }
  else{
    while(current != NULL){
      printf("Node values: %s with a tf_idfs core of: %f\n", current->document_id, current->tf_idf);
      current = current->next;
    }
  }
  return;
}
