#ifndef DATABASEMANAGER_H//头文件保护，防止头文件被重复包含
#define DATABASEMANAGER_H
#include"Question.h"
#include<QVector>
#include<QSql>
#include<QSqlDatabase>
#include<QSet>
#include<QObject>

class QuestionsManager;//前向声明

class DatabaseManager: public QObject
{
    Q_OBJECT
public:
    DatabaseManager();
    void AddDatabaseToLocal(QString StoreName);
    void CreateQuestionsStore(QString StoreName);
    void InsertQuestionToDatabase(Question *q);
    void addDatabaseFromFile(const QString &filePath);
    void addDatabaseFromName(const QString &storeName);

    QString GetCurDatabaseConnect();
    Question* GetQuestion(int id);
    QVector<Question*> GetQuestions();
    void ReviseCurDatabaseConnect(const QString &StoreName);
    QSet<QString> GetAllDatabaseConnectSet();
    bool ReNameDatabase(const QString &sourceName,const QString &srcname);
    ~DatabaseManager();
private:
    QSet<QString> DatabaseConnect;
    QString CurDatabaseConnect;
    QuestionsManager* QuMag;
signals:
    void databaseHasBeenRevise();
public slots:
    void deleteCurDatabase();
};

#endif // DATABASEMANAGER_H
