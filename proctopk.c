#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// Ipek Oztas - Emre Karatas
// a data structure to keep words and counts together. This is a linked list application.
struct dataItem
{
	char* word;
	struct dataItem* next;
	int wordCount;
};
// function to push word count and word itself to the linked list structure
void pushData(struct dataItem** head, char* wordToAdd, int wordCount)
{
	struct dataItem* newDataItem = (struct dataItem*)malloc(sizeof(struct dataItem)); 
  

    	newDataItem->word  = wordToAdd; 
  
    	newDataItem->wordCount = wordCount;

    	newDataItem->next = *head;
    	
    	(*head) = newDataItem; 
}
//TODO
void sortDataItems(struct dataItem** anItem)
{

}
//TODO
int main(int argc, char* argv[])
{

	return 0;
}
