#ifndef QUESTION_H
#define QUESTION_H

#include "datastructures.h"
#include "qtcpsocket.h"

#include <QWidget>

namespace Ui {
class Question;
}

class Question : public QWidget
{
    Q_OBJECT

public:
    explicit Question(QWidget *parent = nullptr);
    ~Question();
    void updateQuestion(DataStructures::clientQaBundle qaBundle);
    QTcpSocket * sock;

    int questionIndex = -1;

private:
    Ui::Question *ui;
    void answerQuestion(char answer);
    void sendStruct(char *data, int size);
    void disableButtons();
    void enableButtons();
};

#endif // QUESTION_H
