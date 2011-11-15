/* Most of this code is taken from or based on the c source file for the
original Vocab tester Version 1 (http://github.com/megamasha/Vocab-Tester)
*/

#ifndef SVFILEOPS_H
#define SVFILEOPS_H

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

#endif // SVFILEOPS_H
