#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

#include "spreadsheet.h"
#include "spreadsheetcompare.h"
#include "cell.h"

Spreadsheet::Spreadsheet(QWidget *parent) : QTableWidget(parent)
{
    autoRecalc = true;
    lastSearchRow = -1;
    lastSearchColumn = -1;
    setItemPrototype(new Cell);
    setSelectionMode(ContiguousSelection);

    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(somethingChanged()));

    clear();
}

void Spreadsheet::clear()
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(rowCount);
    setColumnCount(columnCount);

    for(int i = 0; i < columnCount; i++)
    {
        QTableWidgetItem * item = new QTableWidgetItem;
        item->setText(QString(QChar('A' + i)));
        setHorizontalHeaderItem(i, item);
    }

    setCurrentCell(0, 0);
}

Cell *Spreadsheet::cell(int row, int column) const
{
    return static_cast<Cell *>(item(row, column));
}

QString Spreadsheet::text(int row, int column) const
{
    Cell *c = cell(row, column);
    if(c)
        return c->text();
    else
        return "";
}

QString Spreadsheet::formula(int row, int column) const
{
    Cell *c = cell(row, column);
    if(c)
        return c->formula();
    else
        return "";
}

void Spreadsheet::setFormula(int row, int column, const QString &formula)
{
    Cell *c = cell(row, column);
    if(!c)
    {
        c = new Cell;
        setItem(row, column, c);
    }
    c->setFormula(formula);
}

QString Spreadsheet::currentLocation() const
{
    return QChar('A' + currentColumn()) + QString::number(currentRow() + 1);
}

QString Spreadsheet::currentFormula() const
{
    return formula(currentRow(), currentColumn());
}

void Spreadsheet::somethingChanged()
{
    if (autoRecalc)
        recalculate();
    emit modified();
}

bool Spreadsheet::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("QSpreadsheet"), tr("Cannot read file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);

    quint32 magic;
    in >> magic;
    if(magic != magicNumber)
    {
        QMessageBox::warning(this, tr("QSpreadsheet"), tr("This is not a QSpreadSheet file"));
        return false;
    }

    clear();

    quint16 row;
    quint16 column;
    QString str;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd())
    {
        in >> row >> column >> str;
        setFormula(row, column, str);
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool Spreadsheet::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("QSpreadsheet"), tr("Cannot write file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint32(magicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row = 0; row < rowCount; row++)
        for(int column = 0; column < columnCount; column++)
        {
            QString str = formula(row, column);
            if(!str .isEmpty())
                out << quint16(row) << quint16(column) << str;
        }
    QApplication::restoreOverrideCursor();
    return true;
}

void Spreadsheet::cut()
{
    copy();
    del();
}

void Spreadsheet::copy()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str;

    for(int i = 0; i < range.rowCount(); i++)
    {
        if(i > 0)
            str +=  "\n";
        for(int j = 0; j < range.columnCount(); j++)
        {
            if(j > 0)
                str += "\t";
            str += formula(range.topRow() + i, range.leftColumn() + j);
        }
    }
    QApplication::clipboard()->setText(str);
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if(ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}

void Spreadsheet::paste()
{
    QTableWidgetSelectionRange range = selectedRange();
    QString str = QApplication::clipboard()->text();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t') + 1;

    if(range.rowCount() * range.columnCount() != 1 && (range.rowCount() != numRows || range.columnCount() != numColumns))
    {
        QMessageBox::information(this, tr("QSpreadsheet"), tr("The information cannot be pasted because the copy and paste areas are not the same size"));
        return;
    }
    for(int i = 0; i < numRows; i++)
    {
        QStringList columns = rows[i].split('\t');
        for(int j = 0; j < numColumns; j++)
        {
            int row = range.topRow() + i;
            int column = range.leftColumn() + j;
            if(row < rowCount && column < columnCount)
                setFormula(row, column, columns[j]);
        }
    }
    somethingChanged();
}

void Spreadsheet::del()
{
    QList<QTableWidgetItem *> items = selectedItems();
    if(!items.isEmpty())
    {
        foreach (QTableWidgetItem *item, items)
            delete item;
        somethingChanged();
    }
}

void Spreadsheet::selectCurrentRow()
{
    selectRow(currentRow());
}

void Spreadsheet::selectCurrentColumn()
{
    selectRow(currentColumn());
}

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row, column;
    restart:
    row = lastSearchRow == -1 ? currentRow() : lastSearchRow;
    column = lastSearchColumn == -1 ? currentColumn() : lastSearchColumn;
    while(row < rowCount)
    {
        while(column < columnCount)
        {
            if(text(row, column).contains(str, cs))
            {
                clearSelection();
                setCurrentCell(row, column);
                lastSearchRow = row;
                lastSearchColumn = column + 1;
                activateWindow();
                return;
            }
            column++;
        }
        column = 0;
        row++;
    }
    QApplication::beep();
    int r = QMessageBox::warning(this, tr("QSpreadsheet"), tr("Bottom of spreadsheet reached. Restart searching from top?"), QMessageBox::Yes | QMessageBox::No);
    if(r == QMessageBox::Yes)
    {
        lastSearchRow = 0;
        lastSearchColumn = 0;
        goto restart;
    }
}

