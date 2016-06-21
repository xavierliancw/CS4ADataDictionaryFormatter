#ifndef ZMAINWINDOW_H
#define ZMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class zMainWindow;
}

class zMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zMainWindow(QWidget *parent = 0);
    ~zMainWindow();

private:
    Ui::zMainWindow *ui;
};

#endif // ZMAINWINDOW_H
