#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/wait.h>

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


int main(int argc, char* argv[])
{
    printf ("this is parent with pid is: %d\n", getpid());

    /* the size (in bytes) of shared memory object */
    const int SIZE = 4096;
    /* name of the shared memory object */
    const char *name = "OS";
    /* strings written to shared memory */
    const char *message_0 = "Hello";
    const char *message_1 = "World!";
    /* shared memory file descriptor */
    int fd;
    /* pointer to shared memory obect */
    char *ptr;
    /* create the shared memory object */
    fd = shm_open(name,O_CREAT | O_RDWR,0666);
    /* configure the size of the shared memory object */
    ftruncate(fd, SIZE);
    /* memory map the shared memory object */
    ptr = (char *)
    mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    


	if ( argc < 5 ) {
        printf("You have entered insufficient number of arguments! Usage: proctopk <K> <outfile> <N> <infile1> .... <infileN>\n");
        return -1;
    }
    
    char* outfile = argv[3];
    int N= atoi(argv[4]);
    printf("%d", N);
    char* fileNames[N];
    for ( int i = 0; i < N; i++ ) {
        fileNames[i] = argv[i + 5];
    }
    
    readFiles("in1.txt");
    FILE* outputFile;
    outputFile = fopen(outfile, "w");
    printOutputFile();
    
    fclose(outputFile);

    pid_t n;
    int k,r;
    unsigned int s;
    for (int i = 0; i < N; i++) {
        pid_t pid, pid1;
        /* fork a child process */
        pid = fork();
        if (pid < 0) { /* error occurred */
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid == 0) { /* child process */
            pid1 = getpid();
            //printf("child: pid = %d",pid); /* A */
            //printf("child: pid1 = %d",pid1); /* B */
            /* write to the shared memory object */
            
            printf("child process %d with ID %d and parent ID %d is created\n", i+1, getpid(), getppid());
           
            char s[] = "ipek";
            memcpy(ptr, s, sizeof(char) * 4);

            ptr += sizeof(char)*4;
            printf("%s", "sorun yok");

            exit(0);
        }
        else { /* parent process */
            pid1 = getpid();
            //printf("parent: pid = %d",pid); /* C */
            //printf("parent: pid1 = %d",pid1); /* D */
            wait(NULL);
        }
    }

    for (int i = 0; i < 3; i++){
        wait(NULL);
    }

    

    

    /* open the shared memory object */
    //fd = shm_open(name, O_RDONLY, 0666);
    /* memory map the shared memory object */
    ptr = (char *)
    mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* read from the shared memory object */
    printf("%s",(char *)ptr);

    /* remove the shared memory object */
    shm_unlink(name);
    return 0;
    
    
}
