#include "lobbyview.h"
#include "ui_lobbyview.h"
#include "datastructures.h"

LobbyView::LobbyView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LobbyView)
{
    ui->setupUi(this);
    connect(ui->startPb,&QPushButton::clicked,this,&LobbyView::startGame);
    ui->startPb->setDisabled(true);
}

LobbyView::~LobbyView()
{
    delete ui;
}

void LobbyView::clearPlayers(){
    ui->playersLw->clear();
}

void LobbyView::addPlayer(QString nickname){
    ui->playersLw->addItem(nickname);
}

void LobbyView::startGame(){
    DataStructures::startGameRequest req{};
    sendStruct(reinterpret_cast<char*>(&req),sizeof(req));
}

void LobbyView::sendStruct(char* data, int size){
    int sentSize = 0;
    while(sentSize<size){
        sentSize+= sock->write(data+sentSize,size-sentSize);
    }
}

void LobbyView::setHostPresets(){
    ui->startPb->setDisabled(false);
}

void LobbyView::updateLobbyId(int newLobbyId){
    ui->lobbyIdLabel->setText(QString::number(newLobbyId));
}
