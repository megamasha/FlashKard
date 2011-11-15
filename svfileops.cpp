#include "svfileops.h"

void loaddatabase()//select which database to load
{
    char separator = '~';
    char * tildesep = ".~sv";
    char * commasep = ".csv";
    char * extension = tildesep;
    char * deffilename = DINPUTFILENAME;
    char * inputfilename = (char *)malloc(MAXTEXTLENGTH+1);
    if (!inputfilename) {fprintf(stderr, "Error allocating memory for filename input");exit(1);}
    strcpy(inputfilename,deffilename);
    printf("Loading...\nLoad default database: %s? (y/n)",inputfilename);
    if (!getyesorno())//import user specified database
    {
        printf("Default file type is .~sv. Load .~sv file? (y/n)");
        if (getyesorno()) //import .~sv file
        {
            printf("Enter name of .~sv file to load:\n");
        }
        else //alternative options
        {
            printf("Import .csv file instead? (y/n)");
            if (getyesorno()) //import .csv file
            {
                separator = ',';
                extension = commasep;
                printf("Enter name of .csv file to import:\n");
            }
            else //not loading a file
            {
                printf("No database file selected. No database loaded!\n");
                free(inputfilename);
                return;
            }
        }
        inputfilename=validfilename(gettextfromkeyboard(inputfilename,MAXTEXTLENGTH),extension);
    }
    getrecordsfromfile(inputfilename,separator);
    inputfilename=validfilename(inputfilename,".~sv");
    strcpy(currentfilename,inputfilename);
    free(inputfilename);
}

char * validfilename (char * filename, char * extension)//filename validation
{
    int i, j=0, alreadyvalid=1;
    //check filename is longer than the extension
    if (strlen(filename)>strlen(extension))
    {
        //if so, see if string already contains given extension
        for(i=0;i<=strlen(extension);i++)
        {
            if (filename[(strlen(filename))-i]!=extension[(strlen(extension))-i]) alreadyvalid=0;
        }
        if (alreadyvalid) return filename;//is valid filename, return it
    }
    //find first 'dot' or null in string to append file extension (first character can be dot for hidden unix files)
    for (i=1;filename[i]!='.'&&i<strlen(filename);i++);
    //add extension and return result
    while (i<MAXTEXTLENGTH && j<=strlen(extension))
    {
        filename[i]=extension[j];
        i++;j++;
    }
    if (i==MAXTEXTLENGTH) fprintf(stderr,"Filename reached maximum length including extension, possibly truncated!\n");
    return filename;
}

void getrecordsfromfile(char * inputfilename,char separator)
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
        printf("Opened input file %s, reading contents...\n",inputfilename);
        while (!feof(inputfile))
        {
            newvocab = (struct vocab *)malloc(sizeof(struct vocab));
            if (!newvocab)
            {
                printf("Memory allocation failed!\n");
                return;
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
                }

                addtolist(newvocab,newvocablist);
                if (newvocab->question==NULL||newvocab->answer==NULL)
                {
                    badcounter++;
                    printf("Removing faulty vocab record (%d) created at line %i of input file...\n",badcounter,(goodcounter+badcounter));
                    removefromlist(newvocab,newvocablist,1);
                }
                else goodcounter++;
            }
        }
        fclose(inputfile);
        printf("...finished.\n%i entries read from %s.\n%i faulty entries encountered.\n\n",goodcounter,inputfilename,badcounter);
    }
    return;
}

char * readtextfromfile(int maxchars,char separator)
{
    int i=0;
    char ch;
    char * target = (char *)malloc(maxchars+1); //allocate memory for new string
    if (!target) {printf("Memory allocation failed!\n");return 0;}//return 0 and print error if alloc failed

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
    if (!buff) {printf("Memory allocation failed!\n");return 0;}//return 0 and print error if alloc failed
    if (!maxvalue) maxvalue=MAXINTVALUE;

    ch=getc(inputfile);
    while (!isdigit(ch))
    {
        if (ch == separator||ch=='\n'||ch==EOF) {fprintf(stderr,"Format error or field missing in file\nExpected number, but found '%c'. Replacing with '0'\n",separator,ch);free(buff);return 0;}//if no number found(reached separator before digit), print error, free buff and return 0
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
