#include"JsonManager.h"
#include"Question.h"
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QMessageBox>
#include<QPushButton>
#include<QTextEdit>

JsonManager::JsonManager()
{

}

QVector<Question*> JsonManager::GetQuestionsVectorFromJsonText(QString JsonString)
{
    //创建题目数组
    QVector<Question*> Questions;
    //构建JSON文档
    QJsonDocument jsonDoc = QJsonDocument::fromJson(JsonString.toUtf8());

    //检测JSON文档是否构建成功
    if (jsonDoc.isNull()) {
        //QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
        return Questions;
    }

    //转化为JSON工程
    QJsonObject jsonObj = jsonDoc.object();

    //检测是否包含questions这个键
    if(!jsonObj.contains("questions"))
    {
        //QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
        return Questions;
    }

    //将值转换为JsonArray
    QJsonArray jsonArr = jsonObj.value("questions").toArray();

    //遍历这个数组，读取题目数据
    for(int i = 0;i<jsonArr.size();i++)
    {
        //TempJsonObj存储单个题目的数据
        QJsonObject TempJsonObj = jsonArr[i].toObject();
        if(!TempJsonObj.contains("type"))
        {
            //QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return Questions;
        }
        int type = TempJsonObj["type"].toInt();
        if(!TempJsonObj.contains("options"))
        {
            //QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return Questions;
        }
        QJsonObject options = TempJsonObj["options"].toObject();
        if(!TempJsonObj.contains("title"))
        {
            //QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return Questions;
        }
        QString title = TempJsonObj["title"].toString();
        if(!TempJsonObj.contains("answer"))
        {
            //QMessageBox::critical(nullptr,"错误","JSON文本解析失败");
            return Questions;
        }
        int count = TempJsonObj["count"].toInt();
        QString Answer = TempJsonObj.value("answer").toString();
        int id = TempJsonObj.value("id").toInt();
        int ImageCount = TempJsonObj.value("image_count").toInt();

        //创建一个临时数组存储选项文本
        QVector<std::pair<QString,QString>> OptionText;

        //创建一个图片路径数组
        QVector<QString> ImageUrls;

        //遍历每个选项
        for(auto it = options.begin();it!=options.end();it++)
        {
            QString key = it.key();
            QString text = it.value().toString();
            OptionText.push_back({key,text});
        }

        //构建题目
        Question *q = new Question(id,type,title,count,Answer,ImageCount,OptionText,ImageUrls);

        //将题目加入题库数组
        Questions.push_back(q);
    }
    return Questions;
}

JsonManager::~JsonManager()
{

}

#include"JsonManager.h"
