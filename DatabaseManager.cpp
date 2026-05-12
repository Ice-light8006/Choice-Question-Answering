#include"DatabaseManager.h"
#include"MyUtils.h"
#include<QCoreApplication>
#include<QDir>
#include<QSqlQuery>
#include<QSqlError>
#include<QFile>
#include<QDebug>

#include"QuestionsManager.h"

#define DATABASEDIR "Database"

DatabaseManager::DatabaseManager()
{
    //构建数据库文件夹的路径
    QString DatabaseDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + DATABASEDIR;

    //构建QDir
    QDir dir(DatabaseDirPath);

    //获取文件夹内部所有的文件夹名称
    QStringList names = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

    for(int i = 0;i<names.size();i++)
    {
        QString path = Utils::getDatabaseFile(names[i]);
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",names[i]);
        db.setDatabaseName(path);
        if(db.open())
        {
            this->DatabaseConnect.insert(names[i]);
            qDebug() << QString("本地数据库%1连接成功").arg(names[i]);
        }
        else
        {
            qWarning() << QString("本地数据库%1连接失败").arg(names[i]) << db.lastError().text();
        }
    }
    //connect(this,&DatabaseManager::ReNameFunctionDone,QuMag,&QuestionsManager::InitializationQuestionStore);
}

void DatabaseManager::AddDatabaseToLocal(QString StoreName)
{
    this->DatabaseConnect.insert(StoreName);
}

void DatabaseManager::CreateQuestionsStore(QString StoreName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", StoreName);
    QString FilePath = QCoreApplication::applicationDirPath() + QDir::separator() + DATABASEDIR+QDir::separator() + StoreName + QDir::separator()+ StoreName + ".db";
    QString DatabasePath = QCoreApplication::applicationDirPath() + QDir::separator() + DATABASEDIR + QDir::separator() + StoreName;
    QDir dir;
    if(!dir.exists(DatabasePath))
    {
        dir.mkpath(DatabasePath);
    }
    db.setDatabaseName(FilePath);
    if(!db.open())
    {
        qDebug() << "打开失败" << '\n';
        return;
    }

    //添加外键约束
    QSqlQuery query_pragma(db);
    query_pragma.exec("PRAGMA foreign_keys = ON;");

    QSqlQuery query(db);//Qt中的QSqlQuery不止可以查询，也可以创建表修改表等

    //创建题目表
    bool ok = query.exec(
        "CREATE TABLE Questions ("
        "id	INTEGER PRIMARY KEY,"
        "title	TEXT NOT NULL,"
        "count	INTEGER NOT NULL,"
        "type	INTEGER NOT NULL,"
        "image_count	INTEGER NOT NULL,"
        "answer	TEXT NOT NULL"
        ");"
        );

    //创建选项表
    ok &= query.exec(
        "CREATE TABLE Options ("
        "question_id INTEGER NOT NULL,"
        "option_key TEXT NOT NULL,"
        "option_text TEXT NOT NULL,"
        "FOREIGN KEY (question_id) REFERENCES Questions(id)"
        ");"
        );

    //创建图片表
    ok &= query.exec(
        "CREATE TABLE Images ("
        "question_id INTEGER NOT NULL,"
        "url TEXT NOT NULL,"
        "FOREIGN KEY (question_id) REFERENCES Questions(id)"
        ");"
        );
    if(!ok)
    {
        qDebug() << "创建数据库文件失败" << query.lastError().text();
    }
    else
    {
        qDebug() << "创建数据库文件成功";
        this->DatabaseConnect.insert(StoreName);
    }
}

