#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// Ipek Oztas - Emre Karatas
// a data structure to keep words and counts together. This is a linked list application.
struct dataItem
{
	char word[64];
	struct dataItem* next;
	int wordCount;
};
// function to push word count and word itself to the linked list structure
void pushData(struct dataItem** head, char* wordToAdd,int wordCount)
{
	struct dataItem* newDataItem = (struct dataItem*)malloc(sizeof(struct dataItem)); 
  
    	newDataItem->word  = wordToAdd; 
  
    	newDataItem->wordCount = wordCount; 

    	newDataItem->next = *head;
    	
    	(*head) = newDataItem; 
}
void sortDataItems(struct dataItem** anItem)
{
	char tempWord[64];
	int tempCount;
	struct dataItem temp1;
	struct dataItem temp2;
	for(temp1 = *anItem; temp1!=NULL; temp1 = temp1->next)
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
	
	while ( fscanf(file, "%s", word) == 1 ) 
	{

        	// might leak memory
        	char* current = strdup(word);
        
        	for ( int i = 0; current[i] != '\0'; i++ ) 
        	{
            		if ( current[i] >= 'a' && current[i] <= 'z' ) 
            		{
                		current[i] = current[i] - 32;
            		}
            	}
        }
  	int index = 1;
        head = pushData(head, current,index);
    }
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
    printTree(root->next, outputFile, count);
}

//TODO
int main(int argc, char* argv[])
{

	return 0;
}
