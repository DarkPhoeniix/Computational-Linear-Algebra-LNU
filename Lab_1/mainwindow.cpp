#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "randomization.h"

#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QAction>
#include <QLayout>

#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->action_load_A, &QAction::triggered, this, &MainWindow::loadMatrixA);
    connect(ui->action_save_A, &QAction::triggered, this, &MainWindow::saveMatrixA);
    connect(ui->action_add_new_row_to_A, &QAction::triggered, this, &MainWindow::addRowToMatrixA);
    connect(ui->action_add_new_column_to_A, &QAction::triggered, this, &MainWindow::addColumnToMatrixA);
    connect(ui->action_remove_row_from_A, &QAction::triggered, this, &MainWindow::removeRowFromMatrixA);
    connect(ui->action_remove_column_from_A, &QAction::triggered, this, &MainWindow::removeColumnFromMatrixA);
    connect(ui->action_randomize_A, &QAction::triggered, this, &MainWindow::randomizeA);

    connect(ui->action_load_B, &QAction::triggered, this, &MainWindow::loadMatrixB);
    connect(ui->action_save_B, &QAction::triggered, this, &MainWindow::saveMatrixB);
    connect(ui->action_add_new_row_to_B, &QAction::triggered, this, &MainWindow::addRowToMatrixB);
    connect(ui->action_remove_row_from_B, &QAction::triggered, this, &MainWindow::removeRowFromMatrixB);
    connect(ui->action_randomize_B, &QAction::triggered, this, &MainWindow::randomizeB);

    connect(ui->push_button_solve, &QPushButton::clicked, this, &MainWindow::solve);

    ui->table_x->setEditTriggers(QTableWidget::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMatrixA()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
            sle.setMatrixA(Matrix::readFromFile(filepath.toStdString()));
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixA();
}

void MainWindow::saveMatrixA()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(sle.getMatrixA(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::addRowToMatrixA()
{
    try
    {
        sle.getMatrixA().addRow();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixA();
}

void MainWindow::addColumnToMatrixA()
{
    try
    {
        sle.getMatrixA().addColumn();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixA();
}

void MainWindow::removeRowFromMatrixA()
{
    try
    {
        sle.getMatrixA().removeRow();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixA();
}

void MainWindow::removeColumnFromMatrixA()
{
    try
    {
        sle.getMatrixA().removeColumn();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixA();
}

void MainWindow::randomizeA()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
            sle.getMatrixA().randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixA();
}

void MainWindow::loadMatrixB()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
            sle.setMatrixB(Matrix::readFromFile(filepath.toStdString()));
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixB();
}

void MainWindow::saveMatrixB()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(sle.getMatrixB(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }
}

void MainWindow::addRowToMatrixB()
{
    try
    {
        sle.getMatrixB().addRow();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixB();
}

void MainWindow::removeRowFromMatrixB()
{
    try
    {
        sle.getMatrixB().removeRow();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixB();
}

void MainWindow::randomizeB()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
            sle.getMatrixB().randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateMatrixB();
}

void MainWindow::solve()
{
    try
    {
        sle.solve();
    }
    catch(const std::exception& ex)
    {
        showError(ex.what());
    }

    updateVectorX();
}

void MainWindow::showError(const std::string& message)
{
    QMessageBox messageBox;
    messageBox.critical(this, "Error", QString::fromUtf8(message.c_str()));
    messageBox.show();
}

void MainWindow::updateMatrixA()
{
    ui->table_A->setRowCount(sle.getMatrixA().getNumRows());
    ui->table_A->setColumnCount(sle.getMatrixA().getNumColumns());

    for (size_t i = 0; i < sle.getMatrixA().getNumRows(); ++i)
    {
        for (size_t j = 0; j < sle.getMatrixA().getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle.getMatrixA()[i][j]));
            ui->table_A->setItem(i, j, item);
        }
    }
}

void MainWindow::updateMatrixB()
{
    ui->table_B->setRowCount(sle.getMatrixB().getNumRows());
    ui->table_B->setColumnCount(sle.getMatrixB().getNumColumns());

    for (size_t i = 0; i < sle.getMatrixB().getNumRows(); ++i)
    {
        for (size_t j = 0; j < sle.getMatrixB().getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle.getMatrixB()[i][j]));
            ui->table_B->setItem(i, j, item);
        }
    }
}

void MainWindow::updateVectorX()
{
    ui->table_x->setRowCount(sle.getVectorX().getSize());
    ui->table_x->setColumnCount(1);

    for (size_t i = 0; i < sle.getVectorX().getSize(); ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle.getVectorX()[i]));
        ui->table_x->setItem(i, 0, item);
    }
}
