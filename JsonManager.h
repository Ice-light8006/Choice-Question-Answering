#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include"Question.h"


class JsonManager
{
public:
    JsonManager();

    //如果Json解析失败会返回空数组
    QVector<Question*> GetQuestionsVectorFromJsonText(QString JsonString);

    ~JsonManager();
private:
};

#endif // JSONMANAGER_H
