#include"HomePage.h"
#include<QApplication>

HomePage::HomePage(QWidget* parent)
    :QWidget(parent)
{
    //初始化按钮
    Start_Answer_Button = new QPushButton("开始答题",this);
    Random_Answer_Button = new QPushButton("随机抽题",this);
    importQuestionFromJsonText = new QPushButton("从文本导入题目",this);
    CurChosenDatabase = new QLabel(this);

    //初始化题目数量标签
    questionCnt = new QLabel(this);

    //初始化布局
    HomePageLayout = new QVBoxLayout(this);

    //连接信号与信号（信号转发）
    connect(Start_Answer_Button,&QPushButton::clicked,this,&HomePage::Start_Answer_Button_Clicked);
    connect(Random_Answer_Button,&QPushButton::clicked,this,&HomePage::Random_Answer_Button_Clicked);
    connect(importQuestionFromJsonText,&QPushButton::clicked,this,&HomePage::importQuestionFromTextButtonClicked);

    HomePageLayout->setAlignment(Qt::AlignCenter);//设置水平+垂直都居中对齐，等价于Qt::AlignHCenter | Qt::AlignVCenter
    HomePageLayout->setSpacing(10);
    HomePageLayout->addWidget(questionCnt);
    HomePageLayout->addWidget(Start_Answer_Button);
    HomePageLayout->addWidget(Random_Answer_Button);
    HomePageLayout->addWidget(importQuestionFromJsonText);

    this->setLayout(HomePageLayout);
}

void HomePage::setQuestionCnt(int cnt)
{
    QString temp = QString("当前题目数量：%1").arg(cnt);
    this->questionCnt->setText(temp);
}

HomePage::~HomePage()
{

}

void HomePage::Exit()
{
    QApplication::quit();
}
