#include "fmlhandler.h"
#include "popupwindow.h"
#include "cardpack.h"

FMLHandler::FMLHandler()
{
}

bool FMLHandler::startDocument()
{
    inFlashcardsSection = inFlashcard = false;
    currentCardProperty = "none";
    return true;
}

bool FMLHandler::startElement( const QString &, const QString & name, const QString &, const QXmlAttributes &)
{
    if (name == "flashcards")
    {
        inFlashcardsSection = true;
    }
    else if (name == "flashcard")
    {
        currentCard = new flashCard;
        inFlashcard = true;
    }
    else if (name == "Question" ||
             name == "Answer" ||
             name == "Info" ||
             name == "Hint" ||
             name == "KnownLevel" ||
             name == "LastCorrect" ||
             name == "CurrentStreak" ||
             name == "LevelUp")
    {
        currentCardProperty = name;
    }
    else
        popup.error(0,name);

    return true;
}

bool FMLHandler::characters ( const QString & text )
{
    if (inFlashcardsSection && inFlashcard && currentCardProperty != "none")
    {
        if (currentCardProperty == "Question")
            currentCard->setQuestion(text);
        else if (currentCardProperty == "Answer")
            currentCard->setAnswer(text);
        else if (currentCardProperty == "Info")
            currentCard->setInfo(text);
        else if (currentCardProperty == "Hint")
            currentCard->setHint(text);
        else if (currentCardProperty == "KnownLevel")
            currentCard->knownLevel = (knownLevel_t)text.toInt();
        else if (currentCardProperty == "LastCorrect")
            currentCard->lastCorrect = (text.toInt() == 0) ? false : true;
        else if (currentCardProperty == "CurrentStreak")
            currentCard->currentStreak = text.toInt();
        else if (currentCardProperty == "LevelUp")
            currentCard->levelUp = text.toInt();
    }

    return true;
}

bool FMLHandler::endElement( const QString & , const QString & name, const QString &)
{
    if (name == "flashcards")
    {
        inFlashcardsSection = false;
    }
    else if (name == "flashcard")
    {
        mainPack.addCard(*currentCard,currentCard->knownLevel);
        currentCard = NULL;
        inFlashcard = false;
    }
    else if (name == "Question" ||
             name == "Answer" ||
             name == "Info" ||
             name == "Hint" ||
             name == "KnownLevel" ||
             name == "LastCorrect" ||
             name == "CurrentStreak" ||
             name == "LevelUp")
    {
        currentCardProperty = "none";
    }
    else
        popup.error(0,name);

    return true;
}
