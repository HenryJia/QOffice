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

QVariant Cell::evalExpression(const QString &str, int pos) const
{
    vector <QVariant> variables;
    vector <QString> operators;
    QString *number = new QString;
    QString parenthesis;
    int parenthesisPos = pos;
    int tempPos = pos;
    // Find Aall the digits of the numbers
    for(; str[tempPos] != QChar::Null; tempPos++)
    {
        if(str[tempPos].isNumber() || str[pos] == '.')
            number->append(str[tempPos]);
        else //if(number->size() != 0)
        {
            // We have found the end of the number, so we convert the string
            // of digits into a double and push it on to the variable stack
            // We then delete the number and move on to finding the next number
            variables.push_back(number->toDouble());
            delete number;
            number = new QString;
            // We then push all the operators and parenthesis on to a separate stack
            if(str[tempPos] == '(')
            {
                operators.push_back(str[tempPos]);
                operators.back().append(str[tempPos]);
            }
            else if(str[tempPos - 1] == ')')
                operators.back().append(str[tempPos]);
            else
                operators.push_back(str[tempPos]);
        }
        //else
            //return Invalid;
    }
    variables.push_back(number->toDouble());
    delete number;
    // Find Parenthesis And Loop The Function To Calculate All Values Within The Parenthesis
    tempPos = pos;
    while(str[tempPos] != QChar::Null)
    {
        if(str[tempPos] == '(')
        {
            // First we separate the expression in the Parenthesis as a
            // separate string and pass it to this function.
            tempPos++;
            while(str[tempPos] != QChar::Null)
            {
                if(str[tempPos] == ')')
                    break;
                parenthesis.append(str[tempPos]);
                tempPos++;
            }
            // Then we remove the values within the parenthesis apart from the first one which we set to the value of the parenthesis
            // We also remove the parenthesis
            for(; !operators.at(parenthesisPos).contains('('); parenthesisPos++);
            operators.at(parenthesisPos).chop(1);
            parenthesisPos++;
            variables.at(parenthesisPos) = evalExpression(parenthesis, 0);
            operators.erase(operators.begin() +  parenthesisPos);
            parenthesisPos++;
            for(; !operators.at(parenthesisPos - 1).contains(')'); parenthesisPos++)
            {
                variables.erase(variables.begin() + parenthesisPos);
                if(!operators.at(parenthesisPos).contains(')'))
                    operators.erase(operators.begin() +  parenthesisPos);
            }
            operators.at(parenthesisPos -  1).remove(0,0);
        }
        tempPos++;
    }
    for(int i = 0; i < operators.size(); i++)
    {
        if(operators.at(i) == "/")
        {
            if(variables.at(i + 1) == 0)
                return Invalid;
            variables.at(i) = variables.at(i).toDouble() / variables.at(i + 1).toDouble();
            variables.erase(variables.begin() +  (i + 1));
            operators.erase(operators.begin() +  i);
        }
    }
    for(int i = 0; i < operators.size(); i++)
    {
        if(operators.at(i) == "*")
        {
            variables.at(i) = variables.at(i).toDouble() * variables.at(i + 1).toDouble();
            variables.erase(variables.begin() +  i + 1);
            operators.erase(operators.begin() +  i);
        }
    }
    for(int i = 0; i < operators.size(); i++)
    {
        if(operators.at(i) == "+")
        {
            variables.at(i) = variables.at(i).toDouble() + variables.at(i + 1).toDouble();
            variables.erase(variables.begin() +  i + 1);
            operators.erase(operators.begin() +  i);
        }
        else if(operators.at(i) == "-")
        {
            variables.at(i) = variables.at(i).toDouble() + variables.at(i - 1).toDouble();
            variables.erase(variables.begin() +  i + 1);
            operators.erase(operators.begin() +  i);
        }
    }
    return variables.front();
}

QVariant Cell::evalTerm(const QString &str, int &pos) const
{
    QVariant result = evalFactor(str, pos);
    QVariant finalResult = result;
    while(str[pos] != QChar::Null)
    {
        QChar op = str[pos];
        if(op != '*' && op != '/')
            return result;
        pos++;

        QVariant factor = evalFactor(str, pos);
        if(result.type() == QVariant::Double && factor.type() == QVariant::Double)
        {
            if(op == '*')
                finalResult = result.toDouble() * factor.toDouble();
            else
            {
                if(factor.toDouble() == 0.0)
                    result = Invalid;
                else
                    finalResult = result.toDouble() - factor.toDouble();
            }
        }
        else
            result = Invalid;
    }
    return finalResult;
}

QVariant Cell::evalFactor(const QString &str, int &pos) const
{
    QVariant result;
    bool negative = false;

    if(str[pos] == '-')
    {
        negative = true;
        pos++;
    }

    if(str[pos] == '(')
    {
        pos++;
        result = evalExpression(str, pos);
        if(str[pos] != ')')
            result = Invalid;
    }
    else
    {
        QRegExp regExp("[A-Za-z][1-9][0-9][0,2]");
        QString token;

        while(str[pos].isLetterOrNumber() || str[pos] == '.')
        {
            token += str[pos];
            pos++;
        }
        if(regExp.exactMatch(token))
        {
            int column = token[0].toUpper().unicode() - 'A';
            int row = token.mid(1).toInt() - 1;

            Cell *c = static_cast<Cell *>(tableWidget()->item(row, column));

            if(c)
                result = c->value();
            else
                result = 0.0;
        }
        else
        {
            bool ok;
            result = token.toDouble(&ok);
            if(!ok)
                result = Invalid;
        }
    }
    if(negative)
    {
        if(result.type() == QVariant::Double)
            result = -result.toDouble();
        else
            result = Invalid;
    }
    return result;
}