void DatabaseManager::InsertQuestionToDatabase(Question *q)
{
    //获取当前的连接名称
    QString StoreName = this->GetCurDatabaseConnect();

    //通过指针获取题目数据
    int id = q->getID();
    QString title = q->getTitle();
    QVector<std::pair<QString,QString>> Options = q->getOptions();
    int type = q->getType();
    int count = q->getCount();
    int image_count = q->getImageCount();
    QString answer = q->getAnswer();
    QSqlDatabase db = QSqlDatabase::database(StoreName);

    //判断是否打开
    if(!db.isOpen())
    {
        qDebug() << "错误，数据库没有打开\n";
        return;
    }

    bool success = true;

    //开启事务
    QSqlQuery query(db);

    db.transaction();

    query.prepare("INSERT INTO Questions (id,type,title,answer,count,image_count)"
                  "VALUES(:id, :type, :title, :answer, :count, :image_count)"
                  );

    query.bindValue(":id",id);
    query.bindValue(":type",type);
    query.bindValue(":title",title);
    query.bindValue(":answer",answer);
    query.bindValue(":count",count);
    query.bindValue(":image_count",image_count);

    if(!query.exec())
    {
        qDebug() << "插入题目失败\n";
        success = false;
    }

    //插入选项
    for(int i = 0;i<Options.size();i++)
    {
        QString OptionKey = Options[i].first;
        QString OptionText = Options[i].second;

        query.prepare("INSERT INTO Options (question_id,option_key,option_text)"
                      "VALUES(:qid, :qkey, :qtext)");

        query.bindValue(":qid",id);
        query.bindValue(":qkey",OptionKey);
        query.bindValue(":qtext",OptionText);

        if(!query.exec())
        {
            qDebug() << "插入选项失败\n";
            success = false;
            break;
        }
    }

    //插入图片表
    QVector<QString> ImageUrls = q->getimageUrls();
    for(int i = 0;i<ImageUrls.size();i++)
    {
        QString url = ImageUrls[i];

        query.prepare("INSERT INTO Images(question_id,url)"
                      "VALUES(:id, :url)");

        query.bindValue(":id",id);
        query.bindValue(":url",url);

        if(!query.exec())
        {
            qDebug() << "插入图片失败\n";
            success = false;
            break;
        }
    }

    if(success)
    {
        db.commit();
        qDebug() << "题目插入成功\n";
    }
    else
    {
        db.rollback();
        qDebug() << "插入题目失败\n";
    }
}

void DatabaseManager::addDatabaseFromFile(const QString &filePath)
{
    //构建QDir
    QDir dir(filePath);

    if(!dir.exists())
    {
        qWarning() << "文件" << filePath << "不存在";
        return;
    }

    //获取文件夹内部所有的文件夹名称
    QStringList names = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

    for(int i = 0;i<names.size();i++)
    {
        QString path = Utils::getDatabaseDir()+QDir::separator() + names[i] + QDir::separator() + names[i]+".db";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",names[i]);
        db.setDatabaseName(path);
        if(db.open())
        {
            this->DatabaseConnect.insert(names[i]);
            qDebug() << QString("本地数据库%1连接成功").arg(names[i]);
        }
        else
        {
            qWarning() << QString("本地数据库%1连接失败").arg(names[i]) << db.lastError().text();
        }
    }
    //connect(this,&DatabaseManager::ReNameFunctionDone,QuMag,&QuestionsManager::InitializationQuestionStore);
}

void DatabaseManager::addDatabaseFromName(const QString &storeName)
{
    QString path = Utils::getDatabaseFile(storeName);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",storeName);
    db.setDatabaseName(path);
    if(db.open())
    {
        this->DatabaseConnect.insert(storeName);
        qDebug() << QString("本地数据库%1连接成功").arg(storeName);
    }
    else
    {
        qWarning() << QString("本地数据库%1连接失败").arg(storeName) << db.lastError().text();
    }
}

void DatabaseManager::deleteCurDatabase()
{
    qDebug() << "删除数据库函数执行了";
    QString srcName = this->GetCurDatabaseConnect();
    QString srcDir = QCoreApplication::applicationDirPath()+QDir::separator() + DATABASEDIR + QDir::separator() + srcName +QDir::separator();
    QString srcFile = QCoreApplication::applicationDirPath()+QDir::separator() + DATABASEDIR + QDir::separator() + srcName +QDir::separator() + srcName + ".db";
    QDir dir(srcDir);
    if(!dir.exists())
    {
        qWarning() << "提示：文件夹不存在";
        return;
    }

    // 关闭旧连接 + 删除连接（关键！）
    {
        QSqlDatabase db = QSqlDatabase::database(srcName);
        if(db.isOpen())
            db.close();
    }
    QSqlDatabase::removeDatabase(srcName);

    qDebug() << QSqlDatabase::contains(srcName);

    QDir deleteDir(srcDir);

    //递归删除整个src文件夹（包括src文件夹本身）
    bool ok = deleteDir.removeRecursively();

    if(!ok)
    {
        qWarning() << "删除数据库目录失败（可能仍被占用）";
    }

    //删除集合中对应的连接名
    this->DatabaseConnect.remove(srcName);
}

