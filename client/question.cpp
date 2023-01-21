#include "question.h"
#include "ui_question.h"

Question::Question(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Question)
{
    ui->setupUi(this);
    connect(ui->aPb,&QPushButton::clicked,this,[this](){answerQuestion('a');});
    connect(ui->bPb,&QPushButton::clicked,this,[this](){answerQuestion('b');});
    connect(ui->cPb,&QPushButton::clicked,this,[this](){answerQuestion('c');});
    connect(ui->dPb,&QPushButton::clicked,this,[this](){answerQuestion('d');});

}

Question::~Question()
{
    delete ui;
}

void Question::updateQuestion(DataStructures::clientQaBundle qaBundle)
{
    Question::questionIndex++;
    ui->questionTb->setText(QString::fromStdString(qaBundle.question));
    ui->aPb->setText(QString::fromStdString(qaBundle.answerA));
    ui->bPb->setText(QString::fromStdString(qaBundle.answerB));
    ui->cPb->setText(QString::fromStdString(qaBundle.answerC));
    ui->dPb->setText(QString::fromStdString(qaBundle.answerD));
    enableButtons();
}

void Question::answerQuestion(char answer){
    DataStructures::answerQuestionRequest req{};
    req.questionIndex=Question::questionIndex;

    switch (answer){
        case 'a':{
        req.answerIndex=0;
        break;
    }
    case 'b':{
        req.answerIndex=1;
        break;
    }
    case 'c':{
        req.answerIndex=2;
        break;
    }
    case 'd':{
        req.answerIndex=3;
        break;
    }
    default:{
        return;
    }
    };

    disableButtons();

    char* data = reinterpret_cast<char*>(&req);
    int size = sizeof(req);

    sendStruct(data,size);
}

void Question::sendStruct(char* data, int size){
    int sentSize = 0;
    while(sentSize<size){
        sentSize+= sock->write(data+sentSize,size-sentSize);
    }
}

void Question::disableButtons(){
    ui->aPb->setDisabled(true);
    ui->bPb->setDisabled(true);
    ui->cPb->setDisabled(true);
    ui->dPb->setDisabled(true);
}

void Question::enableButtons(){
    ui->aPb->setDisabled(false);
    ui->bPb->setDisabled(false);
    ui->cPb->setDisabled(false);
    ui->dPb->setDisabled(false);
}
