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
    int categoryIndex = ui->categoriesLw->row(ui->categoriesLw->selectedItems().first());

    DataStructures::chooseQuizCategoryRequest req{};
    req.categoryIndex = categoryIndex;

    sendStruct(reinterpret_cast<char*>(&req),sizeof(req));
}

void CategorySelector::sendStruct(char* data, int size){
    int sentSize = 0;
    while(sentSize<size){
        sentSize+= sock->write(data+sentSize,size-sentSize);
    }
}
