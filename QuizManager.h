#ifndef QUIZMANAGER_H
#define QUIZMANAGER_H
#include"Question.h"
#include"QuestionsManager.h"

class QuizManager
{
public:
    QuizManager(QuestionsManager* quMag);
    Question* getQuestionFromWaitQuestion(int cnt);
    int getWaitQuestionCount();
    Question* getCurQuestion();
    QVector<Question*> getWaitToBeAnswerQuestions();
    void clearWaitToBeAnswerQuestions();
    void randomGetQuestions(int cnt);
    void reviseCurQuestion(Question* q);
    int getCurIndex();
    void reviseCurIndex(int index);
    void copyStoreToWait();
private:
    QuestionsManager* quMag;
    QVector<Question*> WaitToBeAnswerQuestions;
    Question* curQuestion;
    int curIndex;
};

#endif // QUIZMANAGER_H
