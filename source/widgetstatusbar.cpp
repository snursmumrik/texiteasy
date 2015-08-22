#include "widgetstatusbar.h"
#include "ui_widgetstatusbar.h"
#include "configmanager.h"
#include "filemanager.h"
#include "minisplitter.h"
#include "widgetfile.h"
#include "filestructure.h"
#include "widgettextedit.h"
#include "taskpane/taskwindow.h"

#include <QPushButton>
#include <QMenu>
#include <QDebug>
#include <QSplitter>
#include <QToolButton>
#include <QMouseEvent>
#include <QLabel>
#include <QBitmap>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

WidgetStatusBar::WidgetStatusBar(QWidget *parent) :
    QStatusBar(parent),
    ui(new Ui::WidgetStatusBar)
{
    ui->setupUi(this);
    this->setContextMenuPolicy(Qt::PreventContextMenu);




    {
    _labelSplitEditor = new WidgetStatusBarButton(this);
    QImage incorporatedImage(":/data/img/splitbutton_horizontal.png");
    QImage  separatedImage(":/data/img/splitbutton_closetop.png");
    QImage incorporatedHoverImage(":/data/img/splitbutton_horizontal.png");
    QImage  separatedHoverImage(":/data/img/splitbutton_closetop.png");
    if(!ConfigManager::Instance.darkTheme())
    {
        incorporatedImage.invertPixels();
        separatedImage.invertPixels();
        incorporatedHoverImage.invertPixels();
        separatedHoverImage.invertPixels();
    }
    _labelSplitEditor->setCheckable(true);
    _labelSplitEditor->setPixmaps(new QPixmap(QPixmap::fromImage(incorporatedImage)),
                                              new QPixmap(QPixmap::fromImage(separatedImage)),
                                              new QPixmap(QPixmap::fromImage(incorporatedHoverImage)),
                                              new QPixmap(QPixmap::fromImage(separatedHoverImage))
                                              );
    this->addPermanentWidget(_labelSplitEditor);
    }










    _labelConsole = new QLabel(QString("<div style='margin:0px;'><a class='link' style='text-decoration:none; color:")+
                                ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color())+
                               "' href='#'>"+trUtf8("Console")+"</a></div>");
    this->addPermanentWidget(_labelConsole);

    _labelErrorTable = new QLabel(QString("<a style='text-decoration:none; color:")+
                                ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color())+
                               "' href='#'>"+trUtf8("Erreurs")+"</a>");


    this->addPermanentWidget(_labelErrorTable);

    _labelWarningPane = new QLabel(QString("<a style='text-decoration:none; color:")+
                                ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color())+
                               "' href='#'>"+trUtf8("Warning")+"</a>");


    this->addPermanentWidget(_labelWarningPane);


    _positionLabel = new QLabel("<span>"+trUtf8("Ligne %1, Colonne %2").arg("1").arg("1")+"</span>",this);
    _positionLabel->setStyleSheet(QString("font-size:11px; margin-right:5px; color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));
    this->addPermanentWidget(_positionLabel, 0);

    QLabel* messageArea = new QLabel(this);
    messageArea->setStyleSheet(QString("font-size:11px; color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));
    connect(this, SIGNAL(messageChanged(QString)), messageArea, SLOT(setText(QString)));

    this->addPermanentWidget(messageArea, 1);

    //Structur
    _labelStruct = new WidgetStatusBarButton(this);
    _labelStruct->setText("Document");
    _labelStruct->enableLeftClickContextMenu();
    _labelStruct->setEnabled(false);
    this->addPermanentWidget(_labelStruct, 0);


    // Dictionnary
    _labelDictionary = new WidgetStatusBarButton(this);
    _labelDictionary->setText(ConfigManager::Instance.currentDictionary());
    foreach(const QString dico, ConfigManager::Instance.dictionnaries())
    {
        QAction * action = new QAction(dico, _labelDictionary);
        if(!dico.compare(_labelDictionary->text()))
        {
            action->setCheckable(true);
            action->setChecked(true);
        }
        connect(action, SIGNAL(triggered()), &FileManager::Instance, SLOT(setDictionaryFromAction()));
        _labelDictionary->addAction(action);
    }
    _labelDictionary->enableLeftClickContextMenu();
    _labelDictionary->setEnabled(false);
    this->addPermanentWidget(_labelDictionary, 0);


    _encodingLabel = new QLabel(this);
    _encodingLabel->setStyleSheet(QString("font-size:11px; color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));
    this->addPermanentWidget(_encodingLabel, 0);









    _labelLinkSync = new WidgetStatusBarButton(this);
    QImage linkImage(":/data/img/linkSync.png");
    QImage  unlinkImage(":/data/img/unlinkSync.png");
    if(ConfigManager::Instance.darkTheme())
    {
        linkImage.invertPixels();
        unlinkImage.invertPixels();
    }
    _labelLinkSync->setCheckable(true);
    _labelLinkSync->setPixmaps(new QPixmap(QPixmap::fromImage(unlinkImage)), new QPixmap(QPixmap::fromImage(linkImage)));
    this->addPermanentWidget(_labelLinkSync);




    _labelPdfViewerInItsOwnWidget = new WidgetStatusBarButton(this);
    QImage incorporatedImage(":/data/img/incorporatedPdf.png");
    QImage  separatedImage(":/data/img/separatedPdf.png");
    QImage incorporatedHoverImage(":/data/img/incorporatedPdfHover.png");
    QImage  separatedHoverImage(":/data/img/separatedPdfHover.png");
    if(ConfigManager::Instance.darkTheme())
    {
        incorporatedImage.invertPixels();
        separatedImage.invertPixels();
        incorporatedHoverImage.invertPixels();
        separatedHoverImage.invertPixels();
    }
    _labelPdfViewerInItsOwnWidget->setCheckable(true);
    _labelPdfViewerInItsOwnWidget->setPixmaps(new QPixmap(QPixmap::fromImage(incorporatedImage)),
                                              new QPixmap(QPixmap::fromImage(separatedImage)),
                                              new QPixmap(QPixmap::fromImage(incorporatedHoverImage)),
                                              new QPixmap(QPixmap::fromImage(separatedHoverImage))
                                              );
    this->addPermanentWidget(_labelPdfViewerInItsOwnWidget);


    connect(_labelConsole, SIGNAL(linkActivated(QString)), &FileManager::Instance, SLOT(toggleConsole()));
    connect(_labelErrorTable, SIGNAL(linkActivated(QString)), &FileManager::Instance, SLOT(toggleErrorTable()));
    connect(_labelWarningPane, SIGNAL(linkActivated(QString)), &FileManager::Instance, SLOT(toggleWarningPane()));
    connect(&FileManager::Instance, SIGNAL(verticalSplitterChanged()), this, SLOT(updateButtons()));



    this->setMaximumHeight(25);
}

WidgetStatusBar::~WidgetStatusBar()
{
    delete ui;
}

void WidgetStatusBar::cursorPositionChanged(int row, int column)
{
    setPosition(row, column);
    updateStruct();
}

void WidgetStatusBar::setPosition(int row, int column)
{
    _positionLabel->setText("<span>"+trUtf8("Ligne %1, Colonne %2").arg(QString::number(row)).arg(QString::number(column))+"</span>");
}

void WidgetStatusBar::setEncoding(QString encoding)
{
    _encodingLabel->setText(encoding);
}

void WidgetStatusBar::updateStruct()
{
    _labelStruct->removeActions();
    WidgetFile * widget = FileManager::Instance.currentWidgetFile();

    //Stop if there is no opened file
    if(!widget)
    {
        return;
    }
    QStringList structure = widget->widgetTextEdit()->textStruct()->sectionsList("  ");
    QString currentSection = widget->widgetTextEdit()->textStruct()->currentSection();
    if(structure.isEmpty() || currentSection.isEmpty())
    {
        _labelStruct->setText("Document");
    }
    else
    {
        _labelStruct->setText(currentSection);
    }
    foreach(const QString line, structure)
    {
        QAction * action = new QAction(line, _labelStruct);
        connect(action, SIGNAL(triggered()), &FileManager::Instance, SLOT(goToSection()));
        _labelStruct->addAction(action);
    }
    updateTaskPane();
    QTimer::singleShot(1, this, SLOT(checkStructAction())); // do it after because sometimes it does not work
}
void WidgetStatusBar::checkStructAction()
{
    foreach(QAction * action, _labelStruct->actions())
    {
        if(!_labelStruct->text().trimmed().compare(action->text().trimmed()))
        {
            action->setCheckable(true);
            action->setChecked(true);
        }
    }
}

void WidgetStatusBar::updateButtons()
{
    if(!FileManager::Instance.currentWidgetFile())
    {
        _labelDictionary->setText("");
        _labelDictionary->setEnabled(false);
        return;
    }
    //update split button

    QList<int> editorSizes = FileManager::Instance.currentWidgetFile()->editorSplitter()->sizes();
    _labelSplitEditor->setChecked(editorSizes.first());


    // udate Console widget and errorTable widget
     QList<int> sizes= FileManager::Instance.currentWidgetFile()->verticalSplitter()->sizes();
     if(sizes[2] == 0)
     {
         _labelWarningPane->setStyleSheet(QString("background-color:transparent"));
     }
     else
     {
         _labelWarningPane->setStyleSheet(QString("background-color:")+ ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(200)));
     }
    if(sizes[3] == 0)
    {
        _labelErrorTable->setStyleSheet(QString("background-color:transparent"));
    }
    else
    {
        _labelErrorTable->setStyleSheet(QString("background-color:")+ ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(200)));
    }
    if(sizes[4] == 0)
    {
        _labelConsole->setStyleSheet(QString("background-color:transparent"));
    }
    else
    {
        _labelConsole->setStyleSheet(QString("background-color:")+ ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(200)));
    }

    //update struct label
    updateStruct();
    _labelStruct->setEnabled(true);

    //update dictionary label
    _labelDictionary->setEnabled(true);
    _labelDictionary->setText(FileManager::Instance.currentWidgetFile()->dictionary());

    //update Encoding label
    _encodingLabel->setText(FileManager::Instance.currentWidgetFile()->file()->codec());

}

void WidgetStatusBar::updateTaskPane()
{
    WidgetFile * w = FileManager::Instance.currentWidgetFile();
    QString warnings;
    if(w && w->taskPane()->warningTaskCount())
    {
        warnings = " "+QString::number(w->taskPane()->warningTaskCount())+" <img src=\"qrc:/data/img/warning.png\" />";
    }
    _labelWarningPane->setText(QString("<a style='font-size:12px; margin-top:-3px; text-decoration:none; color:")+
            ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color())+ "' href='#'>"+trUtf8("Warning")+warnings+"</a>"
                );
    _labelErrorTable->setText(QString("<a style='font-size:12px; margin-top:-3px; text-decoration:none; color:")+
                                ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color())+
                               "' href='#'>"+trUtf8("Erreurs")+"</a>");

}

