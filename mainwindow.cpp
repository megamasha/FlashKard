#include <QString>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "svfileops.h"
#include "cardpack.h"
#include "flashcardwindow.h"
#include "popupwindow.h"
#include "databasewindow.h"
#include "statswindow.h"
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDomDocument>
#include <QTextStream>
#include "fmlhandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    enableAndDisableButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    //if no changes have been made, the program can be safely closed
    if ( !mainPack.hasUnsavedChanges() )
        event->accept();

    //otherwise ask whether user wants to save changes
    else if (popup.importantQuestion(this,tr("Save changes before quitting?")))
    {
        on_saveButton_clicked();
        event->accept();
    }
    // if user doesn't want to save changes, just quit
    else
        event->accept();
}

void MainWindow::on_exitButton_clicked()
{
    //if non-empty pack has unsaved changes that the user wants to save...
    if ( mainPack.hasUnsavedChanges() &&
         ! mainPack.isEmpty() &&
         popup.importantQuestion(this,tr("Save changes before quitting?")) )
    {
        //...then save
        on_saveButton_clicked();
    }

    exit(EXIT_SUCCESS);
}

void MainWindow::on_loadButton_clicked()
{
    //get filename to open
    currentlyLoadedFilename = QFileDialog::getOpenFileName(this,
                                            tr("Load Flashcards"),
                                            "~/Documents", tr("All FlashKard Files (*.~sv *.csv *.fml *.fdb);;FlashKard Markup Languge (*.fml);;Tilde~Separated Values (*.~sv);;FlashKard Database (*.fdb)"));

    QFileInfo fileToLoad(currentlyLoadedFilename);

    //based on file extension, call appropriate loader
    if      (fileToLoad.suffix() == "fml")
        loadFML();
    else if (fileToLoad.suffix() == "fdb")
        loadFDB();
    else if (fileToLoad.suffix() == "~sv" ||
             fileToLoad.suffix() == "csv")
        importdatabase(currentlyLoadedFilename);


    enableAndDisableButtons();
}

void MainWindow::on_testButton_clicked()
{
    if (mainPack.isEmpty())
        on_loadButton_clicked();

    while (mainPack.isEmpty())
    {
        if(popup.importantQuestion(this,tr("Nothing to test!\n\nLoad flashcards?")))
            on_loadButton_clicked();
        else
            return;
    }

    mainPack.setChanged();
    flashCardWindow tester;
    tester.exec();
}

void MainWindow::on_databaseButton_clicked()
{
    mainPack.setChanged();
    databaseWindow database;
    database.exec();
    enableAndDisableButtons();
}

void MainWindow::on_saveButton_clicked()
{
    if (mainPack.isEmpty()) return; //don't save empty file

    currentlyLoadedFilename = QFileDialog::getSaveFileName(this, tr("Save Flashcard Database"),
                                                currentlyLoadedFilename.toAscii().data(),
                                                tr("FlashKard Markup Languge (*.fml);;Tilde~Separated Values (*.~sv);;FlashKard Database (*.fdb)"));

    QFileInfo fileToSave(currentlyLoadedFilename);

    //based on file extension, call appropriate saving routine
    if      (fileToSave.suffix() == "fml")
        saveFML();
    else if (fileToSave.suffix() == "fdb")
        saveFDB();
    else if (fileToSave.suffix() == "~sv")
        mainPack.exportdatabase(currentlyLoadedFilename);

    mainPack.setUnchanged();
}

void MainWindow::enableAndDisableButtons()
{
    if (mainPack.isEmpty())
    {
        ui->actionSave_FlashKard_Database->setDisabled(true);
        ui->actionStatistics->setDisabled(true);
        ui->saveButton->setDisabled(true);
        ui->statsButton->setDisabled(true);
    }
    else
    {
        ui->actionSave_FlashKard_Database->setEnabled(true);
        ui->actionStatistics->setEnabled(true);
        ui->saveButton->setEnabled(true);
        ui->statsButton->setEnabled(true);
    }
}
void MainWindow::on_statsButton_clicked()
{
    StatsWindow statsWindow;
    statsWindow.exec();
}

void MainWindow::saveFDB()
{
    //Create connection to database file with current filename
    QSqlDatabase flashcardDatabase = QSqlDatabase::addDatabase("QSQLITE");
    flashcardDatabase.setDatabaseName(currentlyLoadedFilename);
    flashcardDatabase.open();

    //create table
    QSqlQuery query;
    query.exec("DROP TABLE Flashcards");
    query.exec("CREATE TABLE Flashcards (Question TEXT, Answer TEXT, Info TEXT, Hint TEXT, KnownLevel TINYINT, LastCorrect TINYINT, CurrentStreak TINYINT, LevelUp TINYINT)");

    //add all cards to database
    flashCard * currentCard = mainPack.getFirstCard();
    query.exec("BEGIN TRANSACTION");
    do
    {
        QString knownLevel, lastCorrect, currentStreak, levelUp;
        QString queryString;
        queryString = "INSERT INTO Flashcards (Question, Answer, Info, Hint, KnownLevel, LastCorrect, CurrentStreak, LevelUp) VALUES ('" +
                currentCard->getQuestion().replace(QString("'"),QString("''")) + "', '" +
                currentCard->getAnswer().replace(QString("'"),QString("''")) + "', '" +
                currentCard->getInfo().replace(QString("'"),QString("''")) + "', '" +
                currentCard->getHint().replace(QString("'"),QString("''")) + "', " +
                knownLevel.setNum((int)currentCard->getKnownLevel()) + ", " +
                lastCorrect.setNum(currentCard->wasCorrectLastTime() ? 1 : 0) + ", " +
                currentStreak.setNum(currentCard->getCurrentStreak()) + ", " +
                levelUp.setNum(currentCard->getLevelUp()) + ")";

        if(!query.exec(queryString.toUtf8().data()))
        {
            popup.error(this,query.lastQuery());
            popup.error(this,query.lastError().text());
        }
    }
    while ((currentCard = mainPack.getNextCard(currentCard)) != NULL);

    query.exec("END TRANSACTION");

    flashcardDatabase.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
}

