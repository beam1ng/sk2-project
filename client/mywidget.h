#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "nicknameselector.h"
#include "lobbybrowser.h"
#include "categoryselector.h"
#include "gamesummary.h"
#include "lobbyview.h"
#include "question.h"
#include <QWidget>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    NicknameSelector *ns = new NicknameSelector();
    LobbyBrowser *lb = new LobbyBrowser();
    CategorySelector *cs = new CategorySelector();
    GameSummary *gs = new GameSummary();
    LobbyView *lv = new LobbyView();
    Question *q = new Question();


protected:
    QTcpSocket * sock {nullptr};
    QTimer * connTimeoutTimer{nullptr};
    void connectBtnHit();
    void socketConnected();
    void socketDisconnected();
    void socketError(QTcpSocket::SocketError);
    void socketReadable();
    void sendBtnHit();

private:
    Ui::MyWidget * ui;

    void readStruct(char *buffer, int size);
    void sendStruct(char *data, int size);
};

#endif // MYWIDGET_H
