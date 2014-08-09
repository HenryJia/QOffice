#include "finddialog.h"

findDialog::findDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    connect(findNextButton, SIGNAL(clicked()), this, SLOT(findNextButton()));
    connect(findPreviousButton, SIGNAL(clicked()), this, SLOT(findPreviousButton()));
}

void findDialog::findNextButtonClicked()
{
    QString text = keyWordsLineEdit->text();
    if(text==tr(""))
        return;
    Qt::CaseSensitivity cs = matchCaseLineEdit->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit findNext(text, cs);
}
void findDialog::findPreviousButtonClicked()
{
    QString text = keyWordsLineEdit->text();
    if(text==tr(""))
        return;
    Qt::CaseSensitivity cs = matchCaseLineEdit->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit findPrevious(text, cs);
}
