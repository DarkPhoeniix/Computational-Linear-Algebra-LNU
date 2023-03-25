#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "randomization.h"
#include "set_matrix_size.h"
#include "helpers.h"

#include <QInputDialog>
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

    createMatrixAMenu_MatrixOperations();
    createMatrixBMenu_MatrixOperations();
    createMatrixCMenu_MatrixOperations();

    createMatrixAMenu_SLESolver();
    createMatrixBMenu_SLESolver();
    createVectorXMenu_SLESolver();
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
            A = std::make_unique<Matrix>(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::saveMatrixA_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");

        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(*A, filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addRowToMatrixA_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, A->getNumRows(), 1, ok.get());

        if (*ok)
        {
            A->addRow(index - 1);
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addColumnToMatrixA_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, A->getNumColumns(), 1, ok.get());

        if (*ok)
        {
            A->addColumn(index - 1);
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeRowFromMatrixA_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, A->getNumRows(), 1, ok.get());

        if (*ok)
        {
            A->removeRow(index - 1);
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeColumnFromMatrixA_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, A->getNumColumns(), 1, ok.get());

        if (*ok)
        {
            A->removeColumn(index - 1);
            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::randomizeA_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");

        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
            A->randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());

        updateMatrixA_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
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
            if (!A.get())
                A = std::make_unique<Matrix>(dialog->getRowsNumber(), dialog->getColumnsNumber());
            else
                A->reset(dialog->getRowsNumber(), dialog->getColumnsNumber());

            updateMatrixA_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::setCellA_MatrixOperations(int rowIndex, int columnIndex)
{
    A->at(rowIndex, columnIndex) = ui->table_A_matrix_operations->item(rowIndex, columnIndex)->text().toDouble();
}

void MainWindow::loadMatrixB_MatrixOperations()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            B = std::make_unique<Matrix>(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::saveMatrixB_MatrixOperations()
{
    try
    {
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(*B, filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addRowToMatrixB_MatrixOperations()
{
    try
    {
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, B->getNumRows(), 1, ok.get());

        if (*ok)
        {
            B->addRow(index - 1);
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addColumnToMatrixB_MatrixOperations()
{
    try
    {
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, B->getNumColumns(), 1, ok.get());

        if (*ok)
        {
            B->addColumn(index - 1);
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeRowFromMatrixB_MatrixOperations()
{
    try
    {
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, B->getNumRows(), 1, ok.get());

        if (*ok)
        {
            B->removeRow(index - 1);
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeColumnFromMatrixB_MatrixOperations()
{
    try
    {
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, B->getNumColumns(), 1, ok.get());

        if (*ok)
        {
            B->removeColumn(index - 1);
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::randomizeB_MatrixOperations()
{
    try
    {
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            B->randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());
            updateMatrixB_MatrixOperations();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
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
        {
            if (!B.get())
                B = std::make_unique<Matrix>(dialog->getRowsNumber(), dialog->getColumnsNumber());
            else
                B->reset(dialog->getRowsNumber(), dialog->getColumnsNumber());
        }

        updateMatrixB_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::setCellB_MatrixOperations(int rowIndex, int columnIndex)
{
    B->at(rowIndex, columnIndex) = ui->table_B_matrix_operations->item(rowIndex, columnIndex)->text().toDouble();
}

void MainWindow::saveMatrixC_MatrixOperations()
{
    try
    {
        if (!C.get())
            throw std::runtime_error("Matrix Result does not exist");

        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(*C, filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addMatrices_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        C = std::make_unique<Matrix>((*A) + (*B));

        updateMatrixC_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::subtractMatrices_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        C = std::make_unique<Matrix>((*A) - (*B));

        updateMatrixC_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::multiplyMatrices_MatrixOperations()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        C = std::make_unique<Matrix>((*A) * (*B));

        updateMatrixC_MatrixOperations();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::loadMatrixA_SLESolver()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            sle.setMatrixA(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixA_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::saveMatrixA_SLESolver()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(sle.getMatrixA(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addRowToMatrixA_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixA().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixA().addRow(index - 1);
            updateMatrixA_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addColumnToMatrixA_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixA().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixA().addColumn(index - 1);
            updateMatrixA_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeRowFromMatrixA_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixA().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixA().removeRow(index - 1);
            updateMatrixA_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeColumnFromMatrixA_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixA().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixA().removeColumn(index - 1);
            updateMatrixA_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::randomizeA_SLESolver()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
            sle.getMatrixA().randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());

        updateMatrixA_SLESolver();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::createMatrixA_SLESolver()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<SetMatrixSize> dialog = std::make_unique<SetMatrixSize>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            sle.setMatrixA(Matrix(dialog->getRowsNumber(), dialog->getColumnsNumber()));
            updateMatrixA_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::setCellA_SLESolver(int rowIndex, int columnIndex)
{
    sle.getMatrixA()[rowIndex][columnIndex] = ui->table_A_sle_solver->item(rowIndex, columnIndex)->text().toDouble();
}

void MainWindow::loadMatrixB_SLESolver()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            sle.setMatrixB(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixB_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::saveMatrixB_SLESolver()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(sle.getMatrixB(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addRowToMatrixB_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixB().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixB().addRow(index - 1);
            updateMatrixB_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::addColumnToMatrixB_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixB().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixB().addColumn(index - 1);
            updateMatrixB_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeRowFromMatrixB_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixB().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixB().removeRow(index - 1);
            updateMatrixB_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::removeColumnFromMatrixB_SLESolver()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle.getMatrixB().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle.getMatrixB().removeColumn(index - 1);
            updateMatrixB_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::randomizeB_SLESolver()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            sle.getMatrixB().randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());
            updateMatrixB_SLESolver();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::createMatrixB_SLESolver()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<SetMatrixSize> dialog = std::make_unique<SetMatrixSize>(this, ok.get());
        dialog->exec();

        if (*ok)
            sle.setMatrixB(Matrix(dialog->getRowsNumber(), dialog->getColumnsNumber()));

        updateMatrixB_SLESolver();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::setCellB_SLESolver(int rowIndex, int columnIndex)
{
    sle.getMatrixB()[rowIndex][columnIndex] = ui->table_B_sle_solver->item(rowIndex, columnIndex)->text().toDouble();
}

void MainWindow::saveVectorX_SLESolver()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Vector::writeToFile(sle.getVectorX(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::solveVectorX_SLESolver()
{
    try
    {
        sle.solve();

        updateVectorX_SLESolver();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MainWindow::updateMatrixA_MatrixOperations()
{
    ui->table_A_matrix_operations->setRowCount(A->getNumRows());
    ui->table_A_matrix_operations->setColumnCount(A->getNumColumns());

    for (size_t i = 0; i < A->getNumRows(); ++i)
    {
        for (size_t j = 0; j < A->getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg((A->at(i, j))));
            ui->table_A_matrix_operations->setItem(i, j, item);
        }
    }
}

void MainWindow::updateMatrixB_MatrixOperations()
{
    ui->table_B_matrix_operations->setRowCount(B->getNumRows());
    ui->table_B_matrix_operations->setColumnCount(B->getNumColumns());

    for (size_t i = 0; i < B->getNumRows(); ++i)
    {
        for (size_t j = 0; j < B->getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(B->at(i, j)));
            ui->table_B_matrix_operations->setItem(i, j, item);
        }
    }
}

void MainWindow::updateMatrixC_MatrixOperations()
{
    ui->table_C_matrix_operations->setRowCount(C->getNumRows());
    ui->table_C_matrix_operations->setColumnCount(C->getNumColumns());

    for (size_t i = 0; i < C->getNumRows(); ++i)
    {
        for (size_t j = 0; j < C->getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(C->at(i, j)));
            ui->table_C_matrix_operations->setItem(i, j, item);
        }
    }
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
    connect(ui->table_A_matrix_operations, &QTableWidget::cellChanged, this, &MainWindow::setCellA_MatrixOperations);

    ui->push_button_matrix_A_matrix_operations->setMenu(matrixToolset);
}

void MainWindow::createMatrixBMenu_MatrixOperations()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_B_matrix_operations");
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

    connect(actions["load"], &QAction::triggered, this, &MainWindow::loadMatrixB_MatrixOperations);
    connect(actions["save"], &QAction::triggered, this, &MainWindow::saveMatrixB_MatrixOperations);
    connect(actions["add_new_row"], &QAction::triggered, this, &MainWindow::addRowToMatrixB_MatrixOperations);
    connect(actions["add_new_column"], &QAction::triggered, this, &MainWindow::addColumnToMatrixB_MatrixOperations);
    connect(actions["remove_row"], &QAction::triggered, this, &MainWindow::removeRowFromMatrixB_MatrixOperations);
    connect(actions["remove_column"], &QAction::triggered, this, &MainWindow::removeColumnFromMatrixB_MatrixOperations);
    connect(actions["randomize"], &QAction::triggered, this, &MainWindow::randomizeB_MatrixOperations);
    connect(actions["create_matrix"], &QAction::triggered, this, &MainWindow::createMatrixB_MatrixOperations);
    connect(ui->table_B_matrix_operations, &QTableWidget::cellChanged, this, &MainWindow::setCellB_MatrixOperations);

    ui->push_button_matrix_B_matrix_operations->setMenu(matrixToolset);
}

void MainWindow::createMatrixCMenu_MatrixOperations()
{
    QFont font;
    font.setPointSize(11);

    QAction* action_save = new QAction();
    action_save->setObjectName("action_save");
    action_save->setText(QCoreApplication::translate("MainWindow", "Save matrix", nullptr));
    action_save->setFont(font);

    QAction* action_add = new QAction();
    action_add->setObjectName("action_add");
    action_add->setText(QCoreApplication::translate("MainWindow", "Add matrices", nullptr));
    action_add->setFont(font);

    QAction* action_subtract = new QAction();
    action_subtract->setObjectName("action_subtract");
    action_subtract->setText(QCoreApplication::translate("MainWindow", "Subtract matrices", nullptr));
    action_subtract->setFont(font);

    QAction* action_multiply = new QAction();
    action_multiply->setObjectName("action_multiply");
    action_multiply->setText(QCoreApplication::translate("MainWindow", "Multiply matrices", nullptr));
    action_multiply->setFont(font);

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_C_matrix_operations");
    matrixToolset->addAction(action_save);
    matrixToolset->addSeparator();
    matrixToolset->addAction(action_add);
    matrixToolset->addAction(action_subtract);
    matrixToolset->addAction(action_multiply);

    connect(action_save, &QAction::triggered, this, &MainWindow::saveMatrixC_MatrixOperations);
    connect(action_add, &QAction::triggered, this, &MainWindow::addMatrices_MatrixOperations);
    connect(action_subtract, &QAction::triggered, this, &MainWindow::subtractMatrices_MatrixOperations);
    connect(action_multiply, &QAction::triggered, this, &MainWindow::multiplyMatrices_MatrixOperations);

    ui->push_button_matrix_C_matrix_operations->setMenu(matrixToolset);

    ui->table_C_matrix_operations->setEditTriggers(QTableWidget::NoEditTriggers);
}

void MainWindow::updateMatrixA_SLESolver()
{
    ui->table_A_sle_solver->setRowCount(sle.getMatrixA().getNumRows());
    ui->table_A_sle_solver->setColumnCount(sle.getMatrixA().getNumColumns());

    for (size_t i = 0; i < sle.getMatrixA().getNumRows(); ++i)
    {
        for (size_t j = 0; j < sle.getMatrixA().getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle.getMatrixA()[i][j]));
            ui->table_A_sle_solver->setItem(i, j, item);
        }
    }
}

void MainWindow::updateMatrixB_SLESolver()
{
    ui->table_B_sle_solver->setRowCount(sle.getMatrixB().getNumRows());
    ui->table_B_sle_solver->setColumnCount(sle.getMatrixB().getNumColumns());

    for (size_t i = 0; i < sle.getMatrixB().getNumRows(); ++i)
    {
        for (size_t j = 0; j < sle.getMatrixB().getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle.getMatrixB()[i][j]));
            ui->table_B_sle_solver->setItem(i, j, item);
        }
    }
}

void MainWindow::updateVectorX_SLESolver()
{
    ui->table_Solution_sle_solver->setRowCount(sle.getVectorX().size());
    ui->table_Solution_sle_solver->setColumnCount(1);

    for (size_t i = 0; i < sle.getVectorX().size(); ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle.getVectorX()[i]));
        ui->table_Solution_sle_solver->setItem(i, 0, item);
    }
}

void MainWindow::createMatrixAMenu_SLESolver()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_A_sle_solver");
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

    connect(actions["load"], &QAction::triggered, this, &MainWindow::loadMatrixA_SLESolver);
    connect(actions["save"], &QAction::triggered, this, &MainWindow::saveMatrixA_SLESolver);
    connect(actions["add_new_row"], &QAction::triggered, this, &MainWindow::addRowToMatrixA_SLESolver);
    connect(actions["add_new_column"], &QAction::triggered, this, &MainWindow::addColumnToMatrixA_SLESolver);
    connect(actions["remove_row"], &QAction::triggered, this, &MainWindow::removeRowFromMatrixA_SLESolver);
    connect(actions["remove_column"], &QAction::triggered, this, &MainWindow::removeColumnFromMatrixA_SLESolver);
    connect(actions["randomize"], &QAction::triggered, this, &MainWindow::randomizeA_SLESolver);
    connect(actions["create_matrix"], &QAction::triggered, this, &MainWindow::createMatrixA_SLESolver);
    connect(ui->table_A_sle_solver, &QTableWidget::cellChanged, this, &MainWindow::setCellA_SLESolver);

    ui->push_button_matrix_A_sle_solver->setMenu(matrixToolset);
}

void MainWindow::createMatrixBMenu_SLESolver()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_B_sle_solver");
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

    connect(actions["load"], &QAction::triggered, this, &MainWindow::loadMatrixB_SLESolver);
    connect(actions["save"], &QAction::triggered, this, &MainWindow::saveMatrixB_SLESolver);
    connect(actions["add_new_row"], &QAction::triggered, this, &MainWindow::addRowToMatrixB_SLESolver);
    connect(actions["add_new_column"], &QAction::triggered, this, &MainWindow::addColumnToMatrixB_SLESolver);
    connect(actions["remove_row"], &QAction::triggered, this, &MainWindow::removeRowFromMatrixB_SLESolver);
    connect(actions["remove_column"], &QAction::triggered, this, &MainWindow::removeColumnFromMatrixB_SLESolver);
    connect(actions["randomize"], &QAction::triggered, this, &MainWindow::randomizeB_SLESolver);
    connect(actions["create_matrix"], &QAction::triggered, this, &MainWindow::createMatrixB_SLESolver);
    connect(ui->table_B_sle_solver, &QTableWidget::cellChanged, this, &MainWindow::setCellB_SLESolver);

    ui->push_button_matrix_B_sle_solver->setMenu(matrixToolset);
}

void MainWindow::createVectorXMenu_SLESolver()
{
    QFont font;
    font.setPointSize(11);

    QAction* action_solve = new QAction();
    action_solve->setObjectName("action_solve");
    action_solve->setText(QCoreApplication::translate("MainWindow", "Solve", nullptr));
    action_solve->setFont(font);

    QAction* action_save = new QAction();
    action_save->setObjectName("action_save");
    action_save->setText(QCoreApplication::translate("MainWindow", "Save vector", nullptr));
    action_save->setFont(font);

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_B_sle_solver");
    matrixToolset->addAction(action_solve);
    matrixToolset->addAction(action_save);

    connect(action_solve, &QAction::triggered, this, &MainWindow::solveVectorX_SLESolver);
    connect(action_save, &QAction::triggered, this, &MainWindow::saveVectorX_SLESolver);

    ui->push_button_solution_sle_solver->setMenu(matrixToolset);

    ui->table_Solution_sle_solver->setEditTriggers(QTableWidget::NoEditTriggers);
}
