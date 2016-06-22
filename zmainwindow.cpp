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
    ui->copiedLbl->setText("");
    ui->dataTbl->setHorizontalHeaderLabels(QStringList() << "Name"
                                           << "Type" << "Value Range"
                                           << "Description");

    if (ui->stackWidg->currentIndex() == 0)
    {
        //Shortcut to generate table
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return),
                      this, SLOT(on_genBt_clicked()));
    }
    //Shortcut to straight up copy the final data table without clicking
    if (ui->stackWidg->currentIndex() == 1)
    {
        int NOTWORKING;
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return),
                      this, SLOT(on_copyBt_clicked()));
    }
}

zMainWindow::~zMainWindow()
//Default destructor
{delete ui;}

QString zMainWindow::stringWrap(QString wrapThis, int start, int stop)
//Word wraps a qString, adding new line escape sequences when appropriate
{
    int UNFINISHED; //Validate start and stop
    int width = stop - start;
    int lastSpace = 0;

    for (int x = 0; x < wrapThis.size(); x++)
    {
        //Mark the last occurence of a space
        if (wrapThis[x] == ' ')
        {
            lastSpace = x;
        }
        //If wrapThis exceeds the width, word wrap
        if (x == width + 1)
        {
            //If it's a space, get rid of it
            if (wrapThis[x] == ' ')
            {
                wrapThis.remove(x,x);
            }
            //Otherwise, find last space and then wrap there
            else
            {
                //Remove that space
                wrapThis.remove(lastSpace,1);
                x = lastSpace;
            }
            //Insert a new line
            wrapThis.insert(x,"\n");

            //Insert spaces until start is reached
            for (int z = 0; z < start; z++)
            {
                wrapThis.insert(x + 2 + z," ");
            }
        }
    }
    return wrapThis;
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
    QString finalData;  //Final data to be displayed
    QString varName;    //Grab variable name
    QString varType;    //Grab variable type
    QString varVal;     //Grab variable value range
    QString varDesc;    //Grab variable description
    QString singLine;   //Single line of table
    int namSp = 4;      //Width of name column (Default value)
    int typSp = 7;      //Width of type column
    int valSp = 11;     //Width of value range column
//    int descSp = 11;    //Width of description column

    //Clear fields on the entry page
    ui->nameLE->clear();
    ui->typeLE->clear();
    ui->valLE->clear();
    ui->descLE->clear();

    //Loop untill everything in dataTbl is read in
    for (int x = 0; x < ui->dataTbl->rowCount(); x++)
    {
        //Grab the text
        varName = ui->dataTbl->item(x,0)->text();
        varType = ui->dataTbl->item(x,1)->text();
        varVal = ui->dataTbl->item(x,2)->text();
        varDesc = ui->dataTbl->item(x,3)->text();

        //RESIZE COLUMNS IF NECESSARY
        //Resize name col if needed
        if (varName.size() > namSp)
        {namSp = varName.size();}
        //Resize type col if needed
        if (varType.size() > typSp)
        {typSp = varType.size();}
        //Resize val col if needed
        if (varVal.size() > valSp)
        {valSp = varVal.size();}

        //If everything except the description exceeds the margin
        if (varName.size() + varType.size() + varVal.size() + 2
                > ui->marginSB->value())
        {
            QMessageBox::information(this, tr("Wow"),
                                     tr("Holy cow, some of your stuff is "
                                        "long.\nMaybe increase your "
                                        "margin size or shorten some "
                                        "stuff?\n¯\\_(ツ)_/¯"),
                                     QMessageBox::Ok);
        }
//        //If an entry is too long, word wrap the description
//        else if (varName.size() + varType.size() + varVal.size()
//                 + varDesc.size() + 3 > ui->marginSB->value())
//        {
//            varDesc = stringWrap(varDesc, namSp + typSp + valSp + 3
//                                 + varVal.size() + 3, 75);
//        }
    }
    //Loop again, this time adding to the table
    for (int x = 0; x < ui->dataTbl->rowCount(); x++)
    {
        //Grab the text
        varName = ui->dataTbl->item(x,0)->text();
        varType = ui->dataTbl->item(x,1)->text();
        varVal = ui->dataTbl->item(x,2)->text();
        varDesc = ui->dataTbl->item(x,3)->text();

        //Word wrap varDesc
//        varDesc = stringWrap(varDesc, namSp + typSp + valSp + 3,
//                             ui->marginSB->value());

        //Add to the table while formatting spaces
        finalData += varName;
        for (int s = 0; s < namSp - varName.size(); s++)
        {finalData += " ";}
        finalData += " " + varType;
        for (int s = 0; s < typSp - varType.size(); s++)
        {finalData += " ";}
        finalData += " " + varVal;
        for (int s = 0; s < valSp - varVal.size(); s++)
        {finalData += " ";}
        finalData += " " + varDesc + "\n";
    }
    //Finalize the table
    singLine = "";
    for (int x = 0; x < namSp; x++)
    {singLine += "=";}
    singLine += " ";
    for (int x = 0; x < typSp; x++)
    {singLine += "=";}
    singLine += " ";
    for (int x = 0; x < valSp; x++)
    {singLine += "=";}
    singLine += " ";
    for (int x = 0; x < ui->marginSB->value() - namSp - typSp - valSp
         - 4; x++)
    {singLine += "=";}
    singLine += "\n";
    finalData.prepend(singLine);
    singLine = "NAME";
    for (int x = 0; x < namSp - 4; x++)
    {singLine += " ";}
    singLine += " TYPE";
    for (int x = 0; x < typSp - 4; x++)
    {singLine += " ";}
    singLine += " VALUE RANGE";
    for (int x = 0; x < valSp - 11; x++)
    {singLine += " ";}
    singLine += " DESCRIPTION\n";
    finalData.prepend(singLine);
    singLine = "---------------\n";
    finalData.prepend(singLine);
    singLine = "Data Dictionary\n";
    finalData.prepend(singLine);

    //Set up UI
    ui->dataTE->setPlainText(finalData);
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
    ui->copiedLbl->setText("Copied!");

    //Make the copied label fade out
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->copiedLbl->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(10000);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a,SIGNAL(finished()),this,SLOT(hide_copiedLbl()));
}

void zMainWindow::on_actionHelp_triggered()
//Show Help window
{
    QMessageBox::information(this, tr("Help & Hints"),
                             tr("CTRL + SHIFT + C on the results page "
                                "will automatically copy the final data "
                                "table, so no clicking is necessary.\n"
                                "CTRL + RETURN will immediately "
                                "generate the data table."),
                             QMessageBox::Ok);
}

void zMainWindow::on_actionAbout_triggered()
//Show About window
{
    QMessageBox::information(this, tr("About"),
                             tr("Version 0.3\n"
                                "Created by Xavier Lian\n"
                                "22 June 2016 for Computer Science 4A"),
                             QMessageBox::Ok);
}

void zMainWindow::hide_copiedLbl()
//Hide copyLbl
{
    ui->copiedLbl->setText("");
}
