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
	int wordCount;
};
// function to push word count and word itself to the linked list structure
void pushData(struct dataItem* head, char* wordToAdd,int wordCount)
{
	struct dataItem* newDataItem = (struct dataItem*)malloc(sizeof(struct dataItem)); 
  
    	newDataItem->word  = wordToAdd; 
  
    	newDataItem->wordCount = wordCount; 

    	newDataItem->next = head;
    	
    	head = newDataItem; 
}
void sortDataItems(struct dataItem* anItem)
{
	char* tempWord;
	int tempCount;
	struct dataItem *temp1;
	struct dataItem *temp2;
	for(temp1 = anItem; temp1!=NULL; temp1 = temp1->next)
	{
		for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next)
		{
			if(strcmp(temp1->word,temp2->word) > 0)
			{
				tempWord = temp1->word;
				tempCount = temp1->wordCount;
				
                		temp1->word = temp2->word;
                		temp2->word = tempWord;
                		temp1->wordCount = temp2->wordCount;
                		temp2->wordCount = tempCount;
			}
		}
	}

}
struct dataItem* head = NULL;
void readFiles(char* fileName)
{
	FILE* file;
	char word[64];
	file = fopen(fileName,"r");
	char* current;
	
	while ( fscanf(file, "%s", word) == 1 ) 
	{

        	// might leak memory
        	current = strdup(word);
        
        	for ( int i = 0; current[i] != '\0'; i++ ) 
        	{
            		if ( current[i] >= 'a' && current[i] <= 'z' ) 
            		{
                		current[i] = current[i] - 32;
            		}
            	}
        }
  	int index = 1;
        pushData(head, current,index);
}
void printData(struct dataItem* root, FILE* outputFile, int* count) {
    if ( root == NULL ) {
        return;
    }
    if ( (*count) == 0 ) {
        fprintf(outputFile, "%s %d", root->word, root->wordCount);
    }
    else {
        fprintf(outputFile, "\n%s %d", root->word, root->wordCount);
    }
    (*count)++;
    printData(root->next, outputFile, count);
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
