#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

class Cell : public QTableWidgetItem
{
public:
    Cell();

    QTableWidgetItem *clone() const;
    void setData(int role, const QVariant &value);
    QVariant data(int role) const;
    void setFormula(const QString &formula);
    QString formula() const;
    void setRecalcNeeded();

private:
    QVariant value() const;
    QVariant evalExpression(const QString &str, int pos) const;
    QVariant evalTerm(const QString &str, int &pos) const;
    QVariant evalFactor(const QString &str, int &pos) const;
    const QVariant Invalid;

    mutable QVariant cachedValue;
    mutable bool cachedRecalcNeeded;
};

#endif // CELL_H
