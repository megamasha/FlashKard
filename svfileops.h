/* Most of this code is taken from or based on the c source file for the
original Vocab tester Version 1 (http://github.com/megamasha/Vocab-Tester)
*/

#ifndef SVFILEOPS_H
#define SVFILEOPS_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <QString> // required to interface with Qt (importdatabase())

#define MAXTEXTLENGTH 256
#define MAXINTVALUE 2147483647

struct vocab
{
    int index; //identifies the entry in the list, allowing it to be selected by use of a random number
    char * question;//pointer to question text
    char * answer;//pointer to the answer text, which is required for the response to be considered correct
    char * info;//pointer to optional extra text giving advice such as to how to format the response
    char * hint;//pointer to optional text giving a clue to the answer
    int right;//indicates whether counter is counting correct or incorrect responses
    int counter;//counts how many times in a row the answer has been correct/incorrect
    int known;//indicates to what level the vocab is known, and thus to which list it belongs
    struct vocab * next;//pointer to next in list
};

struct listinfo//struct holds head, tail and the number of entries for the n2l, norm, known and old lists
{
    struct vocab * head;
    int entries;
    struct vocab * tail;
};

extern FILE * inputfile;
extern struct listinfo n2l, norm, known, old;

void importdatabase(QString fileToImport);/*performs the functions of the old loaddatabase(),
passing correct parameters to getrecordsfromfile(), which reads the file into
vocab structs in memory.
It then converts these vocab structs into flashcards in a cardpack and unloads
the vocab structs from memory.*/
int getrecordsfromfile(char * inputfilename,char separator);//load a file into memory
char * readtextfromfile(int maxchars,char separator);//get text field from file
int readnumberfromfile(int maxvalue,char separator);//get integer field from file
struct vocab * addtolist(struct vocab * newentry, struct listinfo * list);//add given (already filled in) vocab record to given list
int removefromlist(struct vocab * entry, struct listinfo * list,int freeup);//remove given entry from given list. Also destroy record if freeup is true
void reindex (struct listinfo * list);//necessary to stop gaps in the numbering system, which could cause random vocab selection to fail
int unloaddatabase();//clears all vocab structs from memory
int loadvocabintoflashcards();//this (and importdatabase) is new code

#endif // SVFILEOPS_H