QString DatabaseManager::GetCurDatabaseConnect()
{
    if(this->CurDatabaseConnect.isEmpty())
    {
        qWarning() << "CurDatabaseConnect为空";
    }
    return this->CurDatabaseConnect;
}

Question* DatabaseManager::GetQuestion(int id)
{
    QString StoreName = this->GetCurDatabaseConnect();
    QSqlDatabase db = QSqlDatabase::database(StoreName);

    QSqlQuery query_pragma(db);
    query_pragma.exec("PRAGMA foreign_keys = ON");

    QSqlQuery questionsQuery(db);



    questionsQuery.prepare("SELECT count,title,image_count,answer,type FROM Questions WHERE id = :qid");
    questionsQuery.bindValue(":qid",id);

    if(!questionsQuery.exec())
    {
        qDebug() << "查询Questions表失败" << questionsQuery.lastError().text();
        return nullptr;
    }
    if(!questionsQuery.next())
    {
        qDebug() << "您所查询的id不存在\n";
        return nullptr;
    }

    int count = questionsQuery.value(0).toInt();
    QString title = questionsQuery.value(1).toString();
    int image_count = questionsQuery.value(2).toInt();
    QString answer = questionsQuery.value(3).toString();
    int type = questionsQuery.value(4).toInt();

    //查询选项
    QSqlQuery optionsQuery(db);
    optionsQuery.prepare("SELECT option_key,option_text FROM Options WHERE question_id = :qid");
    optionsQuery.bindValue(":qid",id);
    if(!optionsQuery.exec())
    {
        qDebug() << "查询Options表失败" << optionsQuery.lastError().text();
    }
    QVector<std::pair<QString,QString>> Options;
    while(optionsQuery.next())
    {
        QString option_key = optionsQuery.value(0).toString();
        QString option_text = optionsQuery.value(1).toString();
        Options.push_back({option_key,option_text});
    }

    //查询图片路径数组
    QSqlQuery imagesQuery(db);
    QVector<QString> ImageUrls;
    imagesQuery.prepare("SELECT url FROM Images WHERE question_id = :qid");
    imagesQuery.bindValue(":qid",id);
    if(!imagesQuery.exec())
    {
        qDebug() << "查询Images表失败" << imagesQuery.lastError().text();
    }
    while(imagesQuery.next())
    {
        QString url = imagesQuery.value(0).toString();
        ImageUrls.push_back(url);
    }

    Question* q = new Question(id,type,title,count,answer,image_count,Options,ImageUrls);

    return q;
}

//注：如果数据库查询失败会返回空的数组
QVector<Question*> DatabaseManager::GetQuestions()
{
    QString StoreName = this->GetCurDatabaseConnect();
    QSqlDatabase db = QSqlDatabase::database(StoreName);

    bool success = true;

    db.transaction();

    QSqlQuery query = QSqlQuery(db);

    //创建题目数组
    QVector<Question*> Questions;

    success &= query.exec("SELECT id,title,type,image_count,answer,count FROM Questions");

    while(success&&query.next())
    {
        //题目基本数据
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        int type = query.value(2).toInt();
        int image_count = query.value(3).toInt();
        QString answer = query.value(4).toString();
        int count = query.value(5).toInt();

        //选项数组
        QVector<std::pair<QString,QString>> OptionsText;

        //构建QSQLQuery
        QSqlQuery OptionQuery(db);

        //预准备SQL指令
        OptionQuery.prepare("SELECT option_key,option_text FROM Options WHERE question_id = :qid");
        OptionQuery.bindValue(":qid",id);

        success &= OptionQuery.exec();

        if(!success)break;

        //遍历结果集中的每一个选项
        while(OptionQuery.next())
        {
            QString key = OptionQuery.value(0).toString();
            QString text = OptionQuery.value(1).toString();
            std::pair<QString,QString> OptionText = {key,text};
            OptionsText.push_back(OptionText);
        }

        //构建图片Url数组
        QVector<QString> ImageUrls;

        //如果image_count为0那就不执行这个if里的内容，那么ImageUrls就是空的
        if(image_count)
        {
            QSqlQuery ImageQuery = QSqlQuery(db);

            ImageQuery.prepare("SELECT url FROM Images WHERE question_id = :qid");
            ImageQuery.bindValue(":qid",id);

            success &= ImageQuery.exec();

            if(!success) break;

            while(ImageQuery.next())
            {
                QString url = ImageQuery.value(0).toString();
                ImageUrls.push_back(url);
            }
        }
        Question* q = new Question(id,type,title,count,answer,image_count,OptionsText,ImageUrls);
        Questions.push_back(q);
    }
    if(success)
    {
        db.commit();
        qDebug() << "DatabaseManager:Get questions vector from DatabaseManager successfully";
        return Questions;
    }
    else
    {
        db.rollback();
        qWarning() << "DatabaseManager:Get questions vector from DatabaseManager unsuccessfully";
        //失败返回空数组
        return Questions;
    }
}

