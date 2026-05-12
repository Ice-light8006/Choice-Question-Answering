#include"QuizManager.h"
#include<QDebug>
#include<algorithm>
#include<QRandomGenerator>

QuizManager::QuizManager(QuestionsManager *quMag)
{
    this->quMag = quMag;
}

Question* QuizManager::getQuestionFromWaitQuestion(int cnt)
{
    return this->WaitToBeAnswerQuestions[cnt];
}

int QuizManager::getWaitQuestionCount()
{
    return (int)(this->WaitToBeAnswerQuestions.size());
}

Question* QuizManager::getCurQuestion()
{
    return this->curQuestion;
}

QVector<Question *> QuizManager::getWaitToBeAnswerQuestions()
{
    return this->WaitToBeAnswerQuestions;
}

void QuizManager::clearWaitToBeAnswerQuestions()
{
    this->WaitToBeAnswerQuestions.clear();
}

void QuizManager::randomGetQuestions(int cnt)
{
    //获取总题库数组
    QVector<Question*> temp = quMag->getQuestionStore();

    //获取总题库数组的长度
    int len = temp.size();

    //索引数组
    QVector<int> index;

    //设置长度
    index.resize(len);

    //生成索引
    for(int i = 0;i<len;i++)
    {
        index[i] = i;
    }

    //把索引随机打乱排序
    std::shuffle(index.begin(),index.end(),*QRandomGenerator::global());

    //取前cnt个
    index.resize(cnt);

    //排序，保证随机抽出的题目的相对顺序不改变
    std::sort(index.begin(),index.end());

    //提取出抽取到的索引对应的题目
    QVector<Question*> Questions;
    for(int i = 0;i<cnt;i++)
    {
        Questions.append(temp[index[i]]);
    }

    if(this->WaitToBeAnswerQuestions.size())
    {
        this->WaitToBeAnswerQuestions.clear();
    }

    this->WaitToBeAnswerQuestions = Questions;
}

void QuizManager::reviseCurQuestion(Question *q)
{
    this->curQuestion = q;
}

int QuizManager::getCurIndex()
{
    return this->curIndex;
}

void QuizManager::reviseCurIndex(int index)
{
    this->curIndex = index;
}

void QuizManager::copyStoreToWait()
{
    this->WaitToBeAnswerQuestions = quMag->getQuestionStore();
    qDebug()<< "QuMag:把题库复制到等待答题题库成功";
}
