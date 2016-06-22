#ifndef ZMAINWINDOW_H
#define ZMAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QClipboard>

namespace Ui {
class zMainWindow;
}

class zMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zMainWindow(QWidget *parent = 0);
    ~zMainWindow();

private slots:
    void on_addBt_clicked();

    void on_dataTbl_cellChanged(/*int row, */int column);

    void on_nameLE_returnPressed();

    void on_typeLE_returnPressed();

    void on_valLE_returnPressed();

    void on_descLE_returnPressed();

    void on_remBt_clicked();

    void on_genBt_clicked();

    void on_backBt_clicked();

    void on_copyBt_clicked();

private:
    Ui::zMainWindow *ui;
};

#endif // ZMAINWINDOW_H