void DatabaseManager::ReviseCurDatabaseConnect(const QString &StoreName)
{
    if(!this->DatabaseConnect.contains(StoreName))
    {
        qWarning() << QString("连接名称%1不存在").arg(StoreName);
        return;
    }
    this->CurDatabaseConnect = StoreName;
    databaseHasBeenRevise();
}

QSet<QString> DatabaseManager::GetAllDatabaseConnectSet()
{
    return this->DatabaseConnect;
}


bool DatabaseManager::ReNameDatabase(const QString &srcName,const QString &destName)
{
    if(srcName==destName)
    {
        qWarning() << "数据库"+srcName+"的名字没有变化";
        return false;
    }
    if(!this->DatabaseConnect.contains(srcName))
    {
        qWarning() << "集合中没有名为"+srcName+"的数据库";
        return false;
    }

    QString srcDir = QCoreApplication::applicationDirPath()+QDir::separator() + DATABASEDIR + QDir::separator() + srcName +QDir::separator();
    QString destDir = QCoreApplication::applicationDirPath()+QDir::separator() + DATABASEDIR + QDir::separator() + destName +QDir::separator();
    QString srcFile = QCoreApplication::applicationDirPath()+QDir::separator() + DATABASEDIR + QDir::separator() + srcName +QDir::separator() + srcName + ".db";
    QString destFile = QCoreApplication::applicationDirPath()+QDir::separator() + DATABASEDIR + QDir::separator() + destName +QDir::separator() + destName + ".db";

    QDir dir;
    if(!dir.exists(destDir))
    {
        if(dir.mkpath(destDir))
        {
            qDebug() << "创建数据库文件夹" + destName + "成功";
        }
        else
        {
            qDebug() << "创建数据库文件夹" + destName + "失败";
            return false;
        }
    }

    if(!QFile::copy(srcFile,destFile))
    {
        qWarning() << "复制数据库"+srcFile+"到"+destFile+"失败";
        return false;
    }

    if (QSqlDatabase::contains(destName)) {
        qWarning() << "连接名" << destName << "已存在，将先移除";
        QSqlDatabase::removeDatabase(destName);
    }

    //重新连接新的数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",destName);
    db.setDatabaseName(destFile);

    if(!db.open())
    {
        qWarning() << "数据库文件打开失败" << db.lastError();
        return false;
    }



    // 关闭旧连接 + 删除连接（关键！）
    {
        QSqlDatabase db = QSqlDatabase::database(srcName);
        if(db.isOpen())
            db.close();
    }
    QSqlDatabase::removeDatabase(srcName);

    qDebug() << QSqlDatabase::contains(srcName);

    QDir deleteDir(srcDir);

    //递归删除整个src文件夹（包括src文件夹本身）
    deleteDir.removeRecursively();

    //修改集合中的名称
    this->DatabaseConnect.remove(srcName);
    this->DatabaseConnect.insert(destName);

    //如果当前数据库连接为srcName，修改为destName
    if(this->CurDatabaseConnect==srcName)
    {
        this->ReviseCurDatabaseConnect(destName);
    }

    qDebug() <<"成功重命名数据库"+srcName+"为"+destName;
    return true;
}

DatabaseManager::~DatabaseManager()
{

}
