#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>
#include "ui_gotocelldialog.h"

class QPushButton;

class goToCellDialog : public QDialog, public Ui::goToCellDialog
{
    Q_OBJECT

public:
    goToCellDialog(QWidget *parent=0);

private slots:
    void onLineEditTextChanged();
};

#endif // GOTOCELLDIALOG_H
