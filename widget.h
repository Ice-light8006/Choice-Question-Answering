#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QStackedWidget>
#include<QTextEdit>
#include<QVector>
#include"question.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QAbstractButton>
#include<QLabel>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    void ClearWidget();
    ~MainWidget();
private:
    QStackedWidget *qsw;
    QWidget *homepage;
    QWidget *answerpage;
    QDialog *importpage;
    QTextEdit *textedit;
    int sum_questions;
    int cur_question;
    QVector<Question*> questions;
    QVBoxLayout *Main_AnswerPage_Layout;
    QHBoxLayout *Button_AnswerPage_Layout;
    QString cur_answer;
    int cur_type;
    QPushButton *Next_Question;
    QPushButton *Submit;
    QPushButton *Previous_Question;
    QPushButton *exit;
    QAbstractButton *A;
    QAbstractButton *B;
    QAbstractButton *C;
    QAbstractButton *D;
    QLabel *QLabel_Title;

private slots:
    void GoToAnswerPage();
    void GoToImportPage();
    void ImportQuestions();
    void CheckAnswer();
    void NextQuestion();
    void ClearQuestions();
    void PreviousQuestion();
    void ExitProgram();
};
#endif // WIDGET_H
