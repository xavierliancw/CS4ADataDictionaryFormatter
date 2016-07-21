#include "zmainwindow.h"
#include "ui_zmainwindow.h"

zMainWindow::zMainWindow(QWidget *parent) : QMainWindow(parent),
ui(new Ui::zMainWindow)
//Main window constructor
{
    //Initial UI setup
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/APPDATA/windowicon.png"));
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
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
    ui->dataTbl->resizeColumnsToContents();
    ui->constTbl->resizeColumnsToContents();
    ui->tutDescLbl->setText("");
//    ui->copiedLbl->setText("");
//    if (ui->constCB->isChecked())
//    {
//        ui->consLbl->setText("Constants <- (Selected)");
//        ui->varsLbl->setText("Variables");
//    }
//    else
//    {
//        ui->varsLbl->setText("Variables <- (Selected)");
//        ui->consLbl->setText("Constants");
//    }
    //Generate empty dictionary table
    generateMainDict();

    //Fix the table's font so that it's monospaced (everything lines up)
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->dataTE->setFont(fixedFont);

    //Shortcut to copy the text edit that's in view
    int NEEDSFUTUREUPDATE;
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C),
                  this, SLOT(copy_view()));
    //Shortcut to switch between constant and variable mode
    new QShortcut(QKeySequence(Qt::ALT + Qt::Key_C),
                  this, SLOT(check_constCB()));
    //Shortcut to focus name field
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L),
                  this, SLOT(focus_name()));
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
            if (limitCount > width - 4)
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
        return "ERROR: Word wrapping failed";
    }
    return wrapThis;
}

void zMainWindow::generateMainDict()
//Updates main dictionary
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
    //Do the same for the constTbl but only if user wants it displayed
    if (!ui->hideConstsCB->isChecked())
    {
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
    finalData = "/*";
    //Loop until specified margin
    for (int x = 0; x < ui->marginSB->value() - 2; x++)
    {
        finalData += "*";   //Add the top of the comment box
    }
    finalData += "\n MAIN DATA DICTIONARY\n --------------------\n";
    //If there are no constants or variables
    if (ui->constTbl->rowCount() == 0 && ui->dataTbl->rowCount() == 0)
    {
        finalData += " ~No Constants or Variables Used~";
    }
    else
    {
        //If user wants to display the constants section
        if (!ui->hideConstsCB->isChecked())
        {
            finalData += " ***********\n *CONSTANTS*\n ***********\n";
            singLine = " NAME";
            for (int x = 0; x < namSp - 4; x++)
            {singLine += " ";}
            singLine += " TYPE";
            for (int x = 0; x < typSp - 4; x++)
            {singLine += " ";}
            singLine += " VALUE RANGE";
            for (int x = 0; x < valSp - 11; x++)
            {singLine += " ";}
            singLine += " DESCRIPTION\n ";
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
                 - 5; x++)
            {singLine += "=";}
            singLine += "\n";
            finalData += singLine;

            //If there are no variables in the constTbl
            if (ui->constTbl->rowCount() == 0)
            {
                finalData += " NO CONSTANTS\n";
            }
            //Otherwise add the data in
            else
            {
                //Loop through constTbl's rows
                for (int x = 0; x < ui->constTbl->rowCount(); x++)
                {
                    //Grab the text
                    varName = ui->constTbl->item(x,0)->text();
                    varType = ui->constTbl->item(x,1)->text();
                    varVal = ui->constTbl->item(x,2)->text();
                    varDesc = ui->constTbl->item(x,3)->text();

                    //Word wrap varDesc
                    varDesc = StringWrap(varDesc, namSp + typSp
                                         + valSp + 4,
                                         ui->marginSB->value());

                    //Add to the table while formatting spaces
                    finalData += " " + varName;
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
            finalData += "\n ***********\n *VARIABLES*\n ***********\n";
        }
        singLine = " NAME";
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
        singLine = " ";
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
        - 5; x++)
        {singLine += "=";}
        singLine += "\n";
        finalData += singLine;

        //If there are no variables in the dataTbl
        if (ui->dataTbl->rowCount() == 0)
        {
            finalData += " NO VARIABLES";
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
                varDesc = StringWrap(varDesc, namSp + typSp + valSp + 4,
                                     ui->marginSB->value());

                //Add to the table while formatting spaces
                finalData += " " + varName;
                for (int s = 0; s < namSp - varName.size(); s++)
                {finalData += " ";}
                finalData += " " + varType;
                for (int s = 0; s < typSp - varType.size(); s++)
                {finalData += " ";}
                finalData += " " + varVal;
                for (int s = 0; s < valSp - varVal.size(); s++)
                {finalData += " ";}
                finalData += " " + varDesc;

                //Insert endlines between variables
                if (x != ui->dataTbl->rowCount() - 1)
                {
                    finalData += "\n";
                }
            }
        }
    }
    //Finish comment box
    finalData += "\n";
    for (int x = 0; x < ui->marginSB->value() - 2; x++)
    {
        finalData += "*";
    }
    finalData += "*/";

    //Set up UI
    ui->dataTE->setPlainText(finalData);
}

