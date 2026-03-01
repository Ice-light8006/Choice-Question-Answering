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
#include<QLineEdit>
#include<QTableWidget>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    void ClearWidget();
    void InitializeTableWidget(int cnt);
    ~MainWidget();
private:
    QStackedWidget *qsw;
    QWidget *homepage;
    QWidget *answerpage;
    QDialog *importpage;
    QDialog *inputnumpage;
    QTextEdit *textedit;
    QLineEdit *inputedit;
    int sum_questions;
    int cur_question;
    int sum_WaitToBeAnswerQuestion;
    QVector<Question*> WaitToBeAnswerQuestions;
    QVector<Question*> QuestionStore;
    QVBoxLayout *Main_AnswerPage_Layout;
    QHBoxLayout *Button_AnswerPage_Layout;
    QHBoxLayout *Root_AnswerPage_Layout;
    QVBoxLayout *Inputnumpage_Layout;
    QString cur_answer;
    int cur_type;

    QPushButton *Next_Question;
    QPushButton *Submit;
    QPushButton *Return_Home;
    QPushButton *Previous_Question;

    QPushButton *exit;
    QAbstractButton *A;
    QAbstractButton *B;
    QAbstractButton *C;
    QAbstractButton *D;
    QLabel *QLabel_Title;

    QTableWidget *tableWidget;
    int row;
    int column;

private slots:
    void GoToAnswerPage();
    void GoToImportPage();
    void ImportQuestions();
    void CheckAnswer();
    void NextQuestion();
    void ClearQuestions();
    void PreviousQuestion();
    void ExitProgram();
    void RandomAnswer();
    void ImportNum();
    void ReturnHome();
    void GoToQuestion(int x);
};
#endif // WIDGET_H
