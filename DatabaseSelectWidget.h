#ifndef DATABASESELECTWIDGET_H
#define DATABASESELECTWIDGET_H
#include"DatabaseManager.h"
#include"QuestionsManager.h"
#include"MyListWidget.h"
#include<QListWidget>
#include<QVBoxLayout>
#include<QToolButton>
#include<QMap>

class DatabaseSelectWidget:public QWidget
{
    Q_OBJECT
public:
    DatabaseSelectWidget(DatabaseManager* DbMag,QuestionsManager* QuMag,QWidget* parent);
    void deleteItem(QListWidgetItem* item);
    void addItemForListWidget(const QString &dbName);
private:
    MyListWidget* listwidget;
    QVBoxLayout* listwidgetlayout;
    QHBoxLayout* toolbarlayout;
    DatabaseManager* DbMag;
    QuestionsManager* QuMag;
    QToolButton *adddatabase;
    QMap<QListWidgetItem*,QString> Texts;
private slots:
    void TurnToDatabase(QListWidgetItem* Item);
    void AddDatabase();
    void onListItemChanged(QListWidgetItem* Item);
    void onListWidgetContextMenu(const QPoint &pos);

signals:
    void DatabaseJustSwitched();
    void doDeleteAnswerPage();
    void doDeleteDatabaseAndFile();
    void exportDatabaseRequested(const QString &dbName);
};

#endif // DATABASESELECTWIDGET_H
