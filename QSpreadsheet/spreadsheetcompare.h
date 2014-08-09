#ifndef SPREADSHEETCOMPARE_H
#define SPREADSHEETCOMPARE_H

class QStringList;

class spreadsheetCompare
{
public:
    bool operator()(const QStringList &row1, const QStringList &row2);

    enum { keyCount = 3};
    int keys[keyCount];
    bool ascending[keyCount];
};

#endif // SPREADSHEETCOMPARE_H
