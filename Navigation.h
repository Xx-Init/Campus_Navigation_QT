#ifndef NAVIGATION_H
#define NAVIGATION_H
#include <cmath>
#include <QMainWindow>
#include <fstream>
#include <QVector>
#include "head.h"
#include "Graph.h"
namespace Ui {
    class Navigation;
}

class Navigation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Navigation(QWidget* parent = 0);
    bool isin(QPoint x, QPoint y, int n = 10);
    double dist(QPoint a, QPoint b);
    void clr();
    QMouseEvent* event;
    ~Navigation();
    Ui::Navigation* ui;
    Graph G;
private slots:

    void on_pushButton_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_9_clicked();

private:


};

#endif // NAVIGATION_H
