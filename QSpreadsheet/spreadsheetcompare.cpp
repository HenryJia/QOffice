#include <QStringList>

#include "spreadsheetcompare.h"

bool spreadsheetCompare::operator()(const QStringList &row1, const QStringList &row2)
{
    for(int i = 0; i < keyCount; i++)
    {
        int column = keys[i];
        if(column != -1)
        {
            if(row1[column] !=  row2[column])
            {
                if(ascending[i])
                    return row1[column] < row2[column];
                else
                    return row1[column] > row2[column];
            }
        }
    }
    return false;
}
