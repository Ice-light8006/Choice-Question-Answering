#include"ImportJsonPage.h"

ImportJsonPage::ImportJsonPage(QWidget* parent)
    :QDialog(parent)
{
    //构造导入题目的页面
    ImportPageLayoue = new QVBoxLayout(this);
    textedit = new QTextEdit(this);
    ImportPageLayoue->addWidget(textedit);
    Import_Question_Button = new QPushButton("导入题目",this);
    ImportPageLayoue->addWidget(Import_Question_Button);
    connect(Import_Question_Button,&QPushButton::clicked,this,&ImportJsonPage::Import_Question_Button_Clicked);
}

QString ImportJsonPage::GetImputText()
{
    QString JsonText = this->textedit->toPlainText();
    return JsonText;
}

ImportJsonPage::~ImportJsonPage()
{

}
