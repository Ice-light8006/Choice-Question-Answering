#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"Question.h"
#include"QPackManager.h"
#include"JsonManager.h"
#include"DatabaseManager.h"
#include"QuestionsManager.h"
#include"HomePage.h"
#include"AnswerPage.h"
#include"MenuBar.h"
#include"ImportJsonPage.h"
#include"DatabaseSelectWidget.h"
#include"StartPage.h"
#include"QuizManager.h"
#include"CurrentSizeStackedWidget.h"
#include <QWidget>
#include<QPushButton>
#include<QTextEdit>
#include<QVector>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QAbstractButton>
#include<QLabel>
#include<QLineEdit>
#include<QTableWidget>
#include<QMainWindow>
#include<QMenuBar>
#include<QMenu>
#include<QtSql>


class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(JsonManager* JsonMag,
               DatabaseManager* DbMag,
               QuestionsManager* QuMag,QPackManager* qpMag,QMainWindow *parent = nullptr);
    void MyAdjustSize();
    ~MainWidget();
private:
    //Json管理模块类
    JsonManager* JsonMag;

    //数据库管理模块类
    DatabaseManager* DbMag;

    //题目管理模块类
    QuestionsManager* QuMag;

    //QPack管理模块类
    QPackManager* qpMag;

    //答题管理类
    QuizManager* quizMag;

    //创建窗口堆栈
    CurrentSizeStackedWidget* qsw;

    //主页类
    HomePage* homepage;

    //答题页类
    AnswerPage* answerpage;

    //菜单类
    MenuBar *menubar;

    //导入窗口类
    ImportJsonPage *importjsonpage;

    //数据库选择列表类
    DatabaseSelectWidget* dbselectwidget;

    //根widget的布局
    QHBoxLayout* mainwindowlayout;

    //根widget
    QWidget* rootwidget;

    //最开始的界面
    StartPage* startpage;




private slots:
    void StartAnswer();
    void GoToImportPage();
    void ImportQuestionsFromText();
    void GoToHomePage();
    void startRandomAnswer();
    void startFullAnswer();
    void switchAnswerPage();
    void importQpack();
};
#endif // MAINWINDOW_H
