#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#define SHM_SIZE 1024
#define MAX_FILENAMES 10


// Ipek Oztas - Emre Karatas
// a data structure to keep words and counts together. This is a linked list application.
struct dataItem
{
    char* word;
    struct dataItem* next;
    struct dataItem* previous;
    int wordCount;
};

struct shared_data {
    int remaining_children;
    char data[SHM_SIZE];
};

struct dataItem* head = NULL;
struct dataItem* tail = NULL;

struct dataItem* pushData(struct dataItem* head, char* word)
{
    // Check if the word already exists in the linked list
    struct dataItem* current = head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            current->wordCount++;
            return head;
        }
        current = current->next;
    }
    
    // If the word does not exist, create a new node for it
    struct dataItem* newItem = (struct dataItem*)malloc(sizeof(struct dataItem));
    if (newItem == NULL) {
        // Handle memory allocation error
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    newItem->word = strdup(word);
    if (newItem->word == NULL) {
        // Handle memory allocation error
        printf("Memory allocation error\n");
        free(newItem);
        exit(EXIT_FAILURE);
    }
    
    newItem->wordCount = 1; 
    newItem->next = NULL;
    newItem->previous = tail;
    
    if (head == NULL) {
        head = newItem;
    }
    
    if (tail != NULL) {
        tail->next = newItem;
    }
    
    tail = newItem;
    
    return head;
}


void swap(struct dataItem* a, struct dataItem* b) {
    char* tempWord = a->word;
    int tempCount = a->wordCount;
    a->word = b->word;
    a->wordCount = b->wordCount;
    b->word = tempWord;
    b->wordCount = tempCount;
}


void selectionSort(struct dataItem* start) 
{
    if (start == NULL) 
    {
        return;
    }
    
    struct dataItem* ptr1, * ptr2, * min;
    
    for (ptr1 = start; ptr1->next != NULL; ptr1 = ptr1->next) 
    {
        min = ptr1;
        
        for (ptr2 = ptr1->next; ptr2 != NULL; ptr2 = ptr2->next) 
        {
            if (ptr2->wordCount > min->wordCount) 
            {
                min = ptr2;
            }
        }
        
        if (min != ptr1) 
        {
            swap(ptr1, min);
        }
    }
}

void readFiles(int numFiles, char* fileNames[])
{
    char word[64];
    char* current;
    
    for (int i = 0; i < numFiles; i++) {
        FILE* file = fopen(fileNames[i], "r");
        if (file == NULL) {
            printf("Could not open file %s\n", fileNames[i]);
            continue;
        }
        
        while (fscanf(file, "%s", word) == 1) {
            current = strdup(word);
            for (int j = 0; current[j] != '\0'; j++) {
                if (current[j] >= 'a' && current[j] <= 'z') {
                    current[j] = current[j] - 32;
                }
            }
            head = pushData(head, current);
            free(current);
        }
        
        fclose(file);
    }
}


void printData(struct dataItem* head, FILE* outputFile) {
    if (head == NULL) {
        return;
    }
    fprintf(outputFile, "%s %d\n", head->word, head->wordCount);
    printData(head->next, outputFile);
}

void printOutputFile(char* outfile, int wordCount, struct dataItem* data) {
    FILE* outputFile = fopen(outfile, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Sort the data by word count
    selectionSort(data);

    // Write the data to the output file
    for (int i = 0; i < wordCount; i++) {
        fprintf(outputFile, "%s %d\n", data[i].word, data[i].wordCount);
    }

    if (fclose(outputFile) != 0) {
        perror("Error closing output file");
        exit(EXIT_FAILURE);
    }
}

int main()
{
	// todo
}

