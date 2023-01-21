#ifndef NICKNAMESELECTOR_H
#define NICKNAMESELECTOR_H

#include "qtcpsocket.h"
#include <QWidget>

namespace Ui {
class NicknameSelector;
}

class NicknameSelector : public QWidget
{
    Q_OBJECT

public:
    explicit NicknameSelector(QWidget *parent = nullptr);
    void displayNicknameAlreadyExist();
    QTcpSocket * sock;
    ~NicknameSelector();

    bool isTheSameNickname(std::string nickname);
private:
    Ui::NicknameSelector *ui;
    std::string nickname;
    void okButtonHit();
    void sendNicknameProposal();
    void sendStruct(char *data, int size);
};

#endif // NICKNAMESELECTOR_H
