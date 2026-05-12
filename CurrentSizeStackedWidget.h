#ifndef CURRENTSIZESTACKEDWIDGET_H
#define CURRENTSIZESTACKEDWIDGET_H

#include <QStackedWidget>

class CurrentSizeStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    using QStackedWidget::QStackedWidget;

    // 关键：返回当前页面的 sizeHint，而不是所有页面的最大值
    QSize sizeHint() const override {
        QWidget* cur = currentWidget();
        if (cur)
            return cur->sizeHint();
        return QStackedWidget::sizeHint();
    }

    QSize minimumSizeHint() const override {
        QWidget* cur = currentWidget();
        if (cur)
            return cur->minimumSizeHint();
        return QStackedWidget::minimumSizeHint();
    }
};

#endif // CURRENTSIZESTACKEDWIDGET_H
