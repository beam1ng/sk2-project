#include "categoryselector.h"
#include "datastructures.h"
#include "ui_categoryselector.h"

CategorySelector::CategorySelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategorySelector)
{
    ui->setupUi(this);
    connect(ui->selectPb,&QPushButton::clicked,this,&CategorySelector::selectCategory);
}

CategorySelector::~CategorySelector()
{
    delete ui;
}

void CategorySelector::updateCategories(std::string categories[]){
    for(int i = 0;i<8;i++){
        std::string currentCategory = categories[i];
        if(currentCategory=="")return;
        ui->categoriesLw->addItem(QString::fromStdString(currentCategory));
    }
    ui->categoriesLw->setSelectionMode(QListWidget::SelectionMode::SingleSelection);
}

void CategorySelector::selectCategory(){
    int categoryIndex = ui->categoriesLw->indexFromItem(ui->categoriesLw->selectedItems().first()).row();

    DataStructures::chooseQuizCategoryRequest req{};
    req.categoryIndex = categoryIndex;

    sendStruct(reinterpret_cast<char*>(&req),sizeof(req));
}

void CategorySelector::sendStruct(char* data, int size){//unsafe, can block
    int sentSize = 0;
    //QDataStream stream(sock);
    while(sentSize<size){
        //sentSize+= stream.writeRawData(data+sentSize,size-sentSize);
        sentSize+= sock->write(data+sentSize,size-sentSize);
    }
}


//so i added a lot of things following category selection but havent tested it yet
