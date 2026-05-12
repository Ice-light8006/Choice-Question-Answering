#include"MainWindow.h"
#include"DatabaseManager.h"
#include"QuestionsManager.h"
#include"JsonManager.h"
#include"QPackManager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置日志格式
    qSetMessagePattern(
        "[%{time hh:mm:ss}] "
        "[%{type}] "
        "[%{file}:%{line}] "
        "[%{function}] "
        "%{message}"
        );

    //实例化数据库管理器
    DatabaseManager* DbM;
    DbM = new DatabaseManager();

    //实例化题目管理器
    QuestionsManager* QuMag;
    QuMag = new QuestionsManager(DbM);

    //实例化QPack管理模块
    QPackManager* qpMag;
    qpMag = new QPackManager();

    //实例化JSON模块
    JsonManager* JsonMag;
    JsonMag = new JsonManager();

    MainWidget* w = new MainWidget(JsonMag,DbM,QuMag,qpMag);
    w->show();
    return a.exec();
}
