#include "widget.h"
#include"question.h"
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
#include<Qlabel>
#include<QCheckBox>
#include<QLayoutItem>
#include<QFile>
#include<QCoreApplication>


#define PARSE_FAIL_MESSENAGE "JSON文本解析失败"
#define ERROR_MESSENAGE "错误"
#define FILENAME "QuestionsData.json"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),A(nullptr),B(nullptr),C(nullptr),D(nullptr),QLabel_Title(nullptr)//,sum_questions(0)
{
    homepage = new QWidget(this);
    answerpage = new QWidget(this);
    qsw = new QStackedWidget(this);

    qsw->addWidget(homepage);
    qsw->addWidget(answerpage);


    //构建主界面
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(qsw);
    setLayout(MainLayout);

    //主界面按钮
    QPushButton *Start_Answer_Button = new QPushButton("开始答题",homepage);
    QPushButton *Import_Question_Button = new QPushButton("导入题目",homepage);
    QPushButton *Clear = new QPushButton("清空题目",homepage);
    QPushButton *Exit = new QPushButton("退出程序",homepage);


    QVBoxLayout *homepageLayout = new QVBoxLayout(homepage);
    homepageLayout->addWidget(Start_Answer_Button);
    homepageLayout->addWidget(Import_Question_Button);
    homepageLayout->addWidget(Clear);
    homepageLayout->addWidget(Exit);

    //连接信号与槽
    connect(Start_Answer_Button,&QPushButton::clicked,this,&MainWidget::GoToAnswerPage);
    connect(Import_Question_Button,&QPushButton::clicked,this,&MainWidget::GoToImportPage);
    connect(Clear,&QPushButton::clicked,this,&MainWidget::ClearQuestions);
    connect(Exit,&QPushButton::clicked,this,&MainWidget::ExitProgram);

    //构造importpage
    importpage = new QDialog(this);
    QVBoxLayout *ImportLayout = new QVBoxLayout(importpage);
    textedit = new QTextEdit(importpage);
    ImportLayout->addWidget(textedit);
    QPushButton *importpage_Import_Question_Button = new QPushButton("导入题目",importpage);
    ImportLayout->addWidget(importpage_Import_Question_Button);
    connect(importpage_Import_Question_Button,&QPushButton::clicked,this,&MainWidget::ImportQuestions);

    //构建答题界面
    this->Main_AnswerPage_Layout = new QVBoxLayout(answerpage);
    answerpage->setLayout(Main_AnswerPage_Layout);
    this->Button_AnswerPage_Layout = new QHBoxLayout(answerpage);
    Main_AnswerPage_Layout->addLayout(Button_AnswerPage_Layout);
    this->Next_Question = new QPushButton("下一题",answerpage);
    this->Submit = new QPushButton("检查答案",answerpage);
    this->Previous_Question = new QPushButton("上一题",answerpage);
    connect(Submit,&QPushButton::clicked,this,&MainWidget::CheckAnswer);
    connect(Next_Question,&QPushButton::clicked,this,&MainWidget::NextQuestion);
    connect(Previous_Question,&QPushButton::clicked,this,&MainWidget::PreviousQuestion);
    Button_AnswerPage_Layout->addWidget(Previous_Question);
    Button_AnswerPage_Layout->addWidget(Submit);
    Button_AnswerPage_Layout->addWidget(Next_Question);



    QFile Question_File(FILENAME);
    if(!Question_File.open(QIODevice::ReadOnly))
    {
        qDebug() << "只读打开失败" << '\n';
        sum_questions = 0;
        return;
    }
    QByteArray ByteArrayStore = Question_File.readAll();
    QJsonDocument JsonDocStore = QJsonDocument::fromJson(ByteArrayStore);
    QJsonObject JsonObjStore = JsonDocStore.object();
    QJsonArray JsonArrStore = JsonObjStore["questions"].toArray();

    for(int i = 0;i<JsonArrStore.size();i++)
    {
        QJsonObject TempJsonArrStore = JsonArrStore[i].toObject();
        int type = TempJsonArrStore["type"].toInt();
        QString title = TempJsonArrStore["title"].toString();
        QString answer = TempJsonArrStore["answer"].toString();
        QJsonObject options = TempJsonArrStore["options"].toObject();
        if(type==0)
        {
            QString A_Text = options["A"].toString();
            QString B_Text = options["B"].toString();
            QString C_Text = options["C"].toString();
            QString D_Text = options["D"].toString();
            Single_Question *q = new Single_Question(i,0,title,A_Text,B_Text,C_Text,D_Text,answer);
            this->questions.push_back(q);
        }
        else if(type==1)
        {
            QString A_Text = options["A"].toString();
            QString B_Text = options["B"].toString();
            QString C_Text = options["C"].toString();
            QString D_Text = options["D"].toString();
            Mutiply_Question *q = new Mutiply_Question(i,0,title,A_Text,B_Text,C_Text,D_Text,answer);
            this->questions.push_back(q);
        }
        else
        {
            QString A_Text = options["A"].toString();
            QString B_Text = options["B"].toString();
            True_or_False_Question *q = new True_or_False_Question(i,0,title,A_Text,B_Text,answer);
            this->questions.push_back(q);
        }
    }
    this->sum_questions = JsonArrStore.size();
}

