#include "matrix_operations_tab.h"

#include "set_matrix_size.h"
#include "randomization.h"
#include "helpers.h"

#include <QInputDialog>
#include <QTableWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMenu>

namespace
{
std::map<std::string, QAction*> createMatrixToolset()
{
    QFont font;
    font.setPointSize(11);

    QAction* action_load = new QAction();
    action_load->setObjectName("action_load");
    action_load->setText("Load matrix");
    action_load->setFont(font);

    QAction* action_save = new QAction();
    action_save->setObjectName("action_save");
    action_save->setText("Save matrix");
    action_save->setFont(font);

    QAction* action_add_new_row = new QAction();
    action_add_new_row->setObjectName("action_add_new_row");
    action_add_new_row->setText("Add new row");
    action_add_new_row->setFont(font);

    QAction* action_add_new_column = new QAction();
    action_add_new_column->setObjectName("action_add_new_column");
    action_add_new_column->setText("Add new column");
    action_add_new_column->setFont(font);

    QAction* action_remove_row = new QAction();
    action_remove_row->setObjectName("action_remove_row");
    action_remove_row->setText("Remove row");
    action_remove_row->setFont(font);

    QAction* action_remove_column = new QAction();
    action_remove_column->setObjectName("action_remove_column");
    action_remove_column->setText("Remove column");
    action_remove_column->setFont(font);

    QAction* action_randomize = new QAction();
    action_randomize->setObjectName("action_randomize");
    action_randomize->setText("Randomize...");
    action_randomize->setFont(font);

    QAction* action_create_matrix = new QAction();
    action_create_matrix->setObjectName("action_create_matrix");
    action_create_matrix->setText("Create matrix...");
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

MatrixOperationsTab::MatrixOperationsTab(QWidget *parent)
    : QWidget(parent)
{
    layout = new QWidget(this);
    layout->setObjectName("matrix_operations_tab_layout");
    QRect geometry = parent->geometry();
    geometry.setHeight(geometry.height() - 30);
    layout->setGeometry(geometry);

    QGridLayout* gridLayout = new QGridLayout(layout);
    gridLayout->setObjectName("grid");

    QFont font;
    font.setPointSize(12);

    QLabel* label_matrix_A = new QLabel(layout);
    label_matrix_A->setObjectName("label_matrix_A");
    label_matrix_A->setText("Matrix A:");
    label_matrix_A->setFont(font);

    QLabel* label_matrix_B = new QLabel(layout);
    label_matrix_B->setObjectName("label_matrix_B");
    label_matrix_B->setText("Matrix B:");
    label_matrix_B->setFont(font);

    QLabel* label_matrix_C = new QLabel(layout);
    label_matrix_C->setObjectName("label_matrix_C");
    label_matrix_C->setText("Matrix C:");
    label_matrix_C->setFont(font);

    QTableWidget* table_matrix_A = new QTableWidget(layout);
    table_matrix_A->setObjectName("table_matrix_A");
    table_matrix_A->setFont(font);

    QTableWidget* table_matrix_B = new QTableWidget(layout);
    table_matrix_B->setObjectName("table_matrix_B");
    table_matrix_B->setFont(font);

    QTableWidget* table_matrix_C = new QTableWidget(layout);
    table_matrix_C->setObjectName("table_matrix_C");
    table_matrix_C->setFont(font);

    QPushButton* button_matrix_A = new QPushButton(layout);
    button_matrix_A->setObjectName("button_matrix_A");
    button_matrix_A->setText("Matrix A");
    button_matrix_A->setFont(font);
    createMatrixAMenu();

    QPushButton* button_matrix_B = new QPushButton(layout);
    button_matrix_B->setObjectName("button_matrix_B");
    button_matrix_B->setText("Matrix B");
    button_matrix_B->setFont(font);
    createMatrixBMenu();

    QPushButton* button_matrix_C = new QPushButton(layout);
    button_matrix_C->setObjectName("button_matrix_C");
    button_matrix_C->setText("Result");
    button_matrix_C->setFont(font);
    createMatrixCMenu();

    gridLayout->addWidget(label_matrix_A, 0, 0, 1, 1);
    gridLayout->addWidget(label_matrix_B, 0, 1, 1, 1);
    gridLayout->addWidget(label_matrix_C, 0, 2, 1, 1);
    gridLayout->addWidget(table_matrix_A, 1, 0, 1, 1);
    gridLayout->addWidget(table_matrix_B, 1, 1, 1, 1);
    gridLayout->addWidget(table_matrix_C, 1, 2, 1, 1);
    gridLayout->addWidget(button_matrix_A, 2, 0, 1, 1);
    gridLayout->addWidget(button_matrix_B, 2, 1, 1, 1);
    gridLayout->addWidget(button_matrix_C, 2, 2, 1, 1);
}

MatrixOperationsTab::~MatrixOperationsTab()
{
    delete layout;
}

void MatrixOperationsTab::loadMatrixA()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            A = std::make_unique<Matrix>(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::saveMatrixA()
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

void MatrixOperationsTab::addRowToMatrixA()
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
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::addColumnToMatrixA()
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
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::removeRowFromMatrixA()
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
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::removeColumnFromMatrixA()
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
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::randomizeA()
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

        updateMatrixA();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::createMatrixA()
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

            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::setCellA(int rowIndex, int columnIndex)
{
    A->at(rowIndex, columnIndex) = layout->findChild<QTableWidget*>("table_matrix_A")->item(rowIndex, columnIndex)->text().toDouble();
}

void MatrixOperationsTab::loadMatrixB()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            B = std::make_unique<Matrix>(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::saveMatrixB()
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

void MatrixOperationsTab::addRowToMatrixB()
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
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::addColumnToMatrixB()
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
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::removeRowFromMatrixB()
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
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::removeColumnFromMatrixB()
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
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::randomizeB()
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
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::createMatrixB()
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

        updateMatrixB();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::setCellB(int rowIndex, int columnIndex)
{
    B->at(rowIndex, columnIndex) = layout->findChild<QTableWidget*>("table_matrix_B")->item(rowIndex, columnIndex)->text().toDouble();
}

void MatrixOperationsTab::saveMatrixC()
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

void MatrixOperationsTab::addMatrices()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        C = std::make_unique<Matrix>((*A) + (*B));

        updateMatrixC();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::subtractMatrices()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        C = std::make_unique<Matrix>((*A) - (*B));

        updateMatrixC();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::multiplyMatrices()
{
    try
    {
        if (!A.get())
            throw std::runtime_error("Matrix A does not exist");
        if (!B.get())
            throw std::runtime_error("Matrix B does not exist");

        C = std::make_unique<Matrix>((*A) * (*B));

        updateMatrixC();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void MatrixOperationsTab::updateMatrixA()
{
    layout->findChild<QTableWidget*>("table_matrix_A")->setRowCount(A->getNumRows());
    layout->findChild<QTableWidget*>("table_matrix_A")->setColumnCount(A->getNumColumns());

    for (size_t i = 0; i < A->getNumRows(); ++i)
    {
        for (size_t j = 0; j < A->getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg((A->at(i, j))));
            layout->findChild<QTableWidget*>("table_matrix_A")->setItem(i, j, item);
        }
    }
}

void MatrixOperationsTab::updateMatrixB()
{
    layout->findChild<QTableWidget*>("table_matrix_B")->setRowCount(B->getNumRows());
    layout->findChild<QTableWidget*>("table_matrix_B")->setColumnCount(B->getNumColumns());

    for (size_t i = 0; i < B->getNumRows(); ++i)
    {
        for (size_t j = 0; j < B->getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(B->at(i, j)));
            layout->findChild<QTableWidget*>("table_matrix_B")->setItem(i, j, item);
        }
    }
}

void MatrixOperationsTab::updateMatrixC()
{
    layout->findChild<QTableWidget*>("table_matrix_C")->setRowCount(C->getNumRows());
    layout->findChild<QTableWidget*>("table_matrix_C")->setColumnCount(C->getNumColumns());

    for (size_t i = 0; i < C->getNumRows(); ++i)
    {
        for (size_t j = 0; j < C->getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(C->at(i, j)));
            layout->findChild<QTableWidget*>("table_matrix_C")->setItem(i, j, item);
        }
    }
}

void MatrixOperationsTab::createMatrixAMenu()
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

    connect(actions["load"], &QAction::triggered, this, &MatrixOperationsTab::loadMatrixA);
    connect(actions["save"], &QAction::triggered, this, &MatrixOperationsTab::saveMatrixA);
    connect(actions["add_new_row"], &QAction::triggered, this, &MatrixOperationsTab::addRowToMatrixA);
    connect(actions["add_new_column"], &QAction::triggered, this, &MatrixOperationsTab::addColumnToMatrixA);
    connect(actions["remove_row"], &QAction::triggered, this, &MatrixOperationsTab::removeRowFromMatrixA);
    connect(actions["remove_column"], &QAction::triggered, this, &MatrixOperationsTab::removeColumnFromMatrixA);
    connect(actions["randomize"], &QAction::triggered, this, &MatrixOperationsTab::randomizeA);
    connect(actions["create_matrix"], &QAction::triggered, this, &MatrixOperationsTab::createMatrixA);
    connect(layout->findChild<QTableWidget*>("table_matrix_A"), &QTableWidget::cellChanged, this, &MatrixOperationsTab::setCellA);

    layout->findChild<QPushButton*>("button_matrix_A")->setMenu(matrixToolset);
}

void MatrixOperationsTab::createMatrixBMenu()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu(layout);
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

    connect(actions["load"], &QAction::triggered, this, &MatrixOperationsTab::loadMatrixB);
    connect(actions["save"], &QAction::triggered, this, &MatrixOperationsTab::saveMatrixB);
    connect(actions["add_new_row"], &QAction::triggered, this, &MatrixOperationsTab::addRowToMatrixB);
    connect(actions["add_new_column"], &QAction::triggered, this, &MatrixOperationsTab::addColumnToMatrixB);
    connect(actions["remove_row"], &QAction::triggered, this, &MatrixOperationsTab::removeRowFromMatrixB);
    connect(actions["remove_column"], &QAction::triggered, this, &MatrixOperationsTab::removeColumnFromMatrixB);
    connect(actions["randomize"], &QAction::triggered, this, &MatrixOperationsTab::randomizeB);
    connect(actions["create_matrix"], &QAction::triggered, this, &MatrixOperationsTab::createMatrixB);
    connect(layout->findChild<QTableWidget*>("table_matrix_B"), &QTableWidget::cellChanged, this, &MatrixOperationsTab::setCellB);

    layout->findChild<QPushButton*>("button_matrix_B")->setMenu(matrixToolset);
}

void MatrixOperationsTab::createMatrixCMenu()
{
    QFont font;
    font.setPointSize(11);

    QAction* action_save = new QAction(layout);
    action_save->setObjectName("action_save");
    action_save->setText("Save matrix");
    action_save->setFont(font);

    QAction* action_add = new QAction(layout);
    action_add->setObjectName("action_add");
    action_add->setText("Add matrices");
    action_add->setFont(font);

    QAction* action_subtract = new QAction(layout);
    action_subtract->setObjectName("action_subtract");
    action_subtract->setText("Subtract matrices");
    action_subtract->setFont(font);

    QAction* action_multiply = new QAction(layout);
    action_multiply->setObjectName("action_multiply");
    action_multiply->setText("Multiply matrices");
    action_multiply->setFont(font);

    QMenu* matrixToolset = new QMenu(layout);
    matrixToolset->setObjectName("menu_matrix_C_matrix_operations");
    matrixToolset->addAction(action_save);
    matrixToolset->addSeparator();
    matrixToolset->addAction(action_add);
    matrixToolset->addAction(action_subtract);
    matrixToolset->addAction(action_multiply);

    connect(action_save, &QAction::triggered, this, &MatrixOperationsTab::saveMatrixC);
    connect(action_add, &QAction::triggered, this, &MatrixOperationsTab::addMatrices);
    connect(action_subtract, &QAction::triggered, this, &MatrixOperationsTab::subtractMatrices);
    connect(action_multiply, &QAction::triggered, this, &MatrixOperationsTab::multiplyMatrices);

    layout->findChild<QPushButton*>("button_matrix_C")->setMenu(matrixToolset);

    layout->findChild<QTableWidget*>("table_matrix_C")->setEditTriggers(QTableWidget::NoEditTriggers);
}
