#ifndef ANSWERPAGE_H
#define ANSWERPAGE_H

#include"Question.h"
#include"QuizManager.h"
#include<QWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QTableWidget>
#include<QAbstractButton>
#include<QVector>
#include<QLabel>

class AnswerPage :public QWidget
{
    Q_OBJECT

public:
    AnswerPage(QuizManager* quMag,QWidget* parent = nullptr);
    void InitializeTableWidget(int cnt);
    void ReChosenTableWidgetItem(int index);
    void BuildQuestion(Question* q);
    QAbstractButton* CreateAbstractBtn(int type,const QString &text,QWidget *parent);
    void ReTitleText(const QString &text);
    void ClearOptionsLayout();
    QPushButton* GetReturn_Home_Button();
    ~AnswerPage();
private:
    //答题页面的按钮
    QPushButton *Previous_Question_Button;
    QPushButton *Submit_Button;
    QPushButton *Return_Home_Button;
    QPushButton *Next_Question_Button;

    QPushButton *Exit;

    //选项数组
    QVector<QAbstractButton*> Options;

    //标题Label
    QLabel* Title;

    //题号按钮表格
    QTableWidget *TableWidget;

    //布局
    QVBoxLayout *Main_AnswerPage_Layout;
    QHBoxLayout *Button_AnswerPage_Layout;
    QHBoxLayout *Root_AnswerPage_Layout;
    QVBoxLayout *TableWidget_Layout;
    QVBoxLayout *TitleLabel_Layout;
    QVBoxLayout *Options_Layout;

    //题库管理器
    QuizManager* quizMag;

private slots:
    void CheckAnswer();
    void GoToPreviousQuestion();
    void GoToNextQuestion();

signals:
    void Previous_Question_Button_Clicked();
    void Submit_Button_Clicked();
    void Return_Home_Button_Clicked();
    void Next_Question_Button_Clicked();
    void InformMainWindowAdjustSize();
};

#endif // ANSWERPAGE_H