/*
void MainWidget::ClearWidget()
{
    // 获取 answerpage 的所有直接子控件
    QList<QWidget*> children = answerpage->findChildren<QWidget*>(Qt::FindDirectChildrenOnly);
    for (QWidget* child : children) {
        // 跳过两个按钮（它们是 QPushButton）
        if (qobject_cast<QPushButton*>(child)) {
            continue;
        }
        // 删除其他控件（标题、选项等）
        delete child;
    }

    // 重置成员变量指针（它们已经被删除，变为悬空指针，必须置空）
    QLabel_Title = nullptr;
    A = nullptr;
    B = nullptr;
    C = nullptr;
    D = nullptr;

    // 保存按钮布局，重建主布局
    Main_AnswerPage_Layout->removeItem(Button_AnswerPage_Layout);
    delete Main_AnswerPage_Layout;
    Main_AnswerPage_Layout = new QVBoxLayout(answerpage);
    answerpage->setLayout(Main_AnswerPage_Layout);
    Main_AnswerPage_Layout->addLayout(Button_AnswerPage_Layout);

    // 强制界面刷新
    answerpage->update();
}
*/

void MainWidget::ClearWidget()
{
    if(A)delete A;
    if(B)delete B;
    if(C)delete C;
    if(D)delete D;
    if(QLabel_Title)delete QLabel_Title;
    QLabel_Title = nullptr;
    A = nullptr;
    B = nullptr;
    C = nullptr;
    D = nullptr;
}

void MainWidget::GoToAnswerPage()
{
    if(this->sum_questions==0)
    {
        QMessageBox::critical(nullptr,"警告","题库中没有任何题目");
        return;
    }
    ClearWidget();
    qsw->setCurrentWidget(answerpage);
    Question *q = this->questions.front();
    int type = q->getType();
    this->cur_type = type;
    QString title = q->getTitle();
    QString Answer = q->getAnswer();
    this->cur_answer = Answer;
    this->cur_question = 0;
    if(type==0)
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();
        QString C_Text = q->getCText();
        QString D_Text = q->getDText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);


        //构建选项
        this->A = new QRadioButton("A. "+A_Text,answerpage);
        this->B = new QRadioButton("B. "+B_Text,answerpage);
        this->C = new QRadioButton("C. "+C_Text,answerpage);
        this->D = new QRadioButton("D. "+D_Text,answerpage);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
        this->Main_AnswerPage_Layout->insertWidget(3,C);
        this->Main_AnswerPage_Layout->insertWidget(4,D);
    }
    else if(type==1)
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();
        QString C_Text = q->getCText();
        QString D_Text = q->getDText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QCheckBox("A. "+A_Text,answerpage);
        this->B = new QCheckBox("B. "+B_Text,answerpage);
        this->C = new QCheckBox("C. "+C_Text,answerpage);
        this->D = new QCheckBox("D. "+D_Text,answerpage);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
        this->Main_AnswerPage_Layout->insertWidget(3,C);
        this->Main_AnswerPage_Layout->insertWidget(4,D);
    }
    else
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QRadioButton("A. "+A_Text,answerpage);
        this->B = new QRadioButton("B. "+B_Text,answerpage);

        Main_AnswerPage_Layout->removeWidget(C);
        Main_AnswerPage_Layout->removeWidget(D);


        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
    }
}

void MainWidget::GoToImportPage()
{
    importpage->exec();
}

