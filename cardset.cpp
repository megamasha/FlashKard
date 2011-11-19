/*This is basically a linked list*/

#include "cardset.h"

cardSet::cardSet()
{
    head = tail = NULL;
    cardsInSetCounter = 0;
}

bool cardSet::addCard(flashCard & newCard)
//assumes set will never contain more than MAXINT cards.
//does not allocate any memory, so should never fail.
{
    if (head == NULL) //if head is null, list is empty, so make this the head and tail
    {
        head = tail = & newCard;//this is the new head and tail
    }
    else //just appending to the list
    {
        tail->next = & newCard;//adjust current tail to point to new entry
        tail = & newCard;//make the new entry the new tail
    }

    newCard.next = NULL;
    cardsInSetCounter++;

    return true;
}

bool cardSet::removeCard(flashCard &oldCard)
{
    if (head == & oldCard) //if entry being deleted is first in the list
    {
        if (tail == & oldCard) //if entry is only item in the list
        {
            head = tail = NULL;
        }
        else //if first in list, but not last
        {
            head = oldCard.next;
        }
    }
    else //entry is not first in list, so set prev to point to previous entry
    {
        flashCard * prev = head;
        while (prev->next!=&oldCard)
        {
            prev=prev->next;
            if (prev == NULL)
            {
                qCritical("Trying to delete an entry from a list it's not in!!\n");
                return false;
            }
        }

        if (tail == & oldCard)//if entry is at the end of the list
        {
            tail = prev;
            tail->next = NULL;
        }
        else //if entry is somewhere in middle of list
        {
            prev->next=oldCard.next;
        }
    }//this entry is now not pointed to in any list

    cardsInSetCounter--;

    return true;
}
