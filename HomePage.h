#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include<QWidget>
#include<QPushButton>
#include<QVBoxLayout>
#include<QLabel>

class HomePage :public QWidget
{
    Q_OBJECT

public:
    HomePage(QWidget* parent = nullptr);
    void setQuestionCnt(int cnt);
    ~HomePage();
private:
    QLabel* CurChosenDatabase;
    QLabel* questionCnt;
    QPushButton *Start_Answer_Button;
    QPushButton *Random_Answer_Button;
    QPushButton *importQuestionFromJsonText;
    QVBoxLayout *HomePageLayout;

private slots:
    void Exit();

signals:
    void Start_Answer_Button_Clicked();//含义：HomePage可以发出一个叫Start_Answer_Button_Clicked的信号
    void Random_Answer_Button_Clicked();
    void importQuestionFromTextButtonClicked();
};

#endif // HOMEPAGE_H