void MainWidget::ImportQuestions()
{
    QString questions = textedit->toPlainText();
    if(questions.isEmpty())
    {
        QMessageBox::critical(nullptr,"错误","输入的文本为空");
        return;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(questions.toUtf8());
    if (jsonDoc.isNull()) {
        QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    if(!jsonObj.contains("questions"))
    {
        QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
        return;
    }
    QJsonArray jsonArr = jsonObj.value("questions").toArray();
    for(int i = 0;i<jsonArr.size();i++)
    {
        QJsonObject TempJsonObj = jsonArr[i].toObject();
        if(!TempJsonObj.contains("type"))
        {
            QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return;
        }
        int type = TempJsonObj["type"].toInt();
        if(!TempJsonObj.contains("options"))
        {
            QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return;
        }
        QJsonObject options = TempJsonObj["options"].toObject();
        if(!TempJsonObj.contains("title"))
        {
            QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return;
        }
        QString title = TempJsonObj["title"].toString();
        if(!TempJsonObj.contains("answer"))
        {
            QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return;
        }
        if(type==0)
        {
            if(!options.contains("A")||!options.contains("B")||!options.contains("C")||!options.contains("D"))
            {
                QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
                return;
            }
            QString A_Text = options.value("A").toString();
            QString B_Text = options.value("B").toString();
            QString C_Text = options.value("C").toString();
            QString D_Text = options.value("D").toString();
            QString Answer = TempJsonObj.value("answer").toString();
            Question *q = new Single_Question(i,type,title,A_Text,B_Text,C_Text,D_Text,Answer);
            this->questions.push_back(q);
            this->sum_questions++;
        }
        else if(type==1)
        {
            if(!options.contains("A")||!options.contains("B")||!options.contains("C")||!options.contains("D"))
            {
                QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
                return;
            }
            QString A_Text = options.value("A").toString();
            QString B_Text = options.value("B").toString();
            QString C_Text = options.value("C").toString();
            QString D_Text = options.value("D").toString();
            QString Answer = TempJsonObj.value("answer").toString();
            Question *q = new Mutiply_Question(i,type,title,A_Text,B_Text,C_Text,D_Text,Answer);
            this->questions.push_back(q);
            this->sum_questions++;
        }
        else
        {
            if(!options.contains("A")||!options.contains("B"))
            {
                QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
                return;
            }
            QString A_Text = options.value("A").toString();
            QString B_Text = options.value("B").toString();
            QString Answer = TempJsonObj.value("answer").toString();
            Question *q = new True_or_False_Question(i,type,title,A_Text,B_Text,Answer);
            this->questions.push_back(q);
            this->sum_questions++;
        }
    }
    QMessageBox::information(nullptr,"提示","导入题目成功");
    importpage->hide();
    QFile file(FILENAME);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "写入打开文件失败" << '\n';
        return;
    }
    QJsonObject rootObj;
    QJsonArray rootQuestionArray;
    for(int i = 0;i<this->questions.size();i++)
    {
        Question *q = this->questions[i];
        if(this->questions[i]->getType()==0)
        {
            QJsonObject TempJsonObj;
            QJsonObject Options;
            Options["A"] = q->getAText();
            Options["B"] = q->getBText();
            Options["C"] = q->getCText();
            Options["D"] = q->getDText();
            TempJsonObj["options"] = Options;
            TempJsonObj["answer"] = q->getAnswer();
            TempJsonObj["title"] = q->getTitle();
            TempJsonObj["type"] = 0;
            rootQuestionArray.append(TempJsonObj);

        }
        else if(this->questions[i]->getType()==1)
        {
            QJsonObject TempJsonObj;
            QJsonObject Options;
            Options["A"] = q->getAText();
            Options["B"] = q->getBText();
            Options["C"] = q->getCText();
            Options["D"] = q->getDText();
            TempJsonObj["options"] = Options;
            TempJsonObj["answer"] = q->getAnswer();
            TempJsonObj["title"] = q->getTitle();
            TempJsonObj["type"] = 1;
            rootQuestionArray.append(TempJsonObj);
        }
        else
        {
            QJsonObject TempJsonObj;
            QJsonObject Options;
            Options["A"] = q->getAText();
            Options["B"] = q->getBText();
            TempJsonObj["options"] = Options;
            TempJsonObj["answer"] = q->getAnswer();
            TempJsonObj["title"] = q->getTitle();
            TempJsonObj["type"] = 2;
            rootQuestionArray.append(TempJsonObj);
        }
    }
    rootObj["questions"] = rootQuestionArray;
    QJsonDocument doc(rootObj);
    file.write(doc.toJson());
}

void MainWidget::CheckAnswer()
{
    if(cur_type==0)
    {
        QString Checked;
        if(A->isChecked())
        {
            Checked = "A";
        }
        else if(B->isChecked())
        {
            Checked = "B";
        }
        else if(C->isChecked())
        {
            Checked = "C";
        }
        else
        {
            Checked = "D";
        }
        if(Checked==cur_answer)
        {
            QMessageBox::information(this,"提示","答案正确");
        }
        else
        {
            QMessageBox::critical(this,"警告","答案错误");
        }
    }
    else if(cur_type==1)
    {
        QString Checked = "";
        if(A->isChecked())
        {
            Checked += "A";
        }
        if(B->isChecked())
        {
            Checked += "B";
        }
        if(C->isChecked())
        {
            Checked += "C";
        }
        if(D->isChecked())
        {
            Checked += "D";
        }
        if(Checked==cur_answer)
        {
            QMessageBox::information(this,"提示","答案正确");
        }
        else
        {
            QMessageBox::critical(this,"警告","答案错误");
        }
    }
    else
    {
        QString Checked;
        if(A->isChecked())
        {
            Checked = "A";
        }
        else if(B->isChecked())
        {
            Checked = "B";
        }
        if(Checked==cur_answer)
        {
            QMessageBox::information(this,"提示","答案正确");
        }
        else
        {
            QMessageBox::critical(this,"警告","答案错误");
        }
    }
}

void MainWidget::NextQuestion()
{
    if(cur_question==sum_questions-1)
    {
        QMessageBox::critical(nullptr,"警告","已经是最后一题了！");
        this->qsw->setCurrentWidget(homepage);
        return;
    }
    ClearWidget();
    cur_question++;
    Question *q = this->questions[cur_question];
    int type = q->getType();
    this->cur_type = type;
    QString title = q->getTitle();
    QString Answer = q->getAnswer();
    this->cur_answer = Answer;
    if(type==0)
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();
        QString C_Text = q->getCText();
        QString D_Text = q->getDText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QRadioButton("A. "+A_Text,answerpage);
        this->B = new QRadioButton("B. "+B_Text,answerpage);
        this->C = new QRadioButton("C. "+C_Text,answerpage);
        this->D = new QRadioButton("D. "+D_Text,answerpage);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
        this->Main_AnswerPage_Layout->insertWidget(3,C);
        this->Main_AnswerPage_Layout->insertWidget(4,D);
    }
    else if(type==1)
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();
        QString C_Text = q->getCText();
        QString D_Text = q->getDText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QCheckBox("A. "+A_Text,answerpage);
        this->B = new QCheckBox("B. "+B_Text,answerpage);
        this->C = new QCheckBox("C. "+C_Text,answerpage);
        this->D = new QCheckBox("D. "+D_Text,answerpage);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
        this->Main_AnswerPage_Layout->insertWidget(3,C);
        this->Main_AnswerPage_Layout->insertWidget(4,D);
    }
    else
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QRadioButton("A. "+A_Text,answerpage);
        this->B = new QRadioButton("B. "+B_Text,answerpage);

        Main_AnswerPage_Layout->removeWidget(C);
        Main_AnswerPage_Layout->removeWidget(D);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
    }
}