void Spreadsheet::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    int row, column;
    restart:
    row = lastSearchRow == -1 ? currentRow() : lastSearchRow;
    column = lastSearchColumn == -1 ? currentColumn() : lastSearchColumn;
    while(row >= 0)
    {
        while(column >= 0)
        {
            if(text(row, column).contains(str, cs))
            {
                clearSelection();
                setCurrentCell(row, column);
                lastSearchRow = row;
                lastSearchColumn = column == 0 ? -2 : column - 1;
                activateWindow();
                return;
            }
            column--;
        }
        column = columnCount - 1;
        row--;
    }
    QApplication::beep();
    int r = QMessageBox::warning(this, tr("QSpreadsheet"), tr("Top of spreadsheet reached. Restart searching from bottom?"), QMessageBox::Yes | QMessageBox::No);
    if(r == QMessageBox::Yes)
    {
        lastSearchRow = rowCount - 1;
        lastSearchColumn = columnCount - 1;
        goto restart;
    }
}

void Spreadsheet::replace(const QString &replaceStr, const QString &keyWords, Qt::CaseSensitivity cs)
{
    if(!text(currentRow(), currentColumn()).contains(keyWords, cs))
        findNext(keyWords, cs);
    QString str = cell(currentRow(), currentColumn())->data(Qt::EditRole).toString();
    str.replace(keyWords, replaceStr, cs);
    cell(currentRow(), currentColumn())->setData(Qt::EditRole, str);
}

void Spreadsheet::replaceAll(const QString &replaceStr, const QString &keyWords, Qt::CaseSensitivity cs)
{
    for(int row = 0; row < rowCount; row++)
    {
        for(int column = 0; column < columnCount; column++)
        {
            if(text(row, column).contains(keyWords, cs))
            {
                QString str = cell(row, column)->data(Qt::EditRole).toString();
                str.replace(keyWords, replaceStr, cs);
                cell(row, column)->setData(Qt::EditRole, str);
                setCurrentItem(cell(row, column));
            }
        }
    }
}

void Spreadsheet::recalculate()
{
    for(int row = 0; row < rowCount; row++)
        for(int column = 0; column < columnCount; column++)
            if(cell(row, column))
                cell(row, column)->setRecalcNeeded();
    viewport()->update();
}

void Spreadsheet::setAutoRecalculate(bool recalc)
{
    autoRecalc = recalc;
    if(autoRecalc)
        recalculate();
}

void Spreadsheet::sort(const spreadsheetCompare &compare)
{
    QList<QStringList> rows;
    QTableWidgetSelectionRange range = selectedRange();
    int i;
    for(i = 0; i < range.rowCount(); i++)
    {
        QStringList row;
        for(int j = 0; j < range.columnCount(); j++)
            row.append(formula(range.topRow() + i, range.leftColumn() + j));
        rows.append(row);
    }

    qStableSort(rows.begin(), rows.end(), compare);

    for(i = 0; i < range.rowCount(); i++)
        for(int j = 0; j < range.columnCount(); j++)
            setFormula(range.topRow() + i, range.leftColumn() + j, rows[i][j]);
    clearSelection();
    somethingChanged();
}
