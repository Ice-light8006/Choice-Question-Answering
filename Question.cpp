#include"Question.h"


Question::Question(int id,int type,QString title,int count,QString Answer,int ImageCount,
                   QVector<std::pair<QString,QString>> Options,QVector<QString> ImageUrls)
{
    this->title = title;
    this->id = id;
    this->type = type;
    this->Answer = Answer;
    this->count = count;
    this->ImageCount = ImageCount;
    this->imageUrls = ImageUrls;
    this->Options = Options;
}

Question::~Question()
{

}

