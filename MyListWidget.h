#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QLineEdit>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    using QListWidget::QListWidget;

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (isEditing())
            return;

        QListWidget::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (isEditing())
            return;

        QListWidget::mouseReleaseEvent(event);
    }

    void selectionChanged(const QItemSelection &selected,
                          const QItemSelection &deselected) override
    {
        if (isEditing())
            return;

        QListWidget::selectionChanged(selected, deselected);
    }

private:
    bool isEditing()
    {
        QWidget* fw = QApplication::focusWidget();
        return qobject_cast<QLineEdit*>(fw) != nullptr;
    }
};

#endif // MYLISTWIDGET_H
