#ifndef FINDANDREPLACEWIDGET_H
#define FINDANDREPLACEWIDGET_H

#include <QDockWidget>

#include "ui_findandreplacewidget.h"

class findAndReplaceWidget : public QDockWidget, public Ui::findAndReplaceWidget
{
    Q_OBJECT

public:
    findAndReplaceWidget(QWidget *parent = 0);
signals:
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);
    void replace(const QString& replaceStr, const QString& keyWords, Qt::CaseSensitivity);
    void replaceAll(const QString& replaceStr, const QString& keyWords, Qt::CaseSensitivity);
private slots:
    void findNextButtonClicked();
    void findPreviousButtonClicked();
    void replaceButtonClicked();
    void replaceAllButtonClicked();
};

#endif // FINDANDREPLACEWIDGET_H
