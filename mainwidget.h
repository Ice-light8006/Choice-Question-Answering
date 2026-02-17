#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_Answer_pushButton_clicked();

private:
    QStackedWidget *qsw;
    QWidget *page1;
    QWidget *page2;
    QWidget *page3;
    Ui::Widget *ui;
};
#endif // MAINWIDGET_H
