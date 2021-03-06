#ifndef WIDGETFILE_H
#define WIDGETFILE_H

#include <QWidget>
#include <QString>
#include <QScriptValue>
#include <QScriptEngine>
#include <QDebug>

class WidgetTextEdit;
class MiniSplitter;
class WidgetConsole;
class WidgetFindReplace;
class WidgetLineNumber;
class WidgetPdfViewer;
class WidgetPdfDocument;
class TaskWindow;
class WidgetLineNumber;
class SyntaxHighlighter;
class MainWindow;
class File;
class Hunspell;
class IPane;

class WidgetFile : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetFile(MainWindow *parent = 0);
    ~WidgetFile();
    void initTheme();

    void builTex(QString command);
    void bibtex(void);
    void clean(void);
    WidgetFile * masterFile() { return _masterFile; }
    void save();
    void saveAs(QString filename = "");
    void setMasterFile(WidgetFile * master) { _masterFile = master; }
    bool isEmpty();
    TaskWindow * taskPane() { return _widgetSimpleOutput; }
    WidgetPdfViewer * widgetPdfViewer() { return _widgetPdfViewer; }
    SyntaxHighlighter * syntaxHighlighter() { return _syntaxHighlighter; }
    MiniSplitter * verticalSplitter() { return _verticalSplitter; }
    MiniSplitter * editorSplitter() { return _editorSplitter; }

    void open(QString filename);
    void reload();

    bool isConsoleOpen(void);
    bool isErrorTableOpen(void);
    bool isWarningPaneOpen(void);

    File * file();

    void setFileToBuild(File * file);


    Hunspell * spellChecker();
    QString spellCheckerEncoding();
    QString dictionary() { return _dictionary; }
    void setDictionary(QString dico);

    void setWindow(MainWindow * window) { _window = window; }
    MainWindow * window() { return _window; }
    void addWidgetPdfViewerToSplitter();


    static QScriptValue toScriptValue(QScriptEngine *engine, WidgetFile * const &s)
    {
      QScriptValue obj = engine->newQObject(const_cast<WidgetFile *>(s));
      return obj;
    }

    static void fromScriptValue(const QScriptValue &obj, WidgetFile *&s)
    {
        Q_UNUSED(obj);
        Q_UNUSED(s);
    }

    QList<IPane*> & panes() { return _panes; }

signals:
    void verticalSplitterChanged();
    void opened();

public slots:
    WidgetTextEdit * widgetTextEdit() { return _widgetTextEdit; }
    WidgetTextEdit * widgetTextEdit2() { return _widgetTextEdit2; }

    void closeCurrentPane();
    void openPane(IPane * pane);
    void closePane(IPane * pane);
    void togglePane(IPane * pane);
    bool isPaneOpen(IPane * pane);

    bool isEditorSplited();
    void splitEditor(bool split);


    void openFindReplaceWidget(void);
    void closeFindReplaceWidget(void);

private:
    QString _dictionary;
    WidgetTextEdit * _widgetTextEdit;
    WidgetTextEdit * _widgetTextEdit2;
    MiniSplitter * _horizontalSplitter;
    MiniSplitter * _verticalSplitter;
    MiniSplitter * _editorSplitter;
    WidgetConsole * _widgetConsole;
    WidgetFindReplace * _widgetFindReplace;
    WidgetLineNumber * widgetLineNumber;
    WidgetPdfViewer * _widgetPdfViewer;
    TaskWindow * _widgetSimpleOutput;
    TaskWindow * _warningPane;
    WidgetLineNumber * _widgetLineNumber;
    Hunspell * _spellChecker;
    SyntaxHighlighter * _syntaxHighlighter;
    MainWindow * _window;
    WidgetFile * _masterFile;
    int _consoleHeight, _problemsHeight, _warningPaneHeight;
    QList<IPane*> _panes;
    IPane * _currentPane;
};

Q_DECLARE_METATYPE(WidgetFile*)


#endif // WIDGETFILE_H
