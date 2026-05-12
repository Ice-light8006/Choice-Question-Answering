#include"QuestionsManager.h"
#include"DatabaseManager.h"
#include"Question.h"
#include<QSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

QuestionsManager::QuestionsManager(DatabaseManager* dbMag)
{
    this->dbMag = dbMag;

    //连接dbMag的修改数据库连接的信号到初始化题库数组的槽
    connect(dbMag,&DatabaseManager::databaseHasBeenRevise,this,&QuestionsManager::InitializationQuestionStore);
}

void QuestionsManager::InitializationQuestionStore()
{
    if(!this->QuestionStore.isEmpty())
    {
        this->QuestionStore.clear();
    }
    QString StoreName = dbMag->GetCurDatabaseConnect();
    QVector<Question*> Questions = dbMag->GetQuestions();
    int len = Questions.size();
    this->QuestionStore = Questions;
    qDebug() << "从数据库" + dbMag->GetCurDatabaseConnect() +"初始化题库成功";
    qDebug() << "导入QuMag的题目总共有" << len << "个";
}

void QuestionsManager::AddQuestion(Question *q)
{
    this->QuestionStore.push_back(q);
}



int QuestionsManager::GetSumQuestionCount()
{
    return (int)(this->QuestionStore.size());
}

QVector<Question*> QuestionsManager::getQuestionStore()
{
    return this->QuestionStore;
}



QuestionsManager::~QuestionsManager()
{

}
