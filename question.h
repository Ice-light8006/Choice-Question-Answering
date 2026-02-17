#ifndef QUESTION_H
#define QUESTION_H
#include<QString>
#include<QVector>

class Question
{
public:
    Question(int id,int type,QString title);
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
    QString getAnswer()
    {
        return this->Answer;
    }
    virtual QString getAText() = 0;
    virtual QString getBText() = 0;
    virtual QString getCText() = 0;
    virtual QString getDText() = 0;
    ~Question();
protected:
    int id;
    int type;//0表示单选，1表示多选，2表示判断
    QString title;
    QString Answer;
};

class Single_Question:public Question
{
public:
    Single_Question(int id,int type,QString title,QString A_Text,QString B_Text,QString C_Text,QString D_Text,QString Answer);
    QString getAText()
    {
        return A_Text;
    }
    QString getBText()
    {
        return B_Text;
    }
    QString getCText()
    {
        return C_Text;
    }
    QString getDText()
    {
        return D_Text;
    }
    ~Single_Question();
protected:
    QString A_Text;
    QString B_Text;
    QString C_Text;
    QString D_Text;

};

class Mutiply_Question:public Question
{
public:
    Mutiply_Question(int id,int type,QString title,QString A_Text,QString B_Text,QString C_Text,QString D_Text,QString Answer);
    QString getAText()
    {
        return A_Text;
    }
    QString getBText()
    {
        return B_Text;
    }
    QString getCText()
    {
        return C_Text;
    }
    QString getDText()
    {
        return D_Text;
    }
    ~Mutiply_Question();
protected:
    QString A_Text;
    QString B_Text;
    QString C_Text;
    QString D_Text;
};

class True_or_False_Question:public Question
{
public:
    True_or_False_Question(int id,int type,QString title,QString A_Text,QString B_Text,QString Answer);
    QString getAText()
    {
        return A_Text;
    }
    QString getBText()
    {
        return B_Text;
    }
    QString getCText()
    {
        return "";
    }
    QString getDText()
    {
        return "";
    }
    ~True_or_False_Question();
protected:
    QString A_Text;
    QString B_Text;
};

#endif // QUESTION_H
