#ifndef LOBBYBROWSER_H
#define LOBBYBROWSER_H

#include "qtcpsocket.h"
#include <QDialog>

namespace Ui {
class LobbyBrowser;
}

class LobbyBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit LobbyBrowser(QWidget *parent = nullptr);
    QTcpSocket * sock;
    ~LobbyBrowser();

private:
    void CreateLobby();
    void JoinLobby();
    Ui::LobbyBrowser *ui;
};

#endif // LOBBYBROWSER_H
