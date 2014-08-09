#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>

#include "ui_sortdialog.h"

class sortDialog : public QDialog, public Ui::sortDialog
{
    Q_OBJECT

public:
    sortDialog(QWidget *parent = 0);

    void setColumnRange(QChar first, QChar last);
};

#endif // SORTDIALOG_H
