#include"DatabaseSelectWidget.h"
#include<QSet>
#include<QString>
#include<QWidget>
#include<QInputDialog>
#include<QMessageBox>
#include<QListWidgetItem>
#include<QAbstractItemDelegate>
#include<QLineEdit>
#include<QEvent>
#include<QMouseEvent>
#include<QTimer>
#include<QMenu>
#include<QAction>

DatabaseSelectWidget::DatabaseSelectWidget(DatabaseManager* DbMag,QuestionsManager* QuMag,QWidget* parent)
    :QWidget(parent)
{
    this->DbMag = DbMag;
    this->QuMag = QuMag;
    this->listwidget = new MyListWidget(this);
    this->listwidgetlayout = new QVBoxLayout(this);

    //设置固定宽度
    this->listwidget->setFixedWidth(160);

    //设置右键菜单
    this->listwidget->setContextMenuPolicy(Qt::CustomContextMenu);

    this->adddatabase = new QToolButton(this);
    this->toolbarlayout = new QHBoxLayout();
    adddatabase->setText("+");
    this->setStyleSheet(R"(
    QToolButton {
    background-color: transparent;
    border: none;
    padding: 4px;
    border-radius: 6px;
    }
    QToolButton:hover {
    background-color: rgba(0, 0, 0, 30);
    }
    QToolButton:pressed {
    background-color: rgba(0, 0, 0, 60);
    }
    QToolButton:focus {
    outline: none;
    border: 1px solid rgba(0, 120, 215, 150);
    }
    )");
    this->toolbarlayout->addWidget(adddatabase);

    //加一个弹簧填补工具栏右边的空
    this->toolbarlayout->addStretch();

    //配置根布局
    listwidgetlayout->addLayout(toolbarlayout);
    listwidgetlayout->addWidget(listwidget);
    QSet<QString> AllConnect = DbMag->GetAllDatabaseConnectSet();
    for(QSet<QString>::Iterator it = AllConnect.begin();it!=AllConnect.end();it++)
    {
        QListWidgetItem* item = new QListWidgetItem(*it);
        this->Texts[item] = *it;
        this->listwidget->addItem(item);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    listwidget->setEditTriggers(QAbstractItemView::EditKeyPressed);

    //连接信号与槽
    connect(listwidget,&QListWidget::itemClicked,this,&DatabaseSelectWidget::TurnToDatabase);
    connect(adddatabase,&QToolButton::clicked,this,&DatabaseSelectWidget::AddDatabase);
    connect(listwidget,&QListWidget::itemChanged,this,&DatabaseSelectWidget::onListItemChanged);
    connect(listwidget,&QListWidget::customContextMenuRequested,this,&DatabaseSelectWidget::onListWidgetContextMenu);

}

void DatabaseSelectWidget::deleteItem(QListWidgetItem *item)
{
    delete item;
}



void DatabaseSelectWidget::TurnToDatabase(QListWidgetItem *Item)
{
    QString text = Item->text();
    qDebug() << "数据库" + text + "已被选中";
    DbMag->ReviseCurDatabaseConnect(text);
    this->DatabaseJustSwitched();
}

void DatabaseSelectWidget::AddDatabase()
{
    bool ok;
    //QInputDialog *inputdialog = new QInputDialog;
    QString Name = QInputDialog::getText(this,
                                         "输入",
                                         "请输入要创建的数据库的名称",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if(ok&&!Name.isEmpty())
    {
        if(DbMag->GetAllDatabaseConnectSet().contains(Name))
        {
            QMessageBox::critical(this, "警告", "数据库" + Name + "已存在");
            return;
        }

        //创建新的题库数据库
        DbMag->CreateQuestionsStore(Name);

        //在listwidget里添加新的item
        QListWidgetItem* Item = new QListWidgetItem(Name);
        Item->setFlags(Item->flags() | Qt::ItemIsEditable);
        this->listwidget->addItem(Item);


        //维护Item指针->Name的Map
        this->Texts[Item] = Name;

        //提示框
        QMessageBox::information(this,"提示","创建数据库"+Name+"成功");
    }
    else
    {
        //错误提示
        QMessageBox::critical(this,"警告","输入为空或者创建失败");
        qWarning() << "创建失败";
    }

}

void DatabaseSelectWidget::onListItemChanged(QListWidgetItem* Item)
{
    //临时阻塞listwidget的所有信号，防止无限递归
    QSignalBlocker blocker(listwidget);

    QString newName = Item->text();

    QString srcName = this->Texts[Item];

    if(newName==srcName)
    {
        qDebug() << "文本没有被修改";
        return;
    }

    if(DbMag->GetAllDatabaseConnectSet().contains(newName))
    {
        QMessageBox::critical(this,"提示","数据库"+newName+"已存在");
        return;
    }

    //从数据库修改名字
    if(DbMag->ReNameDatabase(srcName,newName))
    {
        qDebug() << "修改数据库"+srcName+"的名字为"+newName+"成功";

        this->Texts[Item] = newName;

        QMessageBox::information(this,"提示","修改数据库"+srcName+"的名字为"+newName+"成功");

    }
    else
    {
        qWarning() << "修改数据库"+srcName+"的名字为"+newName+"失败";
        QMessageBox::critical(this,"提示","修改数据库"+srcName+"的名字为"+newName+"失败");
        Item->setText(srcName);
    }

    //解决事件时序问题，防止修改名称之后如果是点击其它Item的方式结束的编辑文本状态而导致的ui显示的被选中的Item错误
    QTimer::singleShot(0, this, [=]() {
        listwidget->setCurrentItem(Item, QItemSelectionModel::ClearAndSelect);
    });

}

void DatabaseSelectWidget::onListWidgetContextMenu(const QPoint &pos)
{
    //获取鼠标点击处的Item
    QListWidgetItem* item = listwidget->itemAt(pos);

    if(!item)
    {
        return;
        //点空白处不出菜单
    }

    QMenu contextMenu(this);
    QAction* deleteAction = contextMenu.addAction("删除");
    QAction* exportAction = contextMenu.addAction("导出");

    //listwidget->viewport()->mapToGlobal(pos)的作用：
    //鼠标在列表控件内的局部坐标转换为屏幕全局坐标，目的是让右键菜单能准确显示在鼠标光标位置。
    QAction* selectedAction = contextMenu.exec(listwidget->viewport()->mapToGlobal(pos));

    if(selectedAction==deleteAction)
    {
        QString dbName = item->text();
        // 确认对话框
        if (QMessageBox::question(this, "确认", QString("确定要删除数据库 '%1' 吗？").arg(dbName)) == QMessageBox::Yes)
        {
            // 执行删除数据库文件逻辑
            this->doDeleteAnswerPage();
            this->doDeleteDatabaseAndFile();
            // 然后删除 item
            delete item;
        }
    }
    else if(selectedAction==exportAction)
    {
        QString dbName = item->text();
        this->exportDatabaseRequested(dbName);
    }
}

void DatabaseSelectWidget::addItemForListWidget(const QString &dbName)
{
    QListWidgetItem* item = new QListWidgetItem(dbName);
    this->listwidget->addItem(item);
}
