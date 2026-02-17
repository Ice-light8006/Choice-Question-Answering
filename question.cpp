#include"question.h"

Question::Question(int id,int type,QString title)
{
    this->title = title;
    this->id = id;
}

Question::~Question()
{

}

Single_Question::Single_Question(int id, int type,QString title,QString A_Text,QString B_Text,QString C_Text,QString D_Text,QString Answer):Question(id,type,title)
{
    this->type = 0;
    this->A_Text = A_Text;
    this->B_Text = B_Text;
    this->C_Text = C_Text;
    this->D_Text = D_Text;
    this->Answer = Answer;
    this->title = title;
}

Mutiply_Question::Mutiply_Question(int id,int type,QString title,QString A_Text,QString B_Text,QString C_Text,QString D_Text,QString Answer):Question(id,type,title)
{
    this->type = 1;
    this->A_Text = A_Text;
    this->B_Text = B_Text;
    this->C_Text = C_Text;
    this->D_Text = D_Text;
    this->Answer = Answer;
    this->title = title;
}

True_or_False_Question::True_or_False_Question(int id,int type,QString title,QString A_Text,QString B_Text,QString Answer):Question(id,type,title)
{
    this->A_Text = A_Text;
    this->B_Text = B_Text;
    this->Answer = Answer;
    this->title = title;
    this->type = 2;
}

Single_Question::~Single_Question(){}

Mutiply_Question::~Mutiply_Question(){}

True_or_False_Question::~True_or_False_Question(){}

