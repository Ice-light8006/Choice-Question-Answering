#ifndef QUESTION_H
#define QUESTION_H
#include<QString>
#include<QVector>

enum Type
{
    Single = 0,
    Mutiply = 1,
    Indefine = 2
};

class Question
{
public:
    Question(int id,int type,QString title,int count,QString Answer,int ImageCount,QVector<std::pair<QString,QString>> Options,QVector<QString> ImageUrls);
    int getType()
    {
        return this->type;
    }
    int getID()
    {
        return this->id;
    }
    QString getTitle()
    {
        return this->title;
    }
    int getCount()
    {
        return this->count;
    }
    QString getAnswer()
    {
        return this->Answer;
    }
    int getImageCount()
    {
        return this->ImageCount;
    }
    QVector<QString> getimageUrls()
    {
        return this->imageUrls;
    }
    QVector<std::pair<QString,QString>> getOptions()
    {
        return this->Options;
    }
    ~Question();
protected:
    int id;
    int type;//0表示单选，1表示多选，3表示不定项选择
    int count;
    QString title;
    QString Answer;
    int ImageCount;
    QVector<QString> imageUrls;
    QVector<std::pair<QString,QString>> Options;
};

#endif // QUESTION_H
