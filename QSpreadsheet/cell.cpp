#include <QStringList>
#include <vector>

#include "cell.h"

using namespace std;

Cell::Cell()
{
    setRecalcNeeded();
}

QTableWidgetItem *Cell::clone() const
{
    return new Cell(*this);
}

void Cell::setFormula(const QString &formula)
{
    setData(Qt::EditRole, formula);
}

QString Cell::formula() const
{
    return data(Qt::EditRole).toString();
}

void Cell::setData(int role, const QVariant &value)
{
    QTableWidgetItem::setData(role, value);
    if(role == Qt::EditRole)
        setRecalcNeeded();
}

void Cell::setRecalcNeeded()
{
    cachedRecalcNeeded = true;
}

QVariant Cell::data(int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(value().isValid())
            return value().toString();
        else
            return "####";
    }
    else if(role == Qt::TextAlignmentRole)
    {
        if(value().type() == QVariant::String)
            return int(Qt::AlignLeft | Qt::AlignCenter);
        else
            return int(Qt::AlignRight | Qt::AlignCenter);
    }
    else
        return QTableWidgetItem::data(role);
}

QVariant Cell::value() const
{
    if(cachedRecalcNeeded)
    {
        cachedRecalcNeeded = false;
        QString formulaStr = formula();
        if(formulaStr.startsWith('\''))
            cachedValue = formulaStr.mid(1);
        else if(formulaStr.startsWith('='))
        {
            cachedValue = Invalid;
            QString expr = formulaStr.mid(1);
            expr.replace(" ", "");
            expr.append(QChar::Null);
            cachedValue = evalExpression(expr, 0);
            //if(expr[pos] != QChar::Null)
            //    cachedValue = Invalid;
        }
        else
        {
            bool ok;
            double d = formulaStr.toDouble(&ok);
            if(ok)
                cachedValue = d;
            else
                cachedValue = formulaStr;
        }
    }
    return cachedValue;
}

//Need to edit this to use C++11

QVariant Cell::evalExpression(QString str, int pos) const
{
    vector <QVariant> variables;
    vector <QChar> operators;
    vector <int> leftParenthesisPos;
    QString *number = new QString;
    QString *parenthesisString = new QString;
    //int parenthesisPos = pos;
    int tempPos = pos;

    // Deal with any parenthesis
    for(int i = pos; str[tempPos] != QChar::Null && i < str.size(); i++)
    {
        if(str[i] == '(')
            leftParenthesisPos.push_back(i);
        else if(str[i] == ')')
        {
            // This does not work yet if str starts with a bracket. Reasons not known yet.
            for(int j = leftParenthesisPos.back() + 1; j < i; j++)
                parenthesisString->append(str[j]);
            str.replace(*parenthesisString, QString::number(evalExpression(*parenthesisString, 0).toDouble()));
            parenthesisString->insert(0, '(');
            parenthesisString->append(')');
            leftParenthesisPos.pop_back();
            delete parenthesisString;
            parenthesisString = new QString;
        }
    }

    // Find All the digits of the numbers
    for(; str[tempPos] != QChar::Null; tempPos++)
    {
        if(str[tempPos].isNumber() || str[pos] == '.')
            number->append(str[tempPos]);
        else
        {
            // We have found the end of the number, so we convert the string
            // of digits into a double and push it on to the variable stack
            // We then delete the number and move on to finding the next number
            if(number->size() > 0)
            {
                variables.push_back(number->toDouble());
                delete number;
                number = new QString;
            }
            // We then push all the operators and parenthesis on to a separate stack
                operators.push_back(str[tempPos]);
        }
        //else
            //return Invalid;
    }
    if(number->size() != 0)
        variables.push_back(number->toDouble());
    delete number;
    // Perform calculations
    tempPos = pos;
    for(int i = 0; i < operators.size();  )
    {
        if(operators.at(i) == '/')
        {
            if(variables.at(i + 1) == 0)
                return Invalid;
            variables.at(i) = variables.at(i).toDouble() / variables.at(i + 1).toDouble();
            variables.erase(variables.begin() +  (i + 1));
            operators.erase(operators.begin() +  i);
        }
        else
            i++;
    }
    for(int i = 0; i < operators.size(); )
    {
        if(operators.at(i) == '*')
        {
            variables.at(i) = variables.at(i).toDouble() * variables.at(i + 1).toDouble();
            variables.erase(variables.begin() +  i + 1);
            operators.erase(operators.begin() +  i);
        }
        else
            i++;
    }
    for(int i = 0; i < operators.size(); )
    {
        if(operators.at(i) == '+')
        {
            variables.at(i) = variables.at(i).toDouble() + variables.at(i + 1).toDouble();
            variables.erase(variables.begin() +  i + 1);
            operators.erase(operators.begin() +  i);
        }
        else if(operators.at(i) == '-')
        {
            variables.at(i) = variables.at(i).toDouble() + variables.at(i - 1).toDouble();
            variables.erase(variables.begin() +  i + 1);
            operators.erase(operators.begin() +  i);
        }
        else
            i++;
    }
    return variables.front();
}
