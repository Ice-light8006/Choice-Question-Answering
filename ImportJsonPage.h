#ifndef IMPORTJSONPAGE_H
#define IMPORTJSONPAGE_H
#include<QDialog>
#include<QPushButton>
#include<QVBoxLayout>
#include<QTextEdit>

class ImportJsonPage:public QDialog
{
    Q_OBJECT
public:
    ImportJsonPage(QWidget* parent = nullptr);
    QString GetImputText();
    ~ImportJsonPage();
private:
    QPushButton* Import_Question_Button;
    QVBoxLayout* ImportPageLayoue;
    QTextEdit *textedit;
signals:
    void Import_Question_Button_Clicked();
};

#endif // IMPORTJSONPAGE_H
