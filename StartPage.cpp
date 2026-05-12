#include"StartPage.h"

StartPage::StartPage(QWidget* parent)
    :QWidget(parent)
{
    this->StartPageLayout = new QHBoxLayout(this);
    this->StartText = new QLabel("请选择你的数据库");
    StartText->setStyleSheet(
        "color: rgba(64, 64, 64, 128);"   // 半透明黑色（0~255，128≈50%透明度）
        "font-weight: bold;"
        );
    this->StartPageLayout->addWidget(StartText);
    this->setLayout(StartPageLayout);
}
