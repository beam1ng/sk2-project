#include "nicknameselector.h"
#include "ui_nicknameselector.h"
#include "datastructures.h"

NicknameSelector::NicknameSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NicknameSelector)
{
    ui->setupUi(this);
    ui->nicknameExistsLabel->hide();
    connect(ui->okButton,&QPushButton::clicked,this,&NicknameSelector::okButtonHit);
}

NicknameSelector::~NicknameSelector()
{
    delete ui;
}

void NicknameSelector::okButtonHit(){
    sendNicknameProposal();
}

void NicknameSelector::sendNicknameProposal(){
    DataStructures::proposeNicknameRequest pn;
    NicknameSelector::nickname = ui->nicknameLineEdit->text().toStdString();
    strncpy(pn.nickname,NicknameSelector::nickname.c_str(),32);

    sendStruct(reinterpret_cast<char*>(&pn),sizeof(pn));
}

void NicknameSelector::displayNicknameAlreadyExist(){
    ui->nicknameExistsLabel->show();
}

void NicknameSelector::sendStruct(char* data, int size){
    int sentSize = 0;
    QDataStream stream(sock);
    while(sentSize<size){
        sentSize+= stream.writeRawData(data+sentSize,size-sentSize);
    }
}

bool NicknameSelector::isTheSameNickname(std::string nickname){
    return NicknameSelector::nickname == nickname;
}
