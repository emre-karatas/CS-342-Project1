#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <sys/time.h>


// Ipek Oztas - Emre Karatas
// a data structure to keep words and counts together. This is a linked list application.
struct dataItem
{
    char* word;
    struct dataItem* next;
    struct dataItem* previous;
    int wordCount;
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

void readFiles(char* fileName)
{
	FILE* file;
	char word[64];
	file = fopen(fileName,"r");
	char* current;
	
	while ( fscanf(file, "%s", word) == 1 ) 
	{
        	current = strdup(word);
        	for ( int i = 0; current[i] != '\0'; i++ ) 
        	{
            		if ( current[i] >= 'a' && current[i] <= 'z' ) 
            		{
                		current[i] = current[i] - 32;
            		}
            	}
            	head = pushData(head, current);
		free(current); // free the allocated memory after using it
        }
    fclose(file);
}



void printData(struct dataItem* head, FILE* outputFile) {
    if (head == NULL) {
        return;
    }
    fprintf(outputFile, "%s %d\n", head->word, head->wordCount);
    printData(head->next, outputFile);
}

void printOutputFile()
{
    FILE* outputFile = fopen("out.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    selectionSort(head);
    printData(head, outputFile);
    if (fclose(outputFile) != 0) {
        perror("Error closing output file");
        exit(EXIT_FAILURE);
    }
}

//TODO
int main(int argc, char* argv[])
{
	if ( argc < 5 ) {
        printf("You have entered insufficient number of arguments! Usage: proctopk <K> <outfile> <N> <infile1> .... <infileN>\n");
        return -1;
    }
    char* outfile = argv[3];
    int n = atoi(argv[4]);
    printf("%d", n);
    char* fileNames[n];
    for ( int i = 0; i < n; i++ ) {
        fileNames[i] = argv[i + 5];
    }
    

	readFiles("in1.txt");
	printf("file is read");
    FILE* outputFile;
    outputFile = fopen(outfile, "w");
    for ( int i = 0; i < n; i++ ) {
        fprintf(outputFile, "%s", fileNames[i]);
    }
    
    fclose(outputFile);
	return 0;
}
