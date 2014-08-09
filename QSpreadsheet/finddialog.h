#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

#include "ui_finddialog.h"

class findDialog : public QDialog, public Ui::findDialog
{
    Q_OBJECT

public:
    findDialog(QWidget *parent = 0);
signals:
    void findNext(const QString& str, Qt::CaseSensitivity cs);
    void findPrevious(const QString& str, Qt::CaseSensitivity cs);
private slots:
    void findNextButtonClicked();
    void findPreviousButtonClicked();
};

#endif // FINDDIALOG_H
