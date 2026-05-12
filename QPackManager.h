#ifndef ZIPMANAGER_H
#define ZIPMANAGER_H
#include"quazip.h"
#include"JlCompress.h"
#include<QString>

class QPackManager:public QObject
{
    Q_OBJECT
public slots:
    void packQuestionStore(const QString &questionStoreName);
    QString UnpackQuestionStore(const QString &qpackPath);
public:
    QPackManager();
private:
signals:
    void databaseExists();
};

#endif // ZIPMANAGER_H
