#include "editorwindow.h"
#include "ui_editorwindow.h"

editorWindow::editorWindow(QWidget *parent, flashCard * card) :
    QDialog(parent),
    ui(new Ui::editorWindow)
{
    ui->setupUi(this);

    currentCard = card;

    ui->questionText->setText(currentCard->getQuestion());
    ui->answerText->setText(currentCard->getAnswer());
    ui->infoText->setText(currentCard->getInfo());
    ui->hintText->setText(currentCard->getHint());
    /* FISH! TODO
    ui->levelText->setText(createLevelText());
    ui->streakText->setText(createStreakText());*/
    //FISH! TODO enable/disable buttons according to level
}

editorWindow::~editorWindow()
{
    delete ui;
}
