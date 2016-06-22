#include "zmainwindow.h"
#include "ui_zmainwindow.h"

zMainWindow::zMainWindow(QWidget *parent) : QMainWindow(parent),
ui(new Ui::zMainWindow)
//Main window constructor
{
    //Initial UI setup
    ui->setupUi(this);
    ui->stackWidg->setCurrentIndex(0);
    ui->nameLE->setFocus();
    ui->dataTbl->setRowCount(0);
    ui->dataTbl->setColumnCount(4);
    ui->dataTbl->setHorizontalHeaderLabels(QStringList() << "Name"
                                           << "Type" << "Value Range"
                                           << "Description");
}

zMainWindow::~zMainWindow()
{
    delete ui;
}

void zMainWindow::on_addBt_clicked()
//Adds an entry to the data table
{
    //Validate that the variable name field isn't emtpy
    if (ui->nameLE->text() != "")
    {
        QTableWidgetItem *item; //Item to populate table cell

        //Add an entry to the data table
        ui->dataTbl->insertRow(ui->dataTbl->rowCount());

        //Populate first column
        item = new QTableWidgetItem;
        item->setData(0,ui->nameLE->text());
        ui->dataTbl->setItem(ui->dataTbl->rowCount() - 1,0,item);

        //Populate second column
        item = new QTableWidgetItem;
        item->setData(0,ui->typeLE->text());
        ui->dataTbl->setItem(ui->dataTbl->rowCount() - 1,1,item);

        //Populate third column
        item = new QTableWidgetItem;
        item->setData(0,ui->valLE->text());
        ui->dataTbl->setItem(ui->dataTbl->rowCount() - 1,2,item);

        //Populate first column
        item = new QTableWidgetItem;
        item->setData(0,ui->descLE->text());
        ui->dataTbl->setItem(ui->dataTbl->rowCount() - 1,3,item);

        //Resize columns and sort alphabetically
        ui->dataTbl->resizeColumnsToContents();
        ui->dataTbl->sortByColumn(0,Qt::AscendingOrder);

        //Clear all fields and reset focus
        ui->nameLE->clear();
        ui->typeLE->clear();
        ui->valLE->clear();
        ui->descLE->clear();
        ui->nameLE->setFocus();
    }
    //Otherwise show an error window if it is
    else
    {
        QMessageBox::warning(this, tr("Variable Name is Blank"),
                             tr("Please name your variable."),
                             QMessageBox::Ok);
        ui->nameLE->setFocus();
    }
}

void zMainWindow::on_dataTbl_cellChanged(/*int row, */int column)
//Apply a sort if the variable's name is changed
{
    //If the variable's name is changed
    if (column == 0)
    {
        ui->dataTbl->sortByColumn(0,Qt::AscendingOrder);
    }
}

void zMainWindow::on_nameLE_returnPressed()
//Focus typeLE when enter is pressed
{ui->typeLE->setFocus();}

void zMainWindow::on_typeLE_returnPressed()
//Focus valLE when enter is pressed
{ui->valLE->setFocus();}

void zMainWindow::on_valLE_returnPressed()
//Focus descLE when enter is pressed
{ui->descLE->setFocus();}

void zMainWindow::on_descLE_returnPressed()
//Click addBt when enter is pressed
{on_addBt_clicked();}

void zMainWindow::on_remBt_clicked()
//Removes a row from the data table
{
    //Make sure a row is selected in the table
    if (ui->dataTbl->currentRow() >= 0)
    {
        ui->dataTbl->removeRow(ui->dataTbl->currentRow());
    }
}

void zMainWindow::on_genBt_clicked()
//Generate table and switch the view
{
    QString data = "Temp for now";   //Data to be displayed

    //Create and format table

    //Set up UI
    ui->dataTE->setPlainText(data);
    ui->stackWidg->setCurrentIndex(1);
}

void zMainWindow::on_backBt_clicked()
//Go back to the table builder
{
    ui->stackWidg->setCurrentIndex(0);
    ui->dataTE->clear();
}

void zMainWindow::on_copyBt_clicked()
//Copies table to the clipboard
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->dataTE->toPlainText());
}
