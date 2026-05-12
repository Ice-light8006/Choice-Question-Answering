#ifndef MENUBAR_H
#define MENUBAR_H
#include<QMenuBar>
#include<QMenu>
#include<QAction>

class MenuBar: public QMenuBar
{
    Q_OBJECT
public:
    MenuBar(QWidget* parent = nullptr);
    ~MenuBar();
private:
    QMenu *FileMenu;
signals:
    void exportActionTrigger();
    void importQuestionsRequested();
};

#endif // MENUBAR_H
