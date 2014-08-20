#include "findandreplacewidget.h"

findAndReplaceWidget::findAndReplaceWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(findNextButton, SIGNAL(clicked()), this, SLOT(findNextButtonClicked()));
    connect(findPreviousButton, SIGNAL(clicked()), this, SLOT(findPreviousButtonClicked()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(replaceButtonClicked()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(replaceAllButtonClicked()));
}

void findAndReplaceWidget::findNextButtonClicked()
{
    QString text = keyWordsLineEdit->text();
    if(text == tr(""))
        return;
    Qt::CaseSensitivity cs = caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit findNext(text, cs);
}

void findAndReplaceWidget::findPreviousButtonClicked()
{
    QString text = keyWordsLineEdit->text();
    if(text == tr(""))
        return;
    Qt::CaseSensitivity cs = caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit findPrevious(text, cs);
}

void findAndReplaceWidget::replaceButtonClicked()
{
    QString keyWords = keyWordsLineEdit->text();
    QString replaceStr = replaceLineEdit->text();
    if(keyWords == tr(""))
        return;
    Qt::CaseSensitivity cs = caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit replace(replaceStr, keyWords, cs);
}

void findAndReplaceWidget::replaceAllButtonClicked()
{
    QString keyWords = keyWordsLineEdit->text();
    QString replaceStr = replaceLineEdit->text();
    if(keyWords == tr(""))
        return;
    Qt::CaseSensitivity cs = caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit replaceAll(replaceStr, keyWords, cs);
}
