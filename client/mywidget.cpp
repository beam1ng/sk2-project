#include "mywidget.h"
#include "nicknameselector.h"
#include "ui_mywidget.h"
#include "datastructures.h"
#include <QMessageBox>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);
    connect(ui->conectBtn, &QPushButton::clicked, this, &MyWidget::connectBtnHit);
    connect(ui->hostLineEdit, &QLineEdit::returnPressed, ui->conectBtn, &QPushButton::click);
}

MyWidget::~MyWidget() {
    if(sock)
        sock->close();
    delete ui;
}

void MyWidget::connectBtnHit(){
    ui->connectGroup->setEnabled(false);
    if(sock)
        delete sock;
    sock = new QTcpSocket(this);
    connTimeoutTimer = new QTimer(this);
    connTimeoutTimer->setSingleShot(true);
    connect(connTimeoutTimer, &QTimer::timeout, [&]{
        sock->abort();
        sock->deleteLater();
        sock = nullptr;
        connTimeoutTimer->deleteLater();
        connTimeoutTimer=nullptr;
        ui->connectGroup->setEnabled(true);
        QMessageBox::critical(this, "Error", "Connect timed out");
    });

    connect(sock, &QTcpSocket::connected, this, &MyWidget::socketConnected);
    connect(sock, &QTcpSocket::disconnected, this, &MyWidget::socketDisconnected);
    connect(sock, &QTcpSocket::errorOccurred, this, &MyWidget::socketError);
    connect(sock, &QTcpSocket::readyRead, this, &MyWidget::socketReadable);

    sock->connectToHost(ui->hostLineEdit->text(), ui->portSpinBox->value());
    connTimeoutTimer->start(3000);
}

void MyWidget::socketConnected(){
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    connTimeoutTimer=nullptr;
    this->hide();
    ns->sock = sock;
    lb->sock = sock;
    lv->sock = sock;
    q->sock = sock;
    cs->sock = sock;
    ns->show();
}

void MyWidget::socketDisconnected(){
    ns->hide();
    lb->hide();
    lv->hide();
    cs->hide();
    gs->hide();
    q->hide();
    this->show();
    ui->connectGroup->setEnabled(true);
}

void MyWidget::socketError(QTcpSocket::SocketError err){
    if(err == QTcpSocket::RemoteHostClosedError)
        return;
    if(connTimeoutTimer){
        connTimeoutTimer->stop();
        connTimeoutTimer->deleteLater();
        connTimeoutTimer=nullptr;
    }
    QMessageBox::critical(this, "Error", sock->errorString());
    ui->connectGroup->setEnabled(true);
}

