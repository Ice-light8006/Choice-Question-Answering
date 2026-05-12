#include"MenuBar.h"
#include<QApplication>
MenuBar::MenuBar(QWidget* parent)
    :QMenuBar(parent)
{
    FileMenu = this->addMenu("文件(&F)");
    //设置MenuBar
    this->setFixedHeight(25);

    //设置菜单栏的样式
    qApp->setStyleSheet(
        // 1. 菜单栏整体样式
        "QMenuBar {"
        //"   background-color: #f0f0f0;"      // 背景色（可选）
        "   font-size: 12px;"                 // 菜单栏字体大小
        "}"
        // 2. 菜单栏上的项
        "QMenuBar::item {"
        "   padding: 2px 6px;"                // 菜单栏项的上下左右内边距
        "   background-color: transparent;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #c0c0c0;"       // 鼠标悬停时的背景
        "}"
        // 3. 下拉菜单整体
        "QMenu {"
        "   font-size: 12px;"                  // 菜单字体（统一继承）
        //"   border-radius: 0px;"
        "}"
        // 4. 下拉菜单的项
        "QMenu::item {"
        "   padding: 2px 8px;"                 // 内边距：上下2px，左右15px（紧凑）
        "   border-radius: 0px;"         //圆角设置为0得到直角
        "}"
        // 5. 菜单项被选中的样式（可选）
        "QMenu::item:selected {"
        "   background-color: #e0e0e0;"
        "}"
        );

    //导出子菜单
    QAction* exportAction = new QAction("导出题库");
    this->FileMenu->addAction(exportAction);

    //导入子菜单
    QAction* importAction = new QAction("导入题库");
    this->FileMenu->addAction(importAction);

    //连接信号与信号
    connect(exportAction,&QAction::triggered,this,&MenuBar::exportActionTrigger);
    connect(importAction,&QAction::triggered,this,&MenuBar::importQuestionsRequested);

    /*
    //打开本地文件的QAction
    QAction *OpenLocalQuestionFile = new QAction("打开本地题库文件");
    this->FileMenu->addAction(OpenLocalQuestionFile);
    connect(OpenLocalQuestionFile,&QAction::triggered,this,&MainWidget::OpenLocalQuestionJsonFile);
*/

    /*
    //刷新本地题库的QAction
    QAction *RefreshLocalQuestionsBank = new QAction("刷新题库");
    this->FileMenu->addAction(RefreshLocalQuestionsBank);
    connect(RefreshLocalQuestionsBank,&QAction::triggered,this,&MainWidget::RefreshLocalQuestionsBank);
*/
}

MenuBar::~MenuBar()
{

}
