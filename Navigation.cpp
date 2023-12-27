#include "Navigation.h"
#include "ui_Navigation.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFont>
#include <QFileDialog>
#include <QInputDialog>

#include "head.h"
#include "Graph.h"

int op;

Navigation::Navigation(QWidget* parent) ://构造函数
    QMainWindow(parent),
    ui(new Ui::Navigation)
{
    ui->setupUi(this);
    G.uzi = ui;
    ui->label->setText("请选择上述操作");
    ui->label_4->setText("请选择上述操作");
    ui->label_5->setScaledContents(true);
    ui->label_7->setScaledContents(true);
    ui->label_8->setScaledContents(true);
}

Navigation::~Navigation()//析构函数
{
    delete ui;
}


void Navigation::clr()//清除右侧显示栏的显示信息
{
    ui->label->setText("请选择上述操作");
    ui->label_4->setText("请选择上述操作");
    ui->listWidget->clear();
    update();//调用paintEvent函数
}

void Navigation::on_pushButton_clicked()
{
    op = 1;
    ui->label->setText("请输入想加入的地点名称(英文):\n地点1");
}

void Navigation::on_pushButton_16_clicked()
{
    clr();
    G.option(op);
}

void Navigation::on_pushButton_15_clicked()
{
    clr();
    G.print();
}


void Navigation::on_pushButton_2_clicked()
{
    op = 2;
    ui->label->setText("请输入想加入的路径信息(英文)：\n地点1 地点2 距离");
}


void Navigation::on_pushButton_3_clicked()
{
    op = 3;
    ui->label->setText("请输入想删除的地点信息(英文)：\n地点1");
}



void Navigation::on_pushButton_4_clicked()
{
    op =  4;
    ui->label->setText("请输入想删除的路径信息(英文)：\n地点1 地点2");
}


void Navigation::on_pushButton_6_clicked()
{
    op = 0;
    clr();
    G.importFile();
}



void Navigation::on_pushButton_7_clicked()
{
    op = 5;
    ui->label->setText("请输入的地点信息(英文)：\n起点 终点");
}


void Navigation::on_pushButton_5_clicked()
{
    op = 6;
    ui->label->setText("请输入需要经过的地点个数：\n地点个数");
    ui->label_4->setText("请输入的地点信息(英文)：\n起点 终点");
}


void Navigation::on_pushButton_14_clicked()
{
    op = 8;
    ui->label->setText("请输入地点个数及地点名(英文)：\n个数n 地点1...地点n");
    ui->label_4->setText("请输入的访问限制(英文)：\n个数m 限制1...限制m");
}


void Navigation::on_pushButton_9_clicked()
{
    op = 7;
    ui->label->setText("请输入需要经过的地点个数：\n个数n");
    ui->label_4->setText("请输入的需要经过的地点(英文)：\n地点1...地点n");
}

