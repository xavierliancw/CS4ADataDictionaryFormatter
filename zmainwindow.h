#ifndef ZMAINWINDOW_H
#define ZMAINWINDOW_H

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
    void generateMainDict();

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

private:
    Ui::zMainWindow *ui;
};

#endif // ZMAINWINDOW_H