void MainWindow::loadFDB()
{
    //Create connection to database file with current filename
    QSqlDatabase flashcardDatabase = QSqlDatabase::addDatabase("QSQLITE");
    flashcardDatabase.setDatabaseName(currentlyLoadedFilename);
    flashcardDatabase.open();
    QSqlQuery query;

    //read in all cards
    query.exec("SELECT Question,Answer,Info,Hint,KnownLevel,LastCorrect,CurrentStreak,LevelUp FROM Flashcards");

    QString qu;
    QString an;
    QString in;
    QString hi;
    knownLevel_t kl;
    bool lc;
    int cs;
    int lu;
    while (query.next())
    {
        qu = query.value(0).toString();
        an = query.value(1).toString();
        in = query.value(2).toString();
        hi = query.value(3).toString();
        kl = (knownLevel_t)query.value(4).toInt();
        lc = query.value(5).toInt() ? 1 : 0;
        cs = query.value(6).toInt();
        lu = query.value(7).toInt();

        flashCard * fc = new flashCard(qu,an,in,hi,kl,lc,cs,lu);

        mainPack.addCard(*fc,kl);
    }

    //close connection
    flashcardDatabase.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
}

void MainWindow::saveFML()
{
    //create XML file in memory
    QDomDocument flashKardMLDoc("FlashKardML");
    QDomElement flashcards = flashKardMLDoc.createElement("flashcards");

    //add each flashcard node to the flashcards node
    flashCard * currentCard = mainPack.getFirstCard();
    do
    {
        QDomElement flashcard = flashKardMLDoc.createElement("flashcard");

        //strings:
        QDomElement question = flashKardMLDoc.createElement("Question");
        QDomText questionText = flashKardMLDoc.createTextNode(currentCard->getQuestion());
        question.appendChild(questionText);
        flashcard.appendChild(question);
        QDomElement answer = flashKardMLDoc.createElement("Answer");
        QDomText answerText = flashKardMLDoc.createTextNode(currentCard->getAnswer());
        answer.appendChild(answerText);
        flashcard.appendChild(answer);
        QDomElement info = flashKardMLDoc.createElement("Info");
        QDomText infoText = flashKardMLDoc.createTextNode(currentCard->getInfo());
        info.appendChild(infoText);
        flashcard.appendChild(info);
        QDomElement hint = flashKardMLDoc.createElement("Hint");
        QDomText hintText = flashKardMLDoc.createTextNode(currentCard->getHint());
        hint.appendChild(hintText);
        flashcard.appendChild(hint);

        //others:
        QString kl, lc, cs, lu;

        QDomElement knownLevel = flashKardMLDoc.createElement("KnownLevel");
        QDomText knownLevelText = flashKardMLDoc.createTextNode(kl.setNum((int)currentCard->getKnownLevel()));
        knownLevel.appendChild(knownLevelText);
        flashcard.appendChild(knownLevel);
        QDomElement lastCorrect = flashKardMLDoc.createElement("LastCorrect");
        QDomText lastCorrectText = flashKardMLDoc.createTextNode(lc.setNum(currentCard->wasCorrectLastTime() ? 1 : 0));
        lastCorrect.appendChild(lastCorrectText);
        flashcard.appendChild(lastCorrect);
        QDomElement currentStreak = flashKardMLDoc.createElement("CurrentStreak");
        QDomText currentStreakText = flashKardMLDoc.createTextNode(cs.setNum(currentCard->getCurrentStreak()));
        currentStreak.appendChild(currentStreakText);
        flashcard.appendChild(currentStreak);
        QDomElement levelUp = flashKardMLDoc.createElement("LevelUp");
        QDomText levelUpText = flashKardMLDoc.createTextNode(lu.setNum(currentCard->getLevelUp()));
        levelUp.appendChild(levelUpText);
        flashcard.appendChild(levelUp);

        flashcards.appendChild(flashcard);
    }
    while ((currentCard = mainPack.getNextCard(currentCard)) != NULL);

    //add the whole flashcards node to the file in memory
    flashKardMLDoc.appendChild(flashcards);

    //write the file to disk
    QFile outputFile (currentlyLoadedFilename);
    if(!outputFile.open(QIODevice::WriteOnly))
        popup.error(this,tr("Could not open file for writing!"));
    QTextStream outputStream(&outputFile);
    outputStream << flashKardMLDoc.toString();

    outputFile.close();
}

void MainWindow::loadFML()
{
    //use FMLHandler to parse the FML file element by element
    FMLHandler handler;
    QFile inputFile (currentlyLoadedFilename);
    QXmlInputSource fmlSource (&inputFile);

    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);

    reader.parse(fmlSource);
}
