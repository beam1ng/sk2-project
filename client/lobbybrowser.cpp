#include "lobbybrowser.h"
#include "datastructures.h"
#include "ui_lobbybrowser.h"

LobbyBrowser::LobbyBrowser(QWidget *parent) :
    QDialog(parent), ui(new Ui::LobbyBrowser)
{
    ui->setupUi(this);

    connect(ui->CreateLobbyPb,&QPushButton::clicked,this,&LobbyBrowser::CreateLobby);
    connect(ui->JoinLobbyPb,&QPushButton::clicked,this,&LobbyBrowser::JoinLobby);
}

LobbyBrowser::~LobbyBrowser()
{
    delete ui;
}

void LobbyBrowser::CreateLobby(){
    DataStructures::createLobbyRequest c;
    QDataStream stream(sock);
    stream.writeRawData(reinterpret_cast<const char*>(&c),sizeof(c));
}

void LobbyBrowser::JoinLobby(){
    DataStructures::joinLobbyRequest j;
    auto lobbyCode = ui->lobbyCodeLineEdit->text().toLatin1().data();
    qstrncpy(j.lobbyCode,lobbyCode,sizeof(j.lobbyCode));
    QDataStream stream(sock);
    stream.writeRawData(reinterpret_cast<const char*>(&j),sizeof(j));
}
