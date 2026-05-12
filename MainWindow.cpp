#include"DatabaseManager.h"
#include"MainWindow.h"
#include"Question.h"
#include"JsonManager.h"
#include"HomePage.h"
#include"AnswerPage.h"
#include"MyUtils.h"
#include<qpushbutton.h>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QRadioButton>
#include<QTextEdit>
#include<QString>
#include<QJsonDocument>
#include<QDebug>
#include<QJsonArray>
#include<QMessageBox>
#include<QJsonObject>
#include<QLabel>
#include<QCheckBox>
#include<QLayoutItem>
#include<QRandomGenerator>
#include<QFile>
#include<QCoreApplication>
#include<QTableWidget>
#include<QHeaderView>
#include<QDir>
#include<QFileDialog>
#include<QProgressDialog>
#include<QInputDialog>
#include<QPixmap>
#include<QMainWindow>
#include<QApplication>
#include<QDesktopServices>
#include<QUrl>
#include<QSql>
#include<QSqlDatabase>


#define PARSE_FAIL_MESSENAGE "JSON文本解析失败"
#define ERROR_MESSENAGE "错误"
#define FILENAME "QuestionsData.json"
#define IMAGEPATH "image"
#define LOCALQUESTIONFILE "QuestionsData.json"


MainWidget::MainWidget(JsonManager* JsonMag,DatabaseManager* DbMag,QuestionsManager* QuMag,QPackManager* qpMag,QMainWindow *parent)
    : QMainWindow(parent),JsonMag(JsonMag),DbMag(DbMag),QuMag(QuMag),qpMag(qpMag)
{
    //设置为用户无法修改窗口大小，只能通过代码调节窗口大小
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->quizMag = new QuizManager(QuMag);

    //初始化主页
    this->homepage = new HomePage(this);

    //初始化答题页
    this->answerpage = new AnswerPage(quizMag,this);

    //初始化导入Json页
    this->importjsonpage = new ImportJsonPage(this);

    //初始化菜单栏
    this->menubar = new MenuBar(this);

    //设置菜单栏
    this->setMenuBar(menubar);

    //初始化根widget
    this->rootwidget = new QWidget(this);

    //初始化选择数据库的列表
    this->dbselectwidget = new DatabaseSelectWidget(DbMag,QuMag,rootwidget);

    //初始化开始页面
    this->startpage = new StartPage(this);

    //将根widget设为中心控件
    setCentralWidget(rootwidget);

    //初始化根布局
    this->mainwindowlayout = new QHBoxLayout(rootwidget);

    //把根widget的布局设置为根布局
    rootwidget->setLayout(mainwindowlayout);

    //初始化qsw
    qsw = new CurrentSizeStackedWidget(rootwidget);

    qsw->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //将开始页、主页、答题页加入页面堆栈
    qsw->addWidget(startpage);
    qsw->addWidget(homepage);
    qsw->addWidget(answerpage);

    //设置初始页面为开始页面
    qsw->setCurrentWidget(startpage);

    mainwindowlayout->addWidget(dbselectwidget);
    mainwindowlayout->addWidget(qsw,1);

    //连接信号与槽
    connect(homepage,&HomePage::Start_Answer_Button_Clicked,this,&MainWidget::startFullAnswer);
    connect(homepage,&HomePage::Random_Answer_Button_Clicked,this,&MainWidget::startRandomAnswer);
    connect(homepage,&HomePage::importQuestionFromTextButtonClicked,this,&MainWidget::GoToImportPage);
    connect(importjsonpage,&ImportJsonPage::Import_Question_Button_Clicked,this,&MainWidget::ImportQuestionsFromText);
    connect(answerpage,&AnswerPage::Return_Home_Button_Clicked,this,&MainWidget::GoToHomePage);
    connect(dbselectwidget,&DatabaseSelectWidget::DatabaseJustSwitched,this,&MainWidget::GoToHomePage);
    connect(answerpage,&AnswerPage::InformMainWindowAdjustSize,this,&MainWidget::MyAdjustSize);
    connect(dbselectwidget,&DatabaseSelectWidget::doDeleteAnswerPage,this,&MainWidget::switchAnswerPage);
    connect(dbselectwidget,&DatabaseSelectWidget::doDeleteDatabaseAndFile,this->DbMag,&DatabaseManager::deleteCurDatabase);
    connect(dbselectwidget,&DatabaseSelectWidget::exportDatabaseRequested,this->qpMag,&QPackManager::packQuestionStore);
    connect(menubar,&MenuBar::importQuestionsRequested,this,&MainWidget::importQpack);
}

