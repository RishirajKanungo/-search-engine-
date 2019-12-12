# -search-engine-
Local search engine to find move relevant document containing queried search.

## Getting Started
The notes below will explain all you need to do to run the local "search-engine". This includes: Makefile, running C code, syntax to search.

### Prerequisites

If on Windows, make sure that you have downloaded C-Program Compiler (gcc). If on Mac/Linux, continue reading.

## Running "search-engine"
To run the engine, first make sure that you are in the 'src' folder of the repo. Once there, write into your terminal:
```
make
```

This will create the executable files to run the engine. Once the executables are created, ensure that you have filled the 'documents' folder with .txt files. After doing so, you can search for the most relevant document that you query for with the following command: 
```
./search 5 "Hello World"
```

The first parameter is the executable. The second argument is the number of buckets to create the hashmap with for increasing speed. The final argument is the string the user prompts the program to search for.

### Breaking down the calculation of relevance

The simplest way to process a search query is to interpret it as a Boolean query – either document contain all the words in the document or they do not. However, this usually results in too few or too many results and further does not provide a ranking that returns the documents that may be most likely to be useful to the user. We wish to assign a ‘score’ to how well a document matched the query, and to do this we need a way to assign a score to a document-query pair. To do this, consider some questions such as ‘how many times did a word occur in the document’, ‘where the word occurs and how important the word is’. The goal of relevance functions (which is the ‘secret sauce’ of search engines) is to determine a score that co-relates to the relevance of a document.
The term frequency-inverse document frequency (tf-idf) method is one of the most common weighting algorithms used in many information retrieval problems. This starts with a bag of words model – the query is represented by the occurrence counts of each word in the query (which means the order is lost – for example, “john is quicker than mary” and “mary is quicker than john” both have the same representation in this model).

### term frequency (tf)

Term Frequency: The term frequency tf of a term (word) w in document i is a measure of w,i frequency of the word in the document. Using raw frequency, this is the number of times that term (word) appears in the document i. 

### document frequency (df)
The document frequency df is the number of documents that contain the term w

### inverse document frequency (idf)
The inverse document frequency,idf, of term w is defined as idf = log (N/df ), where N is the total number of documents in the database (i.e., being searched).
