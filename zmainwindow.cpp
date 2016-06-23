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
    ui->constTbl->setRowCount(0);
    ui->constTbl->setColumnCount(4);
    ui->dataTbl->setRowCount(0);
    ui->dataTbl->setColumnCount(4);
    ui->constTbl->setHorizontalHeaderLabels(QStringList() << "Name"
                                            << "Type" << "Value Range"
                                            << "Description");
    ui->dataTbl->setHorizontalHeaderLabels(QStringList() << "Name"
                                           << "Type" << "Value Range"
                                           << "Description");
    ui->copiedLbl->setText("");

    if (ui->stackWidg->currentIndex() == 0)
    {
        //Shortcut to generate table
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return),
                      this, SLOT(on_genBt_clicked()));
        //Shortcut to switch between constant and variable mode
        new QShortcut(QKeySequence(Qt::ALT + Qt::Key_C),
                      this, SLOT(check_constCB()));
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

QString zMainWindow::StringWrap(QString wrapThis, int start, int stop)
//Word wraps a qString, adding new line escape sequences when appropriate
{
    //Validate that the width is a natural number
    if (stop > start)
    {
        int width = stop - start;
        int lastSpace = 0;
        int limitCount = 0;//Determines when a wrap is needed

        //Iterate through entire string
        for (int x = 0; x < wrapThis.size(); x++)
        {
            //Mark the last occurence of a space
            if (wrapThis[x] == ' ')
            {
                lastSpace = x;
            }
            //If wrapThis exceeds the width, word wrap
            if (limitCount > width - 1)
            {
                //Go to last space and wrap there
                wrapThis.remove(lastSpace,1);   //Remove that space
                x = lastSpace;  //Move x to the letter after last space

                //Insert a new line
                wrapThis.insert(x,"\n");
                //Insert spaces until start is reached
                for (int z = 0; z < start; z++)
                {
                    wrapThis.insert(x + 1," ");
                }
                //Move current index to start of next line
                x += start + 2;

                //Reset limit counter
                limitCount = 0;
            }
            //Otherwise increment limitCount for next character
            else
            {
                limitCount++;
            }
        }
    }
    else
    {
        //String wrap failed because of bad parameters
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

        //If adding a constant
        if (ui->constCB->isChecked())
        {
            //Add an entry to the const table
            ui->constTbl->insertRow(ui->constTbl->rowCount());

            //Populate first column
            item = new QTableWidgetItem;
            item->setData(0,ui->nameLE->text());
            ui->constTbl->setItem(ui->constTbl->rowCount() - 1,0,item);

            //Populate second column
            item = new QTableWidgetItem;
            item->setData(0,ui->typeLE->text());
            ui->constTbl->setItem(ui->constTbl->rowCount() - 1,1,item);

            //Populate third column
            item = new QTableWidgetItem;
            item->setData(0,ui->valLE->text());
            ui->constTbl->setItem(ui->constTbl->rowCount() - 1,2,item);

            //Populate fourth column
            item = new QTableWidgetItem;
            item->setData(0,ui->descLE->text());
            ui->constTbl->setItem(ui->constTbl->rowCount() - 1,3,item);

            //Resize columns and sort alphabetically
            ui->constTbl->resizeColumnsToContents();
            ui->constTbl->sortByColumn(0,Qt::AscendingOrder);
        }
        //Otherwise it's a variable
        else
        {
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

            //Populate fourth column
            item = new QTableWidgetItem;
            item->setData(0,ui->descLE->text());
            ui->dataTbl->setItem(ui->dataTbl->rowCount() - 1,3,item);

            //Resize columns and sort alphabetically
            ui->dataTbl->resizeColumnsToContents();
            ui->dataTbl->sortByColumn(0,Qt::AscendingOrder);
        }
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
        QMessageBox::warning(this, tr("Blank Name"),
                             tr("Name cannot be blank."),
                             QMessageBox::Ok);
        ui->nameLE->setFocus();
    }
}