void MainWidget::switchAnswerPage()
{
    if(!answerpage)return;
    qsw->setCurrentWidget(startpage);
}

void MainWidget::importQpack()
{
    QString qpackPath = QFileDialog::getOpenFileName(
        this,
        "选择题库",
        QDir::homePath(),
        "QPack Files (*.qpack)"
        );
    qDebug() << "获取选择的QPack文件路径成功";
    QString baseName = this->qpMag->UnpackQuestionStore(qpackPath);
    this->DbMag->addDatabaseFromName(baseName);
    this->dbselectwidget->addItemForListWidget(baseName);
}

void MainWidget::MyAdjustSize()
{
    if (isMaximized() || isFullScreen()) return;

    QWidget* current = qsw->currentWidget();
    if (current) {
        current->updateGeometry();
    }
    qsw->updateGeometry();
    updateGeometry();
    adjustSize();   // 或 resize(sizeHint());
}


void MainWidget::StartAnswer()
{
    quizMag->reviseCurQuestion(quizMag->getQuestionFromWaitQuestion(0));
    quizMag->reviseCurIndex(0);
    answerpage->InitializeTableWidget(quizMag->getWaitQuestionCount());
    answerpage->BuildQuestion(quizMag->getCurQuestion());
    this->qsw->setCurrentWidget(this->answerpage);
    this->MyAdjustSize();
}

void MainWidget::GoToImportPage()
{
    importjsonpage->exec();
}

void MainWidget::ImportQuestionsFromText()
{
    QString JsonText = importjsonpage->GetImputText();
    QVector<Question*> Questions = JsonMag->GetQuestionsVectorFromJsonText(JsonText);
    if(Questions.isEmpty())
    {
        QMessageBox::critical(this,"警告","Json解析失败，请检查你的Json格式是否正确");
        return;
    }
    for(int i = 0;i<Questions.size();i++)
    {
        QuMag->AddQuestion(Questions[i]);
        DbMag->InsertQuestionToDatabase(Questions[i]);
    }
    if(answerpage)
    {
        delete this->answerpage;
        this->answerpage = new AnswerPage(quizMag,this);
    }
    qsw->addWidget(answerpage);
    connect(answerpage,&AnswerPage::Return_Home_Button_Clicked,this,&MainWidget::GoToHomePage);
    QMessageBox::information(this,"提示","导入题目成功！");
    importjsonpage->hide();
    homepage->setQuestionCnt(QuMag->GetSumQuestionCount());
}

void MainWidget::GoToHomePage()
{
    homepage->setQuestionCnt(QuMag->GetSumQuestionCount());
    this->qsw->setCurrentWidget(homepage);
    MyAdjustSize();
}


void MainWidget::startRandomAnswer()
{
    if(!QuMag->GetSumQuestionCount())
    {
        QMessageBox::critical(this,"错误","当前选择的数据库的题库为空");
        return;
    }

    //获取最大的题目数量
    int maxcnt = QuMag->GetSumQuestionCount();

    //记录用户点击了OK还是取消
    bool ok;

    //num表示所抽题目的数量
    int num = QInputDialog::getInt(this,
                                   "输入",
                                   "请输入要抽取的题目数量",
                                   maxcnt/2, //默认值
                                   1,
                                   maxcnt,
                                   1,//步长
                                   &ok);

    //如果用户没有点击确定，ok为false
    if(!ok)
    {
        qDebug() << "User clicked the button of cancel";
        return;
    }

    //如果输入的num的数量和题库中总题数相等，直接当做普通答题
    if(num==maxcnt)
    {
        this->StartAnswer();
        return;
    }

    //调用答题管理类的随机抽题函数
    quizMag->randomGetQuestions(num);

    //开始答题
    StartAnswer();
}

void MainWidget::startFullAnswer()
{
    if(!QuMag->GetSumQuestionCount())
    {
        QMessageBox::critical(this,"错误","当前选择的数据库的题库为空");
        return;
    }
    quizMag->copyStoreToWait();

    //开始答题
    StartAnswer();
}

MainWidget::~MainWidget()
{
    delete qsw;
    delete homepage;
    delete importjsonpage;
    delete answerpage;
}
