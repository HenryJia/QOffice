#include "findandreplacewidget.h"

findAndReplaceWidget::findAndReplaceWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(findNextButton, SIGNAL(clicked()), this, SLOT(findNextButtonClicked()));
    connect(findPreviousButton, SIGNAL(clicked()), this, SLOT(findPreviousButtonClicked()));
}

void findAndReplaceWidget::findNextButtonClicked()
{
    QString text = keyWordsLineEdit->text();
    if(text==tr(""))
        return;
    Qt::CaseSensitivity cs = caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit findNext(text, cs);
}

void findAndReplaceWidget::findPreviousButtonClicked()
{
    QString text = keyWordsLineEdit->text();
    if(text==tr(""))
        return;
    Qt::CaseSensitivity cs = caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit findPrevious(text, cs);
}

