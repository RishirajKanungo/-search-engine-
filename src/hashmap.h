#ifndef HASHMAP_H
#define HASHMAP_H
#include <glob.h>
#include <math.h>

struct llnode {
        char* word;
        char* document_id;
        int num_occurrences;
        struct llnode* next;

};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};

struct glob_t{
        size_t gl_path;
        char** gl_paths;
        size_t gl_offs;
};

struct rankedllist
{
  struct ranked_node* head;
};

struct ranked_node{
        char* document_id;
        double tf_idf;
        struct ranked_node* next;
};

struct hashmap* hm_create(int num_buckets);
void hm_put(struct hashmap* hm, char* word, char* document_id);
void hm_remove(struct hashmap* hm, char* word, char* document_id);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);
double compute_idf_of_word(char* word, unsigned long totalDocuments, struct hashmap* hm);
void stop_word(struct hashmap* hm);
int get_tf(char* word, char* document, struct hashmap* hm);
int get_df(char* word, struct hashmap* hm);
void training(struct hashmap* hm);
void search_query(struct hashmap* hm, unsigned long totalDocuments,  char *argv[]);
struct rankedllist* createllist(void);
void write_to_doc(struct rankedllist* rankedlist);
void add_by_idf(struct rankedllist *list, char* document_id, double tf_idf_score);
void printhashmap(struct hashmap* hm);
void print_llist(struct rankedllist* rankedlist);

#endif