void MyWidget::socketReadable(){
    while(sock->bytesAvailable()){

        //QDataStream stream(sock);
        DataStructures::serverCommands serverCommand;

        //stream.readRawData(reinterpret_cast<char*>(&serverCommand),sizeof(serverCommand));
        readStruct(reinterpret_cast<char*>(&serverCommand),sizeof(serverCommand));

        switch(serverCommand){
        case DataStructures::answerNicknameProposal:{
            DataStructures::answerNicknameProposalStruct req{};
            //stream.readRawData(reinterpret_cast<char*>(&req)+sizeof(serverCommand),sizeof(req)-sizeof(serverCommand));
            readStruct(reinterpret_cast<char*>(&req)+sizeof(serverCommand),sizeof(req)-sizeof(serverCommand));
            if(req.isApproved){
                ns->hide();
                lb->show();
            }else{
                ns->displayNicknameAlreadyExist();
            }
            break;
        }
        case DataStructures::quizCategories:{
            DataStructures::quizCategoriesStruct req{};
            readStruct(reinterpret_cast<char*>(&req)+sizeof(serverCommand),sizeof(req)-sizeof(serverCommand));


            //show categoryselector
            cs->show();

            //just update cs, and then sometime cs will send data to server with chosen categoryindex
            std::string categories[8]={""};
            for(int i = 0;i<8;i++){
                categories[i]=std::string(req.categoryNames[i]);
            }
            cs->updateCategories(categories);
            break;
        }
        case DataStructures::chooseQuizCategoryResult:{
            DataStructures::chooseQuizCategoryResultStruct req{};
            readStruct(reinterpret_cast<char*>(&req)+sizeof(serverCommand),sizeof(req)-sizeof(serverCommand));

            //show lv
            cs->hide();
            lv->show();

            break;
        }
        case DataStructures::joinLobbyResult:{
            DataStructures::joinLobbyResultStruct req{};
            //stream.readRawData(reinterpret_cast<char*>(&req)+sizeof(serverCommand),sizeof(req)-sizeof(serverCommand));
            readStruct(reinterpret_cast<char*>(&req)+sizeof(serverCommand),sizeof(req)-sizeof(serverCommand));
            if(req.isSuccess){
                lb->hide();
                lv->show();
                lv->updateLobbyId(req.lobbyId);
                DataStructures::getLobbyDetailsStruct req{};
                sendStruct(reinterpret_cast<char*>(&req),sizeof(req));
            }
            break;
        }
        case DataStructures::lobbyDetails:{
            DataStructures::lobbyDetailsStruct ld{};
            //stream.readRawData(reinterpret_cast<char*>(&ld)+sizeof(serverCommand),sizeof(ld)-sizeof(serverCommand));
            readStruct(reinterpret_cast<char*>(&ld)+sizeof(serverCommand),sizeof(ld)-sizeof(serverCommand));
            lv->clearPlayers();
            if(ns->isTheSameNickname(std::string(ld.hostNickname))){
                lv->setHostPresets();
            }
            for(int i = 0;i<4;i++){
                QString currentNickname = QString(ld.nicknames[i]);
                if(currentNickname!=""){
                    lv->addPlayer(currentNickname);
                }
            }
            break;
        }
        case DataStructures::gameQuestion:{
            DataStructures::gameQuestionStruct gq{};

            readStruct(reinterpret_cast<char*>(&gq)+sizeof(serverCommand),sizeof(gq)-sizeof(serverCommand));
            //if hidden, hide current and show questionUI
            if(q->isHidden()){
                lv->hide();
                q->show();
            }

            DataStructures::clientQaBundle qaBundle;
            qaBundle.question = gq.question;
            qaBundle.answerA = gq.answers[0];
            qaBundle.answerB = gq.answers[1];
            qaBundle.answerC = gq.answers[2];
            qaBundle.answerD = gq.answers[3];

            q->updateQuestion(qaBundle);

            break;
        }
        case DataStructures::gameSummary:{
            DataStructures::gameSummaryStruct gs{};
            readStruct(reinterpret_cast<char*>(&gs)+sizeof(serverCommand),sizeof(gs)-sizeof(serverCommand));
            q->hide();
            MyWidget::gs->show();
            MyWidget::gs->UpdateText(std::string(gs.summaryString));

            break;
        }
        case DataStructures::gameClosedByHost:{
            DataStructures::gameClosedByHostStruct gcbh{};
            readStruct(reinterpret_cast<char*>(&gcbh)+sizeof(serverCommand),sizeof(gcbh)-sizeof(serverCommand));
            q->hide();
            lb->show();
            break;
        }
        default:{
            break;
        }};
    }
}

void MyWidget::sendBtnHit(){
    //auto txt = ui->msgLineEdit->text().trimmed();
    //if(txt.isEmpty())
    //    return;
    //sock->write((txt+'\n').toUtf8());
}

void MyWidget::readStruct(char* buffer,int size){//unsafe, can block
    int redSize = 0;
    //QDataStream stream(sock);
    while(redSize<size){
        //redSize+= stream.readRawData(buffer,size-redSize);
        redSize+= sock->read(buffer,size-redSize);
    }
}

void MyWidget::sendStruct(char* data, int size){//unsafe, can block
    int sentSize = 0;
    //QDataStream stream(sock);
    while(sentSize<size){
        //sentSize+= stream.writeRawData(data+sentSize,size-sentSize);
        sentSize+= sock->write(data+sentSize,size-sentSize);
    }
}