void zMainWindow::on_addBt_clicked()
//Adds an entry to the data table
{
    //Block signals
    QSignalBlocker blockSigsFrom(ui->constTbl);
    QSignalBlocker alsoBlock(ui->dataTbl);

    //Validate that the variable name field isn't emtpy
    if (ui->nameLE->text() != "")
    {
        int ret = QMessageBox::Yes;
        //Check if any fields are blank
        if (ui->nameLE->text() == "" || ui->typeLE->text() == ""
            || ui->valLE->text() == "" || ui->descLE->text() == "")
        {
            ret = QMessageBox::information(this, tr("Blank Fields"),
                                           tr("Hey, there are some "
                                              "blank fields.\nDo you "
                                              "want to proceed?"),
                                           QMessageBox::Yes,
                                           QMessageBox::No);
        }
        if (ret == QMessageBox::Yes)
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
                ui->constTbl->setItem(ui->constTbl->rowCount()- 1,0,item);

                //Populate second column
                item = new QTableWidgetItem;
                item->setData(0,ui->typeLE->text());
                ui->constTbl->setItem(ui->constTbl->rowCount()- 1,1,item);

                //Populate third column
                item = new QTableWidgetItem;
                item->setData(0,ui->valLE->text());
                ui->constTbl->setItem(ui->constTbl->rowCount()- 1,2,item);

                //Populate fourth column
                item = new QTableWidgetItem;
                item->setData(0,ui->descLE->text());
                ui->constTbl->setItem(ui->constTbl->rowCount()- 1,3,item);

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

            //Update the text edit to display live additions
            generateMainDict();
        }
    }
    //Otherwise show an error window if name field is blank
    else
    {
        QMessageBox::warning(this, tr("Blank Name"),
                             tr("Name cannot be blank."),
                             QMessageBox::Ok);
        ui->nameLE->setFocus();
    }
    //Finish blocking
    blockSigsFrom.unblock();
    alsoBlock.unblock();
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

    //Redraw table
    generateMainDict();
}

void zMainWindow::on_copyMainBt_clicked()
//Copy the main dictionary
{
    //Copy to clipboard
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->dataTE->toPlainText());
//    ui->copiedLbl->setText("Copied!");

//    //You're here figure out how to fade text in a push button.

//    //Make the copied label fade out
//    ui->genBt->setText("Copied!");
//    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
//    ui->copiedLbl->setGraphicsEffect(eff);
//    ui->genBt->setGraphicsEffect(eff);
//    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
//    a->setDuration(10000);
//    a->setStartValue(1);
//    a->setEndValue(0);
//    a->setEasingCurve(QEasingCurve::OutBack);
//    a->start(QPropertyAnimation::DeleteWhenStopped);
//    connect(a,SIGNAL(finished()),this,SLOT(hide_copiedLbl()));
}

void zMainWindow::on_actionHelp_triggered()
//Show Help window
{
    QMessageBox::information(this, tr("Help & Hints"),
                             tr("To enter data faster, use RETURN "
                                "instead of TAB to navigate the fields.\n"
                                "CTRL+SHIFT+C will copy the dictionary "
                                "in view to the clipboard.\n"
                                "ALT+C will check or uncheck the "
                                "constants checkbox.\n"
                                "CTRL+L will focus the variable name "
                                "field."),
                             QMessageBox::Ok);
}

void zMainWindow::on_actionAbout_triggered()
//Show About window
{
    QMessageBox::information(this, tr("About"),
                             tr("Version 0.7\n"
                                "Created by Xavier Lian\n"
                                "20 July 2016 for Computer Science 4A"),
                             QMessageBox::Ok);
}

void zMainWindow::check_constCB()
//Checks or unchecks constCB
{
    ui->constCB->click();
}

void zMainWindow::on_actionFeedback_triggered()
//Opens feedback information window
{
    QMessageBox::information(this, tr("Feedback Please! ( ͡° ͜ʖ ͡°)"),
                             tr("Created by Xavier Lian\n"
                                "Email me at xavierliancw@gmail.com and "
                                "leave some feedback on this program.\n\n"
                                "Let me know what should be changed, "
                                "added, and/or improved.\n\n"
                                "If you think you can add stuff "
                                "yourself, fork my repository on GitHub, "
                                "then submit a pull request when you're "
                                "done.\n\n"
                                "The repo is: "
           "https://github.com/xavierliancw/CS4ADataDictionaryFormatter"),
                             QMessageBox::Ok);
}

void zMainWindow::focus_name()
//Focuses name field and highlights all text within
{ui->nameLE->setFocus(); ui->nameLE->selectAll();}

void zMainWindow::on_descLE_textChanged(/*const QString &arg1*/)
//Make tutorial label appear when approriate
{
    //Show when there's something in descLE and in nameLE
    if (ui->descLE->text() != "" && ui->nameLE->text() != "")
    {
        ui->tutDescLbl->setText("Press return to add");
    }
    else
    {
        ui->tutDescLbl->setText("");
    }
}

void zMainWindow::on_marginSB_valueChanged(int arg1)
//Live updates main dictionary when margin value is changed
{generateMainDict();}

void zMainWindow::copy_view()
//Copies the text edit that's in view to the clipboard
{
    switch (ui->majorTW->currentIndex())
    {
    case 0:
        break;
    case 1:
        ui->copyMainBt->click(); break;
    }
}

void zMainWindow::on_constTbl_cellChanged(/*int row, int column*/)
//Live update the main dictionary when this table is edited
{
    generateMainDict();
}

void zMainWindow::on_dataTbl_cellChanged(/*int row, int column*/)
//Live update the main dictionary when this table is edited
{
    generateMainDict();
}

void zMainWindow::on_hideConstsCB_clicked()
//Live update the main dictionary when this is toggled
{
    generateMainDict();
}
