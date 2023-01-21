#ifndef CATEGORYSELECTOR_H
#define CATEGORYSELECTOR_H

#include <QWidget>
#include <qtcpsocket.h>

namespace Ui {
class CategorySelector;
}

class CategorySelector : public QWidget
{
    Q_OBJECT

public:
    explicit CategorySelector(QWidget *parent = nullptr);
    ~CategorySelector();
    QTcpSocket * sock;
    void updateCategories(std::string categories[]);
private:
    Ui::CategorySelector *ui;
    void selectCategory();
    void sendStruct(char *data, int size);
};

#endif // CATEGORYSELECTOR_H
