#ifndef LOBBYVIEW_H
#define LOBBYVIEW_H

#include "qtcpsocket.h"
#include <QWidget>

namespace Ui {
class LobbyView;
}

class LobbyView : public QWidget
{
    Q_OBJECT

public:
    explicit LobbyView(QWidget *parent = nullptr);
    ~LobbyView();
    QTcpSocket * sock;
    void clearPlayers();
    void addPlayer(QString nickname);

    void setHostPresets();
    void updateLobbyId(int newLobbyId);
private:
    Ui::LobbyView *ui;
    void sendStruct(char *data, int size);
    void startGame();
};

#endif // LOBBYVIEW_H
