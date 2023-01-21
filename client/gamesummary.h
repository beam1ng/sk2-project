#ifndef GAMESUMMARY_H
#define GAMESUMMARY_H

#include <QWidget>

namespace Ui {
class GameSummary;
}

class GameSummary : public QWidget
{
    Q_OBJECT

public:
    explicit GameSummary(QWidget *parent = nullptr);
    ~GameSummary();

    void UpdateText(std::string text);
private:
    Ui::GameSummary *ui;
};

#endif // GAMESUMMARY_H
