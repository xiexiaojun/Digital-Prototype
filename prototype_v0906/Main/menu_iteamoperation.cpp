#include "menu_iteamoperation.h"
#include "ui_menu_iteamoperation.h"
#include <QDebug>
Menu_iteamOperation::Menu_iteamOperation(QWidget *parent) :
    QMenu(parent),
    ui(new Ui::Menu_iteamOperation)
{
    ui->setupUi(this);
    myAddAction();
}

Menu_iteamOperation::~Menu_iteamOperation()
{
    delete ui;
}

void Menu_iteamOperation::myAddAction(){

    QAction *del_action =new QAction("删除");
    del_action->setIcon(QIcon(":/img/delete.png"));
    this->addAction(del_action);
    connect(del_action , &QAction::triggered,[=](){
          itemOperate(del);
    });

    QAction *property_action =new QAction("属性");
    property_action->setIcon(QIcon(":/img/property.png"));
    this->addAction(property_action);
    connect(property_action , &QAction::triggered,[=](){
          itemOperate(property);
    });

    QAction *edit_action =new QAction("编辑");
    edit_action->setIcon(QIcon(":/img/editComp.png"));
    this->addAction(edit_action);
    connect(edit_action , &QAction::triggered,[=](){
          itemOperate(edit);
    });

    QAction *ppi_action =new QAction("PPI显示");
    ppi_action->setIcon(QIcon(":/img/windowsicon.png"));
    this->addAction(ppi_action);
    connect(ppi_action , &QAction::triggered,[=](){
          itemOperate(ppi);
    });

    QAction *ashow_action =new QAction("A显");
    ashow_action->setIcon(QIcon(":/img/windowsicon.png"));
    this->addAction(ashow_action);
    connect(ashow_action , &QAction::triggered,[=](){
          itemOperate(ashow);
    });
}

