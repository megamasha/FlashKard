/* uses legacy C functions and variables (below) and structs/defines (using the header
file).

The importdatabase() performs the functions of the old loaddatabase(),
passing correct parameters to getrecordsfromfile(), which reads the file into
vocab structs in memory.
It then converts these vocab structs into flashcards in a cardpack and unloads
the vocab structs from memory.
*/

#include "svfileops.h"
#include <QString> // required to interface with Qt (importdatabase())
#include "flashcard.h"
#include "cardpack.h"

FILE * inputfile = NULL;
struct listinfo n2l, norm, known, old;

void importdatabase(QString fileToImport)
{
    QByteArray temporaryIntermediate = fileToImport.toAscii();
    char * inputfilename = temporaryIntermediate.data();

    //identify csv or ~sv file
    char separator = '~';
    if (inputfilename[strlen(inputfilename)-3] == 'c') separator = ',';

    //check some records are imported from the file
    if (getrecordsfromfile(inputfilename,separator))
    {
        loadvocabintoflashcards();
        //clear the vocab structs
        unloaddatabase();
    }
}

int getrecordsfromfile(char * inputfilename,char separator)
{
    int goodcounter = 0,badcounter = 0;
    struct vocab * newvocab;
    struct listinfo * newvocablist;
    if (!(inputfile = fopen(inputfilename, "r")))
    {
        fprintf(stderr,"Unable to read input file'%s'. File does not exist or is in use.\n",inputfilename);
    }
    else
    {
        qDebug("Opened input file %s, reading contents...\n",inputfilename);
        while (!feof(inputfile))
        {
            newvocab = (struct vocab *)malloc(sizeof(struct vocab));
            if (!newvocab)
            {
                qDebug("Memory allocation failed!\n");
                return goodcounter;
            }
            else
            {
                newvocab->question=newvocab->answer=newvocab->info=newvocab->hint=NULL;
                newvocab->question=readtextfromfile(MAXTEXTLENGTH,separator);
                newvocab->answer=readtextfromfile(MAXTEXTLENGTH,separator);
                newvocab->info=readtextfromfile(MAXTEXTLENGTH,separator);
                newvocab->hint=readtextfromfile(MAXTEXTLENGTH,separator);
                newvocab->right=readnumberfromfile(1,separator);
                newvocab->counter=readnumberfromfile(0,separator);
                newvocab->known=readnumberfromfile(3,separator);

                switch (newvocab->known)
                {
                    case 0: newvocablist = &n2l;break;
                    case 1: newvocablist = &norm;break;
                    case 2: newvocablist = &known;break;
                    case 3: newvocablist = &old;break;
                    default: {newvocablist = &n2l;fprintf(stderr,"invalid known level! Defaulting to 0");}
                }

                addtolist(newvocab,newvocablist);
                if (newvocab->question==NULL||newvocab->answer==NULL)
                {
                    badcounter++;
                    qDebug("Removing faulty vocab record (%d) created at line %i of input file...\n",badcounter,(goodcounter+badcounter));
                    removefromlist(newvocab,newvocablist,1);
                }
                else goodcounter++;
            }
        }
        fclose(inputfile);
        qDebug("...finished.\n%i entries read from %s.\n%i faulty entries encountered.\n\n",goodcounter,inputfilename,badcounter);
    }
    return goodcounter;
}

char * readtextfromfile(int maxchars,char separator)
{
    int i=0;
    int ch;
    char * target = (char *)malloc(maxchars+1); //allocate memory for new string
    if (!target) {qDebug("Memory allocation failed!\n");return 0;}//return 0 and print error if alloc failed

    ch=getc(inputfile);
    if (ch==separator||ch==EOF){free(target);return NULL;}//if field is blank (zero-length), return null pointer (||EOF added because it hangs on blank database)
    while (isspace(ch))
    {
        ch = getc(inputfile);//cycle forward until you reach text
        if (ch == separator||ch=='\n'||ch==EOF) {free(target);return NULL;}//if no text found(reached separator before anything else), return null pointer
    }
    if (ch=='"') //Entry is in quotes (generated by excel when exporting to .csv and field contains a comma)
    {
        ch=getc(inputfile);//move to next character after the quotes
        while (i<(maxchars-1) && ch!='"' && ch!='\n')//stop when you reach the end quotes, end of line, or when text too long
        {
            target[i++]=ch;
            ch = getc(inputfile); //copy name from file to target, one char at a time
        }
        ch=getc(inputfile);//consume separator that follows quotes, so next field does not appear empty (this was a bug... SQEESH!)
    }
    else //entry is not in quotes, so char is currently first letter of string
    {
        while (i<(maxchars-1) && ch!=separator && ch!='\n')//stop when you reach separator, end of line, or when text too long
        {
            target[i++]=ch;
            ch = getc(inputfile); //copy name from file to target, one char at a time
        }
    }
    target[i] = '\0';//terminate string
    return target;
}