void MainWidget::ClearQuestions()
{
    this->questions.clear();
    sum_questions = 0;
    cur_question = 0;
    QFile file(FILENAME);
    if(!file.exists())
    {
        QMessageBox::critical(nullptr,"警告","文件不存在");
        return;
    }
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr,"警告","写入打开文件失败");
        return;
    }
    file.resize(0);
    QMessageBox::information(nullptr,"提示","清空题目成功");
}

void MainWidget::PreviousQuestion()
{
    if(cur_question==0)
    {
        QMessageBox::critical(nullptr,"警告","已经是第一题了！");
        return;
    }
    ClearWidget();
    cur_question--;
    Question *q = this->questions[cur_question];
    int type = q->getType();
    this->cur_type = type;
    QString title = q->getTitle();
    QString Answer = q->getAnswer();
    this->cur_answer = Answer;
    if(type==0)
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();
        QString C_Text = q->getCText();
        QString D_Text = q->getDText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QRadioButton("A. "+A_Text,answerpage);
        this->B = new QRadioButton("B. "+B_Text,answerpage);
        this->C = new QRadioButton("C. "+C_Text,answerpage);
        this->D = new QRadioButton("D. "+D_Text,answerpage);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
        this->Main_AnswerPage_Layout->insertWidget(3,C);
        this->Main_AnswerPage_Layout->insertWidget(4,D);
    }
    else if(type==1)
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();
        QString C_Text = q->getCText();
        QString D_Text = q->getDText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QCheckBox("A. "+A_Text,answerpage);
        this->B = new QCheckBox("B. "+B_Text,answerpage);
        this->C = new QCheckBox("C. "+C_Text,answerpage);
        this->D = new QCheckBox("D. "+D_Text,answerpage);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
        this->Main_AnswerPage_Layout->insertWidget(3,C);
        this->Main_AnswerPage_Layout->insertWidget(4,D);
    }
    else
    {
        QString A_Text = q->getAText();
        QString B_Text = q->getBText();

        //构建标题
        QLabel_Title = new QLabel(title,answerpage);
        this->Main_AnswerPage_Layout->insertWidget(0,QLabel_Title);

        //构建选项
        this->A = new QRadioButton("A. "+A_Text,answerpage);
        this->B = new QRadioButton("B. "+B_Text,answerpage);

        Main_AnswerPage_Layout->removeWidget(C);
        Main_AnswerPage_Layout->removeWidget(D);

        this->Main_AnswerPage_Layout->insertWidget(1,A);
        this->Main_AnswerPage_Layout->insertWidget(2,B);
    }
}

void MainWidget::ExitProgram()
{
    qApp->quit();
}

MainWidget::~MainWidget()
{
    delete qsw;
    delete homepage;
    delete importpage;
    delete answerpage;
}
