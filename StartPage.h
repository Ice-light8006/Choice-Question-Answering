#ifndef STARTPAGE_H
#define STARTPAGE_H
#include<QWidget>
#include<QHBoxLayout>
#include<QLabel>

class StartPage:public QWidget
{
    Q_OBJECT
public:
    StartPage(QWidget* parent);
private:
    QHBoxLayout* StartPageLayout;
    QLabel* StartText;
};

#endif // STARTPAGE_H