int readnumberfromfile (int maxvalue,char separator)
{
    int number, i=0;
    char ch;
    char * buff = (char *)malloc(10+1);//allocate enough space for an 10-digit number and a terminating null
    if (!buff) {qDebug("Memory allocation failed!\n");return 0;}//return 0 and print error if alloc failed
    if (!maxvalue) maxvalue=MAXINTVALUE;

    ch=getc(inputfile);
    while (!isdigit(ch))
    {
        if (ch == separator||ch=='\n'||ch==EOF) {fprintf(stderr,"Format error or field missing in file\nExpected number, but found '%c'. Replacing with '0'\n",ch);free(buff);return 0;}//if no number found(reached separator before digit), print error, free buff and return 0
        ch = getc(inputfile);//cycle forward until you reach a digit
    }
    while (i<10 && ch!=separator && ch!='\n')//stop when you reach separator, end of line, or when number too long
    {
        buff[i++]=ch;
        ch = getc(inputfile); //copy number from file to buff, one char at a time
    }
    buff[i] = '\0';//terminate string
    number = atoi(buff)<=maxvalue ? atoi(buff) : maxvalue;//convert string to number and make sure it's in range
    free(buff);
    return number;
}

struct vocab * addtolist(struct vocab * newentry, struct listinfo * list)
{
    if (!list->head)//if head is null, there is no list, so create one
    {
        list->head = list->tail = newentry;//this is the new head and tail
        list->entries = newentry->index = 1;
        newentry->next = NULL;
    }
    else//just appending to the list
    {
        list->tail->next = newentry;//adjust current tail to point to new entry
        list->tail = newentry;//make the new entry the new tail
        newentry->index=++list->entries;
        newentry->next = NULL;
    }
    //give the entry the appropriate 'known' level for this list (for calculating scores, and deducing which list its in without searching)
    if (list==&n2l) newentry->known = 0;
    else if (list==&norm) newentry->known = 1;
    else if (list==&known) newentry->known = 2;
    else if (list==&old) newentry->known = 3;
    else {fprintf(stderr,"Unable to correctly add vocab entry to list!");return NULL;}

    return newentry;
}

int removefromlist(struct vocab * entry, struct listinfo * list,int freeup)
{
    struct vocab * prev;
    if (list->head == entry) //if entry being deleted is first in the list
    {
        if (list->tail == entry) //if entry is only item in the list
        {
            list->head = list->tail = NULL;
        }
        else //if first in list, but not last
        {
            list->head = entry->next;
        }
    }
    else //entry is not first in list, so set prev to point to previous entry
    {
        prev = list->head;
        while (prev->next!=entry)
        {
            prev=prev->next;
            if (!prev)
            {
                qDebug("Trying to delete an entry from a list it's not in!!\n");
                return 0;
            }
        }
        if (list->tail == entry)//if entry is at the end of the list
        {
            list->tail = prev;
            list->tail->next = NULL;
        }
        else //if entry is somewhere in middle of list
        {
            prev->next=entry->next;
        }
    }//this entry is now not pointed to in any list
    list->entries--;
    /*following line removed because it could theoretically break a list if the entry was removed from a list after it had been added to another
    entry->next = NULL;//and doesn't point to anything either*/
    reindex(list);
    if (freeup) //if freeup is set, this also wipes the record and frees up the memory associated with it
    {
        if(entry->question) free(entry->question);
        if(entry->answer) free(entry->answer);
        if(entry->info) free(entry->info);
        if(entry->hint) free(entry->hint);
        if(entry) free(entry);
    }
    return 1;
}

void reindex (struct listinfo * list)
{
    int counter = 1;
    struct vocab * workingentry = list->head;
    while (workingentry)
    {
        workingentry->index = counter++;
        workingentry=workingentry->next;
    }
    if (list->entries!=counter-1) qDebug("Reindexing Error!\n");
}

int unloaddatabase()
{
    int l = 0,counter = 0;
    struct vocab * entry;
    struct listinfo * list; //assigned by switch with l, cycles through all the lists
    for (;l<=3;l++)
    {
        switch (l)
        {
            case 0: {list = &n2l;break;}
            case 1: {list = &norm;break;}
            case 2: {list = &known;break;}
            case 3: {list = &old;break;}
            default: {fprintf(stderr,"List pointer error!\n");return 0;}
        }
        while (list->head!=NULL)
        {
            entry = list->head;
            removefromlist(entry,list,1);
            counter++;
        }
    }
    qDebug("Unloaded %i entries from memory.\n",counter);
    return 1;
}

int loadvocabintoflashcards()
{
    int l = 0;
    struct vocab * entry;
    struct listinfo * list; //assigned by switch with l, cycles through all the lists
    for (;l<=3;l++) //cycle through all levels
    {
        switch (l)
        {
            case 0: {list = &n2l;break;}
            case 1: {list = &norm;break;}
            case 2: {list = &known;break;}
            case 3: {list = &old;break;}
            default: {fprintf(stderr,"List pointer error!\n");return 0;}
        }
        entry = list->head;
        while (entry!=NULL)
        {
            knownLevel_t knownLevel = (knownLevel_t)l;
            bool lastCorrect = (entry->right) ? true : false;
            int currentStreak = entry->counter;
            int levelUp = entry->counter;
            QString qu = entry->question;
            QString an = entry->answer;
            QString in = entry->info;
            QString hi = entry->hint;

            flashCard * fc = new flashCard(qu,an,in,hi,knownLevel,lastCorrect,currentStreak,levelUp);

            mainPack.addCard(*fc,knownLevel);

            entry = entry->next;
        }

    } //for: cycling through lists

    return 1;
}
