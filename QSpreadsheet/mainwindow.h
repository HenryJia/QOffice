#ifndef mainWindow_H
#define mainWindow_H

#define VERSION_STRING "QSpreadSheet Alpha I Revision 1"
#define COPYRIGHT_STRING "Copyright &copy CGS Developer Team"
#define ABOUT_STRING "QSpreadSheet is a robust, multi-platoform spreadsheet program written in Qt by the CGS Developer team ->"
#define TEAM_STRING1 "The Lead Developer        ->  Hengjian (Henry) Jia"
#define TEAM_STRING2 "The Ideas Guy             ->  Samuel (Daisy) Jefferson Zhou"
#define TEAM_STRING3 "The Python Developer      ->  Oliver Cullis"
#define TEAM_STRING4 "The Jack-Of-All-Trades    ->  Theo Brown"
#define TEAM_STRING5 "The Tester                ->  Vova Viner"

#include <QMainWindow>

class QAction;
class QLabel;
class QStringList;
class QMenu;
class QToolBar;
class QString;
class QVBoxLayout;
class QDockWidget;

class findDialog;
class Spreadsheet;
class findAndReplaceWidget;

class mainWindow : public QMainWindow
{

    Q_OBJECT

public:
    mainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveAsFile();
    void find();
    void goToCell();
    void sort();
    void about();
    void openRecentFile();
    void updateStatusBar();
    void spreadsheetModified();
private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void createOtherWidgets();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    Spreadsheet * spreadsheet;
    findAndReplaceWidget *searchAndReplaceWidget;
    QLabel *locationLabel;
    QLabel *formulaLabel;
    static QStringList recentFiles;
    QString curFile;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *saveAsFileAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *selectAllAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *showGridAction;
    QAction *autoRecalcAction;
    QAction *sortAction;
    QAction *recalcAction;
    QAction *findAction;
    QAction *goToCellAction;
    QAction *exitAction;
    QAction *closeAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif // mainWindow_H
