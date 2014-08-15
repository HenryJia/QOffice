#ifndef FINDANDREPLACEWIDGET_H
#define FINDANDREPLACEWIDGET_H

#include <QWidget>

#include "ui_findandreplacewidget.h"

class findAndReplaceWidget : public QWidget, public Ui::findAndReplaceWidget
{
    Q_OBJECT

public:
    findAndReplaceWidget(QWidget *parent = 0);
signals:
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);
private slots:
    void findNextButtonClicked();
    void findPreviousButtonClicked();
};

#endif // FINDANDREPLACEWIDGET_H
