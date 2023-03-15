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
//TODO
int main(int argc, char* argv[])
{

	return 0;
}
