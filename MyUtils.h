#ifndef MYUTILS_H
#define MYUTILS_H

#include<QString>
#include<QCoreApplication>
#include<QDir>

namespace Utils
{
    inline QString getDatabaseDir()
    {
        QString temp = QCoreApplication::applicationDirPath()+QDir::separator()+"Database";
        return temp;
    }

    inline QString getDatabaseDir(const QString &dbName)
    {
        //如果传入的参数为空，直接返回空字符串
        if(dbName=="")
        {
            return dbName;
        }
        QString temp = QCoreApplication::applicationDirPath()+QDir::separator()+"Database" + QDir::separator() + dbName;
        return temp;
    }

    inline QString getDatabaseFile(const QString &dbName)
    {
        //如果传入的参数为空，直接返回空字符串
        if(dbName=="")
        {
            return dbName;
        }
        QString temp = getDatabaseDir(dbName);
        temp+=QDir::separator()+dbName+".db";
        return temp;
    }

    inline bool checkFileExists(const QString &dbName)
    {
        //如果传入的参数为空，直接返回false
        if(dbName=="")
        {
            return false;
        }
        QString dbFile = getDatabaseFile(dbName);
        QDir dir(dbFile);
        return dir.exists();
    }

    inline bool checkDirExists(const QString &dbName)
    {
        //如果传入的参数为空，直接返回false
        if(dbName=="")
        {
            return false;
        }
        QString dbDir = getDatabaseDir(dbName);
        QDir dir(dbDir);
        return dir.exists();
    }
}

#endif // MYUTILS_H
