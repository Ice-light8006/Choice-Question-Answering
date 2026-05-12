#ifndef QUESTIONSMANAGER_H
#define QUESTIONSMANAGER_H
#include"Question.h"
#include<QObject>

//前向声明
class DatabaseManager;

class QuestionsManager: public QObject
{
    Q_OBJECT
public:
    QuestionsManager(DatabaseManager* dbMag);

    void AddQuestion(Question* q);
    int GetSumQuestionCount();
    QVector<Question*> getQuestionStore();
    ~QuestionsManager();
private:
    //题库数组
    QVector<Question*> QuestionStore;

    //数据库
    DatabaseManager* dbMag;
public slots:
    void InitializationQuestionStore();
};

#endif // QUESTIONSMANAGER_H
