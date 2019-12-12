#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define MAXCHAR 1024

int main(int argc, char** argv){

    if (argc < 3){
      printf("Too few Arguments Error");
      return 0;
    }
    //stores the number of buckets from user
    int num_buckets = atoi(argv[1]);
    
    int totalDocuments;

    //glob structure to store list of documents
    glob_t glob_struct;
    int glob_val = glob("./documents/*.txt", 0, NULL, &glob_struct);
    if(glob_val == 1){
      return 0;
    }

    //initialize values of total documents
    totalDocuments = glob_struct.gl_pathc;
    glob_val = 0;

    //create hashmap with specified number of buckets
    struct hashmap* hm = hm_create(num_buckets);

    //preprocessing the map with removed stop words
    training(hm);

    //searching for the words and outputting the most relevant document
    search_query(hm, totalDocuments, argv);

    return 0;
}
