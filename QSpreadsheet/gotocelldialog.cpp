#include <QButtonGroup>
#include <QPushButton>

#include "gotocelldialog.h"

goToCellDialog::goToCellDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    QRegExp regExp ("[A-Za-z][1-9][0-9]{0,2}");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onLineEditTextChanged()));
}

void goToCellDialog::onLineEditTextChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(lineEdit->hasAcceptableInput());
}
