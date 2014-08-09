#include <QAction>
#include <QLabel>
#include <QString>
#include <QMenuBar>
#include <QArgument>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QApplication>
#include <QMutableStringListIterator>
#include <QTableWidgetSelectionRange>
#include <QSettings>

#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
#include "spreadsheet.h"
#include "spreadsheetcompare.h"
#include "mainwindow.h"

QStringList mainWindow::recentFiles;

mainWindow::mainWindow()
{
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    searchDialog = 0;

    setWindowIcon(QIcon("images/icon.png"));
    setCurrentFile("");
}

void mainWindow::createActions()
{
    newFileAction = new QAction(tr("&New"), this);
    newFileAction->setIcon(QIcon("images/new.png"));
    newFileAction->setShortcut(QKeySequence::New);
    newFileAction->setStatusTip(tr("Create a new Spreadsheet file."));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openFileAction = new QAction(tr("&Open"), this);
    openFileAction->setIcon(QIcon("images/open.png"));
    openFileAction->setShortcut(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Open a Spreadsheet file."));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveFileAction = new QAction(tr("&Save"), this);
    saveFileAction->setIcon(QIcon("images/save.png"));
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAction->setStatusTip(tr("Save the current Spreadsheet file."));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsFileAction = new QAction(tr("&Save As"), this);
    saveAsFileAction->setIcon(QIcon("images/saveas.png"));
    saveAsFileAction->setShortcut(QKeySequence::SaveAs);
    saveAsFileAction->setStatusTip(tr("Save the current Spreadsheet file in a different place."));
    connect(saveAsFileAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    for(int i = 0; i < MaxRecentFiles; i++)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    closeAction = new QAction(tr("&Close"),this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close this window"));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    exitAction = new QAction(tr("E&xit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application."));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    setAttribute(Qt::WA_DeleteOnClose);

    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all the cells in the Spreadsheet."));
    connect(selectAllAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));;

    selectRowAction = new QAction(tr("Current Ro&w"), this);
    selectRowAction->setShortcut(tr("Ctrl+W"));
    selectRowAction->setStatusTip(tr("Select all the cells in the current row."));
    connect(selectRowAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentRow()));;

    selectColumnAction = new QAction(tr("Current Col&umn"), this);
    selectColumnAction->setShortcut(tr("Ctrl+U"));
    selectColumnAction->setStatusTip(tr("Select all the cells in the current column."));
    connect(selectColumnAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentColumn()));;

    cutAction = new QAction(tr("&Cut"), this);
    cutAction->setIcon(QIcon("images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut text."));
    connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon("images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy text."));
    connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon("images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste text."));
    connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));

    findAction = new QAction(tr("&Find"), this);
    findAction->setIcon(QIcon("images/find.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setStatusTip(tr("Find text."));
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

    goToCellAction = new QAction(tr("&Go to cell"), this);
    goToCellAction->setIcon(QIcon("images/gotocell.png"));
    goToCellAction->setShortcut(tr("Ctrl+G"));
    goToCellAction->setStatusTip(tr("Go to a specified cell."));
    connect(goToCellAction, SIGNAL(triggered()), this, SLOT(goToCell()));

    sortAction = new QAction(tr("&Sort"), this);
    sortAction->setStatusTip(tr("Sort the rows of the spreadsheet into a particular order"));
    connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));

    recalcAction = new QAction(tr("&Recalculate"), this);
    cutAction->setShortcut(tr("F9"));
    recalcAction->setStatusTip(tr("Recalculate all the formulas in the spreadsheet"));
    connect(recalcAction, SIGNAL(triggered()), spreadsheet, SLOT(recalculate()));

    showGridAction = new QAction(tr("&Show Grid"), this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(spreadsheet->showGrid());
    showGridAction->setStatusTip(tr("Show/hide the Spreadsheet's grid."));
    connect(showGridAction, SIGNAL(triggered()), spreadsheet, SLOT(setShowGrid(bool)));

    autoRecalcAction = new QAction(tr("Auto-&Recalculate Formulas"), this);
    autoRecalcAction->setCheckable(true);
    autoRecalcAction->setChecked(spreadsheet->autoRecalculate());
    autoRecalcAction->setStatusTip(tr("Enable/Disable automatic calculation of Spreadsheet formulas."));
    connect(autoRecalcAction, SIGNAL(triggered()), spreadsheet, SLOT(setAutoRecalculate(bool)));

    aboutAction = new QAction(tr("About"), this);
    aboutAction->setStatusTip(tr("Show this program's about box."));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's about box."));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void mainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Edit"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveAsFileAction);
    separatorAction = fileMenu->addSeparator();

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    selectSubMenu = editMenu->addMenu(tr("&Select"));
    selectSubMenu->addAction(selectAllAction);
    selectSubMenu->addAction(selectRowAction);
    selectSubMenu->addAction(selectColumnAction);
    editMenu->addSeparator();

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalcAction);
    toolsMenu->addAction(sortAction);
    menuBar()->addSeparator();

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalcAction);
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void mainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void mainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newFileAction);
    fileToolBar->addAction(openFileAction);
    fileToolBar->addAction(saveFileAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
}

void mainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified), this, SLOT(SpreadsheetModified()));

    updateStatusBar();
}

void mainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

void mainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void mainWindow::newFile()
{
    mainWindow *window = new mainWindow;
    window->show();
}

bool mainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("QSpreadsheet"), tr("The document has been modified. \n""Do you want to save your changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(r == QMessageBox::Yes)
            return saveFile();
        else if(r == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void mainWindow::openFile()
{
    // TODO: Enable reading and writing in other spreadsheet formats.
    if(okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spreadsheet"), ".", tr("Spreadsheet Files (*.sp)\n""Comma-Separated Values Files (*.csv)""Lotus 1-2-3 Files (*.wkl *.wks)""All Files (*.*)"));
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool mainWindow::loadFile(const QString &fileName)
{
    if(!spreadsheet->readFile(fileName))
    {
        statusBar()->showMessage(tr("Loading Canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File Loaded"), 2000);
    return true;
}

bool mainWindow::saveFile()
{
    if(curFile.isEmpty())
        return saveAsFile();
    else
        return saveFile(curFile);
}

bool mainWindow::saveFile(const QString &fileName)
{
    if(!spreadsheet->writeFile(fileName))
    {
        statusBar()->showMessage(tr("Saving Canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Files Saved"), 2000);
    return true;
}

bool mainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"), ".", tr("Spreadsheet files (*.sp)"));
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void mainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void mainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);
    QString shownName = tr("Untitled");
    if(!curFile.isEmpty())
    {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
        foreach(QWidget *win, QApplication::topLevelWidgets())
        {
            if(mainWindow *mainWin = qobject_cast<mainWindow *>(win))
                mainWin->updateRecentFileActions();
        }
    }
    setWindowTitle(tr("%1[*] - %2"). arg(shownName). arg(tr("Spreadsheet")));
}

QString mainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void mainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext())
        if(!QFile::exists(i.next()))
            i.remove();
    for(int j = 0; j < MaxRecentFiles; j++)
    {
        if(j < recentFiles.count())
        {
            QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
            recentFileActions[j]->setVisible(false);
    }
}

void mainWindow::openRecentFile()
{
    if(okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if(action)
            loadFile(action->data().toString());
    }
}

void mainWindow::find()
{
    //This is a non-modal dialog, which means that the main window is still usable whilst this dialog is open
    if(!searchDialog)
    {
        searchDialog = new findDialog(this);
        connect(searchDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), spreadsheet, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
        connect(searchDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)), spreadsheet, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
    }

    searchDialog->show();
    searchDialog->raise();
    searchDialog->activateWindow();
}

void mainWindow::goToCell()
{
    //This is a modal dialog, which means that the main window is disabled and unusable whilst this dialog is open
    goToCellDialog dialog(this);
    if(dialog.exec())
    {
        QString str = dialog.lineEdit->text().toUpper();
        spreadsheet->setCurrentCell(str.mid(1).toInt() - 1, str[0].unicode() - 'A');
    }
}

void mainWindow::sort()
{
    sortDialog dialog(this);
    QTableWidgetSelectionRange range = spreadsheet->selectedRange();
    dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());
    if(dialog.exec())
    {
        //TODO: Put this into an array to make it more efficient
        spreadsheetCompare compare;
        compare.keys[0] = dialog.primaryColumnCombo->currentIndex();
        compare.keys[1] = dialog.secondaryColumnCombo->currentIndex();
        compare.keys[2] = dialog.tertiaryColumnCombo->currentIndex();
        compare.ascending[0] = (dialog.primaryOrderCombo->currentIndex()==0);
        compare.ascending[1] = (dialog.secondaryOrderCombo->currentIndex()==0);
        compare.ascending[2] = (dialog.tertiaryOrderCombo->currentIndex()==0);
        spreadsheet->sort(compare);
    }
}

void mainWindow::about()
{
    QMessageBox::about(this, tr("About QSpreadsheet"),
                       tr("<h2>"VERSION_STRING"</h2>"
                          "<p>"COPYRIGHT_STRING"</p>"
                          "<p>"ABOUT_STRING"</p>"
                          "<p>"TEAM_STRING1"</p>"
                          "<p>"TEAM_STRING2"</p>"
                          "<p>"TEAM_STRING3"</p>"
                          "<p>"TEAM_STRING4"</p>"
                          "<p>"TEAM_STRING5"</p>"));
}

void mainWindow::writeSettings()
{
    QSettings settings("CGS Developers", "QSpreadsheet");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalcAction->isChecked());

}

void mainWindow::readSettings()
{
    QSettings settings("CGS Developers", "QSpreadsheet");

    restoreGeometry(settings.value("geometry").toByteArray());

    recentFiles = settings.value("recentFiles").toStringList();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    autoRecalcAction->setChecked(autoRecalc);
}
