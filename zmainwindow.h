#ifndef ZMAINWINDOW_H
#define ZMAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QMessageBox>
#include <QClipboard>
#include <QShortcut>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDebug>

namespace Ui {
class zMainWindow;
}

class zMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zMainWindow(QWidget *parent = 0);
    ~zMainWindow();
    QString StringWrap(QString wrapThis, int start, int stop);
    QString generateMainDict();
    void generateHeader();
    bool addToPlan();

private slots:
    void on_addBt_clicked();

    void on_constTbl_cellChanged(/*int row, */int column);

    void on_dataTbl_cellChanged(/*int row, */int column);

    void on_nameLE_returnPressed();

    void on_typeLE_returnPressed();

    void on_valLE_returnPressed();

    void on_descLE_returnPressed();

    void on_remBt_clicked();

    void on_copyMainBt_clicked();

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

    void check_constCB();

    void on_actionFeedback_triggered();

    void focus_name();

    void on_descLE_textChanged(/*const QString &arg1*/);

    void on_marginSB_valueChanged(int arg1);

    void copy_view();

    void on_constTbl_cellChanged(/*int row, int column*/);

    void on_dataTbl_cellChanged(/*int row, int column*/);

    void on_hideConstsCB_clicked();

    void hideAllCopied();

    void on_todayBt_clicked();

    void on_chapSB_valueChanged(int arg1);

    void on_progSB_valueChanged(int arg1);

    void on_fileNameLE_editingFinished();

    void on_proNameLE_editingFinished();

    void on_lastModDE_editingFinished();

    void on_problemStateTE_textChanged();

    void on_planSB_valueChanged(int arg1);

    void on_planLE_returnPressed();

    void on_planLE_editingFinished();

    void on_delPlanBt_clicked();

    void on_insPlanBt_clicked();

    void on_mainClassLE_textChanged(const QString &arg1);

    void on_fileNameLE_returnPressed();

    void on_proNameLE_returnPressed();

    void on_planLE_textChanged(const QString &arg1);

    void on_headerCopyBt_clicked();

    void on_lastModDE_dateChanged(const QDate &date);

private:
    Ui::zMainWindow *ui;
    std::vector<QString> planSteps;
};

#endif // ZMAINWINDOW_H
