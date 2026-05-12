#include"AnswerPage.h"
#include<QHeaderView>
#include<QRadioButton>
#include<QCheckBox>
#include<QMessageBox>
#include<algorithm>

AnswerPage::AnswerPage(QuizManager* quizMag,QWidget* parent)
    :QWidget(parent)
{
    //传入题库指针
    this->quizMag = quizMag;

    //初始化按钮
    Previous_Question_Button = new QPushButton("上一题",this);
    Submit_Button = new QPushButton("检查答案",this);
    Return_Home_Button = new QPushButton("回到主页",this);
    Next_Question_Button = new QPushButton("下一题",this);


    //初始化布局

    this->Main_AnswerPage_Layout = new QVBoxLayout();
    this->Button_AnswerPage_Layout = new QHBoxLayout();
    this->Root_AnswerPage_Layout = new QHBoxLayout(this);//传this的布局作为顶级布局
    this->TableWidget_Layout = new QVBoxLayout();
    this->TitleLabel_Layout = new QVBoxLayout();
    this->Options_Layout = new QVBoxLayout();

    //构建选题表格
    this->TableWidget = new QTableWidget(this);

    // 调整所有列宽以适应内容
    TableWidget->resizeColumnsToContents();

    // 调整所有行高以适应内容
    TableWidget->resizeRowsToContents();



    //初始化标题Label
    this->Title = new QLabel(this);
    Title->setWordWrap(true);
    //Title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);//宽度正常，高度只用“刚好包住文字”

    //构建按钮布局
    Button_AnswerPage_Layout->addWidget(Previous_Question_Button);
    Button_AnswerPage_Layout->addWidget(Submit_Button);
    Button_AnswerPage_Layout->addWidget(Return_Home_Button);
    Button_AnswerPage_Layout->addWidget(Next_Question_Button);

    //构建表格布局
    this->TableWidget_Layout->addWidget(TableWidget);


    //构建根布局
    this->Root_AnswerPage_Layout->addLayout(Main_AnswerPage_Layout);
    this->Root_AnswerPage_Layout->addLayout(TableWidget_Layout);

    //构建主答题布局
    this->Main_AnswerPage_Layout->addLayout(TitleLabel_Layout,0);
    this->Main_AnswerPage_Layout->addStretch();
    this->Main_AnswerPage_Layout->addLayout(Options_Layout,1);
    this->Main_AnswerPage_Layout->addStretch();
    this->Main_AnswerPage_Layout->addLayout(Button_AnswerPage_Layout,0);

    //构建Title布局
    this->TitleLabel_Layout->addWidget(Title);

    //设置主布局
    this->setLayout(Root_AnswerPage_Layout);

    //连接信号与信号
    connect(Previous_Question_Button,&QPushButton::clicked,this,&AnswerPage::Previous_Question_Button_Clicked);
    connect(Submit_Button,&QPushButton::clicked,this,&AnswerPage::Submit_Button_Clicked);
    connect(Return_Home_Button,&QPushButton::clicked,this,&AnswerPage::Return_Home_Button_Clicked);
    connect(Next_Question_Button,&QPushButton::clicked,this,&AnswerPage::Next_Question_Button_Clicked);

    //连接按钮的信号与槽
    connect(this,&AnswerPage::Previous_Question_Button_Clicked,this,&AnswerPage::GoToPreviousQuestion);
    connect(this,&AnswerPage::Next_Question_Button_Clicked,this,&AnswerPage::GoToNextQuestion);
    connect(this,&AnswerPage::Submit_Button_Clicked,this,&AnswerPage::CheckAnswer);

    //链接表格的信号与槽
    connect(TableWidget,&QTableWidget::cellClicked,this,[=](int row,int column){
        if(row*5+column>=quizMag->getWaitQuestionCount())return;
        int index = row*5+column;
        Question* q = quizMag->getQuestionFromWaitQuestion(index);
        this->BuildQuestion(q);
        this->InformMainWindowAdjustSize();
        quizMag->reviseCurIndex(index);
        quizMag->reviseCurQuestion(q);
    });
}

void AnswerPage::InitializeTableWidget(int cnt)
{
    this->TableWidget->clear();
    int row = ceil(cnt/5.0);
    int column = 5;
    this->TableWidget->setRowCount(row);
    this->TableWidget->setColumnCount(column);
    for(int i = 0;i<ceil(cnt*1.0/5)*5;i++)
    {

        QTableWidgetItem *Item = new QTableWidgetItem(QString::number(i+1));
        if(i>=cnt)
        {
            Item->setFlags(Qt::NoItemFlags);
        }
        this->TableWidget->setItem(i/5,i%5,Item);

    }
    if(cnt%5!=0)
    {
        for(int i = cnt;i<row*5;i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
            //item->setBackground(QBrush(Qt::white));
            item->setBackground(Qt::transparent);
            this->TableWidget->setItem(i/5, i%5, item);
        }
    }
    TableWidget->resizeColumnsToContents();
    TableWidget->resizeRowsToContents();
    TableWidget->setFixedWidth(160);
    TableWidget->horizontalHeader()->hide();
    TableWidget->verticalHeader()->hide();
    TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //TableWidget->setFocusPolicy(Qt::NoFocus);
    TableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    TableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    TableWidget->setFocusPolicy(Qt::StrongFocus);
    TableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    //初始设置为第一个单元格
    TableWidget->setCurrentCell(0,0);

}

