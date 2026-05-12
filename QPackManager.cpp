#include"QPackManager.h"
#include"MyUtils.h"
#include"MenuBar.h"
#include<QCoreApplication>
#include<QDir>
#include<QFileDialog>
#include<QFileInfo>
#define DATABASEDIR "Database"


void QPackManager::packQuestionStore(const QString &questionStoreName)
{
    QString questionStoreDir = QCoreApplication::applicationDirPath()+QDir::separator()+DATABASEDIR;
    QString chosenDir = QFileDialog::getExistingDirectory(nullptr,
                                                          "请选择文件夹",
                                                          QString(),
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                                                          );
    if(chosenDir.isEmpty())
    {
        qDebug() << "用户点了取消";
        return;
    }
    if(!JlCompress::compressDir(chosenDir + QDir::separator() + questionStoreName+".qpack",
                                 questionStoreDir+QDir::separator() + questionStoreName,
                                 true))
    {
        qWarning() << "压缩失败";
        return;
    }
}

QString QPackManager::UnpackQuestionStore(const QString &qpackPath)
{
    QFileInfo qpackInfo = QFileInfo(qpackPath);
    QString baseName = qpackInfo.baseName();
    if(Utils::checkDirExists(baseName)||Utils::checkFileExists(baseName))
    {
        qDebug() << "数据库" << baseName << "已存在";
        this->databaseExists();
        return "";
    }
    QString dbDir = Utils::getDatabaseDir(baseName);
    QDir dir(dbDir);
    if(!dir.exists())
    {
        dir.mkpath(dbDir);
        qDebug() << "数据库文件夹" << baseName << "不存在，已重新创建";
    }

    // JlCompress::extractDir(压缩包路径, 解压目标文件夹)
    QStringList extractedFiles = JlCompress::extractDir(qpackPath,dbDir);

    if (extractedFiles.isEmpty()) {
        qDebug() << "解压失败或压缩包为空";
        return "";
    } else {
        qDebug() << "成功解压文件数：" << extractedFiles.size();
        QFileInfoList filelist = dir.entryInfoList(
            QStringList() << "*.db",
            QDir::Files
            );
        if(!filelist.isEmpty())
        {
            QString filepath = filelist.first().absoluteFilePath();
            QString destpath = Utils::getDatabaseFile(baseName);
            QFile::rename(filepath,destpath);
        }
        else
        {
            qWarning() << "qpack内部没有db文件";
            return "";
        }
        return baseName;
    }
}

QPackManager::QPackManager()
{

}
