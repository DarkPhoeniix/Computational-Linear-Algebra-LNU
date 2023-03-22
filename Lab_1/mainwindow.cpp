#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "randomization.h"
#include "set_matrix_size.h"

#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QMenu>

#include <memory>
#include <map>

namespace
{
std::map<std::string, QAction*> createMatrixToolset()
{
    QFont font;
    font.setPointSize(11);

    QAction* action_load = new QAction();
    action_load->setObjectName("action_load");
    action_load->setText(QCoreApplication::translate("MainWindow", "Load matrix", nullptr));
    action_load->setFont(font);

    QAction* action_save = new QAction();
    action_save->setObjectName("action_save");
    action_save->setText(QCoreApplication::translate("MainWindow", "Save matrix", nullptr));
    action_save->setFont(font);

    QAction* action_add_new_row = new QAction();
    action_add_new_row->setObjectName("action_add_new_row");
    action_add_new_row->setText(QCoreApplication::translate("MainWindow", "Add new row", nullptr));
    action_add_new_row->setFont(font);

    QAction* action_add_new_column = new QAction();
    action_add_new_column->setObjectName("action_add_new_column");
    action_add_new_column->setText(QCoreApplication::translate("MainWindow", "Add new column", nullptr));
    action_add_new_column->setFont(font);

    QAction* action_remove_row = new QAction();
    action_remove_row->setObjectName("action_remove_row");
    action_remove_row->setText(QCoreApplication::translate("MainWindow", "Remove row", nullptr));
    action_remove_row->setFont(font);

    QAction* action_remove_column = new QAction();
    action_remove_column->setObjectName("action_remove_column");
    action_remove_column->setText(QCoreApplication::translate("MainWindow", "Remove column", nullptr));
    action_remove_column->setFont(font);

    QAction* action_randomize = new QAction();
    action_randomize->setObjectName("action_randomize");
    action_randomize->setText(QCoreApplication::translate("MainWindow", "Randomize...", nullptr));
    action_randomize->setFont(font);

    QAction* action_create_matrix = new QAction();
    action_create_matrix->setObjectName("action_create_matrix");
    action_create_matrix->setText(QCoreApplication::translate("MainWindow", "Create matrix...", nullptr));
    action_create_matrix->setFont(font);

    std::map<std::string, QAction*> actions;
    actions["load"] = action_load;
    actions["save"] = action_save;
    actions["add_new_row"] = action_add_new_row;
    actions["add_new_column"] = action_add_new_column;
    actions["remove_row"] = action_remove_row;
    actions["remove_column"] = action_remove_column;
    actions["randomize"] = action_randomize;
    actions["create_matrix"] = action_create_matrix;

    return actions;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createMenubarForMatrixOperations();

//    connect(ui->table_A, &QTableWidget::cellChanged, this, &MainWindow::setCellA);

//    connect(ui->action_load_B, &QAction::triggered, this, &MainWindow::loadMatrixB);
//    connect(ui->action_save_B, &QAction::triggered, this, &MainWindow::saveMatrixB);
//    connect(ui->action_add_new_row_to_B, &QAction::triggered, this, &MainWindow::addRowToMatrixB);
//    connect(ui->action_remove_row_from_B, &QAction::triggered, this, &MainWindow::removeRowFromMatrixB);
//    connect(ui->action_randomize_B, &QAction::triggered, this, &MainWindow::randomizeB);
//    connect(ui->action_create_matrix_B, &QAction::triggered, this, &MainWindow::createMatrixB);
//    connect(ui->table_B, &QTableWidget::cellChanged, this, &MainWindow::setCellB);

//    connect(ui->push_button_sle_solver_solve, &QPushButton::clicked, this, &MainWindow::solve);

//    ui->table_x->setEditTriggers(QTableWidget::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMatrixA_MatrixOperations()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            A = Matrix::readFromFile(filepath.toStdString());
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::saveMatrixA_MatrixOperations()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(A, filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::addRowToMatrixA_MatrixOperations()
{
    try
    {
        A.addRow();

        updateMatrixA_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::addColumnToMatrixA_MatrixOperations()
{
    try
    {
        A.addColumn();

        updateMatrixA_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::removeRowFromMatrixA_MatrixOperations()
{
    try
    {
        A.removeRow();

        updateMatrixA_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::removeColumnFromMatrixA_MatrixOperations()
{
    try
    {
        A.removeColumn();

        updateMatrixA_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::randomizeA_MatrixOperations()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
            A.randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());

        updateMatrixA_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::createMatrixA_MatrixOperations()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<SetMatrixSize> dialog = std::make_unique<SetMatrixSize>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            A.reset(dialog->getRowsNumber(), dialog->getColumnsNumber());
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::setCellA_MatrixOperations(int rowIndex, int columnIndex)
{
   A[rowIndex][columnIndex] = ui->table_A_matrix_operations->item(rowIndex, columnIndex)->text().toDouble();
}

void MainWindow::loadMatrixB_MatrixOperations()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            B = Matrix::readFromFile(filepath.toStdString());
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::saveMatrixB_MatrixOperations()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(B, filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::addRowToMatrixB_MatrixOperations()
{
    try
    {
        B.addRow();

        updateMatrixB_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::addColumnToMatrixB_MatrixOperations()
{
    try
    {
        B.addColumn();

        updateMatrixB_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::removeRowFromMatrixB_MatrixOperations()
{
    try
    {
        B.removeRow();

        updateMatrixB_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::removeColumnFromMatrixB_MatrixOperations()
{
    try
    {
        B.removeColumn();

        updateMatrixB_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::randomizeB_MatrixOperations()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            B.randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::createMatrixB_MatrixOperations()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<SetMatrixSize> dialog = std::make_unique<SetMatrixSize>(this, ok.get());
        dialog->exec();

        if (*ok)
            B.reset(dialog->getRowsNumber(), dialog->getColumnsNumber());

        updateMatrixB_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::setCellB_MatrixOperations(int rowIndex, int columnIndex)
{
    B[rowIndex][columnIndex] = ui->table_B_matrix_operations->item(rowIndex, columnIndex)->text().toDouble();
}

void MainWindow::showError(const std::string& message)
{
    QMessageBox messageBox;
    messageBox.critical(this, "Error", QString::fromUtf8(message.c_str()));
    messageBox.show();
}

void MainWindow::updateMatrixA_MatrixOperations()
{
    ui->table_A_matrix_operations->setRowCount(A.getNumRows());
    ui->table_A_matrix_operations->setColumnCount(A.getNumColumns());

    for (size_t i = 0; i < A.getNumRows(); ++i)
    {
        for (size_t j = 0; j < A.getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(A[i][j]));
            ui->table_A_matrix_operations->setItem(i, j, item);
        }
    }
}

void MainWindow::updateMatrixB_MatrixOperations()
{
    ui->table_B_matrix_operations->setRowCount(B.getNumRows());
    ui->table_B_matrix_operations->setColumnCount(B.getNumColumns());

    for (size_t i = 0; i < B.getNumRows(); ++i)
    {
        for (size_t j = 0; j < B.getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(B[i][j]));
            ui->table_B_matrix_operations->setItem(i, j, item);
        }
    }
}

void MainWindow::updateMatrixC_MatrixOperations()
{
    ui->table_C_matrix_operations->setRowCount(C.getNumRows());
    ui->table_C_matrix_operations->setColumnCount(C.getNumColumns());

    for (size_t i = 0; i < C.getNumRows(); ++i)
    {
        for (size_t j = 0; j < C.getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(C[i][j]));
            ui->table_C_matrix_operations->setItem(i, j, item);
        }
    }
}

void MainWindow::createMenubarForMatrixOperations()
{
    createMatrixAMenu_MatrixOperations();
//    createMatrixAToolset(ui->push_button_sle_solver_matrix_A);
//    createMatrixBToolset(ui->tab_matrix_operations);
    //createMatrixResultToolset(ui->tab_matrix_operations);
}

void MainWindow::createMenubarForSLESolver()
{

}

void MainWindow::createMatrixAMenu_MatrixOperations()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_A_matrix_operations");
    matrixToolset->addAction(actions["load"]);
    matrixToolset->addAction(actions["save"]);
    matrixToolset->addSeparator();
    matrixToolset->addAction(actions["add_new_row"]);
    matrixToolset->addAction(actions["add_new_column"]);
    matrixToolset->addAction(actions["remove_row"]);
    matrixToolset->addAction(actions["remove_column"]);
    matrixToolset->addSeparator();
    matrixToolset->addAction(actions["randomize"]);
    matrixToolset->addAction(actions["create_matrix"]);

    connect(actions["load"], &QAction::triggered, this, &MainWindow::loadMatrixA_MatrixOperations);
    connect(actions["save"], &QAction::triggered, this, &MainWindow::saveMatrixA_MatrixOperations);
    connect(actions["add_new_row"], &QAction::triggered, this, &MainWindow::addRowToMatrixA_MatrixOperations);
    connect(actions["add_new_column"], &QAction::triggered, this, &MainWindow::addColumnToMatrixA_MatrixOperations);
    connect(actions["remove_row"], &QAction::triggered, this, &MainWindow::removeRowFromMatrixA_MatrixOperations);
    connect(actions["remove_column"], &QAction::triggered, this, &MainWindow::removeColumnFromMatrixA_MatrixOperations);
    connect(actions["randomize"], &QAction::triggered, this, &MainWindow::randomizeA_MatrixOperations);
    connect(actions["create_matrix"], &QAction::triggered, this, &MainWindow::createMatrixA_MatrixOperations);

    ui->push_button_matrix_A_matrix_operations->setMenu(matrixToolset);
}

void MainWindow::createMatrixBMenu_MatrixOperations()
{
//    QFont font;
//    font.setPointSize(11);

//    QAction* action_load_B = new QAction();
//    action_load_B->setObjectName("action_load_B");
//    action_load_B->setText(QCoreApplication::translate("MainWindow", "Load matrix", nullptr));
//    action_load_B->setFont(font);

//    QAction* action_save_B = new QAction();
//    action_save_B->setObjectName("action_save_B");
//    action_save_B->setText(QCoreApplication::translate("MainWindow", "Save matrix", nullptr));
//    action_save_B->setFont(font);

//    QAction* action_add_new_row_to_B = new QAction();
//    action_add_new_row_to_B->setObjectName("action_add_new_row_to_B");
//    action_add_new_row_to_B->setText(QCoreApplication::translate("MainWindow", "Add new row", nullptr));
//    action_add_new_row_to_B->setFont(font);

//    QAction* action_add_new_column_to_B = new QAction();
//    action_add_new_column_to_B->setObjectName("action_add_new_column_to_B");
//    action_add_new_column_to_B->setText(QCoreApplication::translate("MainWindow", "Add new column", nullptr));
//    action_add_new_column_to_B->setFont(font);

//    QAction* action_remove_row_from_B = new QAction();
//    action_remove_row_from_B->setObjectName("action_remove_row_from_B");
//    action_remove_row_from_B->setText(QCoreApplication::translate("MainWindow", "Remove row", nullptr));
//    action_remove_row_from_B->setFont(font);

//    QAction* action_remove_column_from_B = new QAction();
//    action_remove_column_from_B->setObjectName("action_remove_column_from_B");
//    action_remove_column_from_B->setText(QCoreApplication::translate("MainWindow", "Remove column", nullptr));
//    action_remove_column_from_B->setFont(font);

//    QAction* action_randomize_B = new QAction();
//    action_randomize_B->setObjectName("action_randomize_B");
//    action_randomize_B->setText(QCoreApplication::translate("MainWindow", "Randomize...", nullptr));
//    action_randomize_B->setFont(font);

//    QAction* action_create_matrix_B = new QAction();
//    action_create_matrix_B->setObjectName("action_create_matrix_B");
//    action_create_matrix_B->setText(QCoreApplication::translate("MainWindow", "Create matrix...", nullptr));
//    action_create_matrix_B->setFont(font);

//    QMenu* toolbar_matrix_operations_matrix_B = new QMenu();
//    toolbar_matrix_operations_matrix_B->addAction(action_load_B);
//    toolbar_matrix_operations_matrix_B->addAction(action_save_B);
//    toolbar_matrix_operations_matrix_B->addSeparator();
//    toolbar_matrix_operations_matrix_B->addAction(action_add_new_row_to_B);
//    toolbar_matrix_operations_matrix_B->addAction(action_add_new_column_to_B);
//    toolbar_matrix_operations_matrix_B->addAction(action_remove_row_from_B);
//    toolbar_matrix_operations_matrix_B->addAction(action_remove_column_from_B);
//    toolbar_matrix_operations_matrix_B->addSeparator();
//    toolbar_matrix_operations_matrix_B->addAction(action_randomize_B);
//    toolbar_matrix_operations_matrix_B->addAction(action_create_matrix_B);

    //    ui->push_button_matrix_B_matrix_operations->setMenu(toolbar_matrix_operations_matrix_B);
}

void MainWindow::createMatrixCMenu_MatrixOperations()
{

}