void AnswerPage::ReChosenTableWidgetItem(int index)
{
    TableWidget->setCurrentCell(index/5,index%5);
}

void AnswerPage::BuildQuestion(Question *q)
{
    if(!this->Options.isEmpty())
    {
        this->Options.clear();
    }
    //清空选项的布局
    this->ClearOptionsLayout();

    //获取题目的title
    QString title = q->getTitle();

    //清空标题的布局
    this->ReTitleText(title);

    //获取选项数量
    int count = q->getCount();

    //获取题目类型
    int type = q->getType();

    //构建选项数组
    QVector<std::pair<QString,QString>> Options(count);

    //获取选项数组
    Options = q->getOptions();

    for(int i = 0;i<count;i++)
    {
        QString key = Options[i].first;
        key+=". ";
        QString text = Options[i].second;
        QAbstractButton *btn =this->CreateAbstractBtn(type,key+text,this);
        btn->setProperty("key",Options[i].first);
        this->Options.push_back(btn);
        this->Options_Layout->addWidget(btn);
    }
    this->InformMainWindowAdjustSize();
    quizMag->reviseCurQuestion(q);
}

QAbstractButton *AnswerPage::CreateAbstractBtn(int type,const QString &text,QWidget *parent)
{
    QAbstractButton *AbsBtn;
    switch(type)
    {
    case 0:
        AbsBtn = new QRadioButton(text,parent);
        return AbsBtn;
        break;
    default:
        AbsBtn = new QCheckBox(text,parent);
        return AbsBtn;
        break;
    }
}

void AnswerPage::ReTitleText(const QString &text)
{
    this->Title->setText(text);
}

void AnswerPage::ClearOptionsLayout()
{
    //获取选项布局
    QLayout *layout = this->Options_Layout;

    //先判断是不是空指针
    if(!layout)return;

    QLayoutItem *item = nullptr;

    while((item = layout->takeAt(0))!=nullptr)
    {
        QWidget *widget = item->widget();
        if(widget)
        {
            widget->deleteLater();
        }

    }
    delete item;
}

QPushButton* AnswerPage::GetReturn_Home_Button()
{
    return this->Return_Home_Button;
}

AnswerPage::~AnswerPage()
{

}

void AnswerPage::CheckAnswer()
{
    //获取题目答案
    QString cur_answer = quizMag->getCurQuestion()->getAnswer();
    std::sort(cur_answer.begin(),cur_answer.end());

    //Checked记录用户勾选的选项
    QString Checked = "";

    //遍历每个选项，检查是否被勾选
    for(int i = 0;i<this->Options.size();i++)
    {
        if(this->Options[i]->isChecked())
        {
            //如果勾选就把这个选项加进Checked
            Checked += this->Options[i]->property("key").toString();
        }
    }
    std::sort(Checked.begin(),Checked.end());

    //提示窗口
    if(Checked==cur_answer)
    {
        QMessageBox::information(this,"提示","答案正确");
    }
    else
    {
        QMessageBox::critical(this,"警告","答案错误");
    }
}

void AnswerPage::GoToPreviousQuestion()
{
    int index = quizMag->getCurIndex();
    if(index==0)
    {
        QMessageBox::information(this,"提示","现在是第一题");
        return;
    }
    index--;
    quizMag->reviseCurQuestion(quizMag->getQuestionFromWaitQuestion(index));
    quizMag->reviseCurIndex(index);
    BuildQuestion(quizMag->getCurQuestion());
    this->TableWidget->setCurrentCell(index/5,index%5);
    this->adjustSize();
    this->InformMainWindowAdjustSize();
}

void AnswerPage::GoToNextQuestion()
{
    int index = quizMag->getCurIndex();
    if(index==quizMag->getWaitQuestionCount()-1)
    {
        QMessageBox::information(this,"提示","现在是最后一题");
        return;
    }
    index++;
    quizMag->reviseCurQuestion(quizMag->getQuestionFromWaitQuestion(index));
    quizMag->reviseCurIndex(index);
    BuildQuestion(quizMag->getCurQuestion());
    this->TableWidget->setCurrentCell(index/5,index%5);
    this->adjustSize();
    this->InformMainWindowAdjustSize();
}