void WidgetStatusBar::initTheme()
{
    this->setStyleSheet("QStatusBar::item { margin-left:4px; border: none;} QStatusBar {padding:0px; height:100px; background: "+
                        ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("line-number").background().color())+
                                     "}");

    _labelStruct->label()->setStyleSheet(QString("font-size:12px; margin-right:5px; color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));

    _labelDictionary->label()->setStyleSheet(QString("font-size:12px; margin-right:5px; color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));
    //_labelDictionary->setStyleSheet(QString("color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));

    _labelConsole->setText(QString("<div style='font-size:12px; '><a class='link' style='text-decoration:none; color:")+
                                ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color())+
                               "' href='#'>"+trUtf8("Console")+"</a></div>");
    updateTaskPane();
    _positionLabel->setStyleSheet(QString("font-size:12px; margin-right:5px; color:")+ConfigManager::Instance.colorToString(ConfigManager::Instance.getTextCharFormats("normal").foreground().color()));


    bool darkTheme = ConfigManager::Instance.getTextCharFormats("normal").background().color().value() < 100;
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(darkTheme ? 400 : 130));
    effect->setOffset(darkTheme ? -1 : 1, darkTheme ? -1 : 1);
    _labelConsole->setGraphicsEffect(effect);
    effect= new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(darkTheme ? 400 : 130));
    effect->setOffset(darkTheme ? -1 : 1, darkTheme ? -1 : 1);
    _labelErrorTable->setGraphicsEffect(effect);
    effect= new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(darkTheme ? 400 : 130));
    effect->setOffset(darkTheme ? -1 : 1, darkTheme ? -1 : 1);
    _labelWarningPane->setGraphicsEffect(effect);
    effect= new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(darkTheme ? 400 : 130));
    effect->setOffset(darkTheme ? -1 : 1, darkTheme ? -1 : 1);
    _positionLabel->setGraphicsEffect(effect);
    effect= new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(ConfigManager::Instance.getTextCharFormats("normal").background().color().darker(darkTheme ? 400 : 130));
    effect->setOffset(darkTheme ? -1 : 1, darkTheme ? -1 : 1);
    _labelDictionary->setGraphicsEffect(effect);

    updateButtons();
}
/***************************************************
 *
 *          WidgetStatusBarButton
 *
 **************************/

