#include "gamesummary.h"
#include "ui_gamesummary.h"

GameSummary::GameSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameSummary)
{
    ui->setupUi(this);
}

GameSummary::~GameSummary()
{
    delete ui;
}

void GameSummary::UpdateText(std::string text){
    ui->textEdit->setText(QString::fromStdString(text));
}