void zMainWindow::on_constTbl_cellChanged(/*int row, */int column)
//Apply a sort if the varable's name is changed in the const table
{
    //If the variable's name is changed
    if (column == 0)
    {
        ui->constTbl->sortByColumn(0,Qt::AscendingOrder);
    }
}

void zMainWindow::on_dataTbl_cellChanged(/*int row, */int column)
//Apply a sort if the variable's name is changed in the data table
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
//Removes a row from the any of the tables
{
    //Make sure a row is selected in the table
    if (ui->constTbl->currentRow() >= 0)
    {
        ui->constTbl->removeRow(ui->constTbl->currentRow());
    }
    if (ui->dataTbl->currentRow() >= 0)
    {
        ui->dataTbl->removeRow(ui->dataTbl->currentRow());
    }
    ui->constTbl->resizeColumnsToContents();
    ui->dataTbl->resizeColumnsToContents();
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

    //Clear fields on the entry page
    ui->nameLE->clear();
    ui->typeLE->clear();
    ui->valLE->clear();
    ui->descLE->clear();

    //Loop until everything in dataTbl is read in
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
    }
    //Do the same for the constTbl
    for (int x = 0; x < ui->constTbl->rowCount(); x++)
    {
        //Grab the text
        varName = ui->constTbl->item(x,0)->text();
        varType = ui->constTbl->item(x,1)->text();
        varVal = ui->constTbl->item(x,2)->text();
        varDesc = ui->constTbl->item(x,3)->text();

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
    }
    //If everything except the description exceeds the margin
    if (varName.size() + varType.size() + varVal.size() + 2
            > ui->marginSB->value())
    {
        QMessageBox::information(this, tr("¯\\_(ツ)_/¯"),
                                 tr("Holy cow, some of your stuff is "
                                    "long.\nMaybe increase your "
                                    "margin size or shorten some "
                                    "stuff?\nDescription word "
                                    "wrapping will not work."),
                                 QMessageBox::Ok);
    }
    //Now generate the table heading
    finalData = "DATA DICTIONARY\n---------------\n"
                "***********\n*CONSTANTS*\n***********\n";
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
    finalData += singLine;
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
    finalData += singLine;

    //If there are no variables in the constTbl
    if (ui->constTbl->rowCount() == 0)
    {
        finalData += "NO CONSTANTS\n";
    }
    //Otherwise add the data in
    else
    {
        //Loop through constTbl's rows
        for (int x = 0; x < ui->dataTbl->rowCount(); x++)
        {
            //Grab the text
            varName = ui->constTbl->item(x,0)->text();
            varType = ui->constTbl->item(x,1)->text();
            varVal = ui->constTbl->item(x,2)->text();
            varDesc = ui->constTbl->item(x,3)->text();

            //Word wrap varDesc
            varDesc = StringWrap(varDesc, namSp + typSp + valSp + 3,
                                 ui->marginSB->value());

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
    }
    finalData += "\n***********\n*VARIABLES*\n***********\n";
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
    finalData += singLine;
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
    finalData += singLine;

    //If there are no variables in the dataTbl
    if (ui->dataTbl->rowCount() == 0)
    {
        finalData += "NO VARIABLES\n";
    }
    //Otherwise add the data in
    else
    {
        //Loop through dataTbl's rows
        for (int x = 0; x < ui->dataTbl->rowCount(); x++)
        {
            //Grab the text
            varName = ui->dataTbl->item(x,0)->text();
            varType = ui->dataTbl->item(x,1)->text();
            varVal = ui->dataTbl->item(x,2)->text();
            varDesc = ui->dataTbl->item(x,3)->text();

            //Word wrap varDesc
            varDesc = StringWrap(varDesc, namSp + typSp + valSp + 3,
                                 ui->marginSB->value());

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
    }

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

void zMainWindow::check_constCB()
//Checks or unchecks constCB
{ui->constCB->click();ui->nameLE->setFocus();}