void WidgetStatusBarButton::leaveEvent(QEvent *)
{
    if(isChecked())
    {
        if(_checkedPixmap)
        {
            _label->setPixmap(*_checkedPixmap);
        }
    }
    else
    {
        if(_defaultPixmap)
        {
            _label->setPixmap(*_defaultPixmap);
        }
    }
}

void WidgetStatusBarButton::enterEvent(QEvent *)
{
    if(isChecked())
    {
        if(_checkedHoverPixmap)
        {
            _label->setPixmap(*_checkedHoverPixmap);
        }
    }
    else
    {
        if(_defaultHoverPixmap)
        {
            _label->setPixmap(*_defaultHoverPixmap);
        }
    }
}
void WidgetStatusBarButton::setChecked(bool checked)
{
    if(this->action() && checked != this->action()->isChecked())
    {
        this->action()->toggle();
        return;
    }
    if(_checked != checked)
    {
        this->toggleCheckedWithoutTriggeringAction();
    }
}

void WidgetStatusBarButton::toggleChecked()
{
    _checked = ! _checked;
    if(this->action())
    {
        this->action()->trigger();
    }
}

void WidgetStatusBarButton::toggleCheckedWithoutTriggeringAction()
{
    _checked = ! _checked;
    if(isChecked())
    {
        if(_checkedPixmap)
        {
            _label->setPixmap(*_checkedPixmap);
        }
    }
    else
    {
        if(_defaultPixmap)
        {
            _label->setPixmap(*_defaultPixmap);
        }
    }
}
void WidgetStatusBarButton::mousePressEvent(QMouseEvent * event)
{
    if(!this->isEnabled())
    {
        return;
    }
    if(this->isCheckable() && this->action())
    {
        this->action()->toggle();
        return;
    }
    if(event->button() == Qt::RightButton || (_leftClickContextMenu && event->button() == Qt::LeftButton))
    {
        if(this->actions().count())
        {
            QMenu menu(this);
            QList<QAction*> listAction = this->actions();
            menu.addActions(listAction);
            int bottom = menu.actionGeometry(listAction.last()).bottom();

            QAction * action = menu.exec(this->mapToGlobal(QPoint(0, - 6 - bottom)));
            if(action)
            {
                this->setText(action->text());
                updateGeometry();
                foreach(QAction * a, this->actions())
                {
                    a->setChecked(false);
                }
                action->setCheckable(true);
                action->setChecked(true);
            }
            return;
        }
    }
}
void WidgetStatusBarButton::setAction(QAction *action)
{
    _action = action;
    this->setCheckable(_action->isCheckable());
    if(this->isCheckable())
    {
        this->setChecked(_action->isChecked());
        connect(_action, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
    }
}

void WidgetStatusBarButton::setText(QString text)
{
    _label->setText(text);
    updateGeometry();
}

void WidgetStatusBarButton::updateGeometry()
{
    QFontMetrics fm(QFont(this->_label->font().family(), 12));

    int width = fm.width(_label->text()) + 10;
    _label->setMinimumWidth(width);
    _label->setMaximumWidth(width);
    this->setMinimumWidth(width);
    this->setMaximumWidth(width);
    QWidget::updateGeometry();
}

void WidgetStatusBarButton::removeActions()
{
    foreach(QAction * action, this->actions())
    {
        removeAction(action);
    }
}
