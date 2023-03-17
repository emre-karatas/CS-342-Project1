#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_FILES 100

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


void printData(struct dataItem* head, FILE* outputFile, int* count) {
    if (head == NULL || *count <= 0) {
        return;
    }
    fprintf(outputFile, "%s %d\n", head->word, head->wordCount);
    (*count)--;
    printData(head->next, outputFile, count);
}


/* Function to write output file */
void printOutputFile(char* outputFileName, int topKWords) {
    int count = topKWords;
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    selectionSort(head);
    printData(head, outputFile, &count);
    if (fclose(outputFile) != 0) {
        perror("Error closing output file");
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char* argv[]) {
    printf("This is parent with pid: %d\n", getpid());

    const int SIZE = 4096;
    const char *name = "OS";
    const char *message_0 = "Hello";
    const char *message_1 = "World!";
    int fd;
    char *ptr;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (argc < 5) {
        printf("You have entered an insufficient number of arguments! Usage: proctopk <K> <outfile> <N> <infile1> .... <infileN>\n");
        return -1;
    }
    int topKWords = atoi(argv[1]);
    char* outfile = argv[2];
    int N = atoi(argv[3]);
    char* fileNames[MAX_FILES];
    for (int i = 0; i < N; i++) {
        fileNames[i] = argv[i + 4];
    }

    // Call readFiles function for each input file
    for (int i = 0; i < N; i++) {
        readFiles(fileNames[i]);
    }

    // Call printOutputFile to write output to file
    printOutputFile(outfile,topKWords);
    printf("done");

    // Create child processes
    pid_t pid;
    for (int i = 0; i < N; i++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        else if (pid == 0) { // child process
            printf("Child process %d with ID %d and parent ID %d is created\n", i+1, getpid(), getppid());

            // Write to shared memory
            char s[] = "ipek";
            memcpy(ptr, s, sizeof(char) * 4);

            ptr += sizeof(char)*4;

            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Read from shared memory
    printf("%s\n", (char *)ptr);

    // Remove shared memory object
    shm_unlink(name);

    return 0;
}
