#include "gauss_seidel_method_tab.h"

#include "set_matrix_size.h"
#include "randomization.h"
#include "helpers.h"

#include <QInputDialog>
#include <QTableWidget>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
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

GaussSeidelMethodTab::GaussSeidelMethodTab(QWidget *parent)
    : QWidget(parent)
    , sle(std::make_unique<SLE>())
{
    layout = new QWidget(this);
    layout->setObjectName("gauss_seidel_method_tab_layout");
    QRect geometry = parent->geometry();
    geometry.setHeight(geometry.height() - 30);
    layout->setGeometry(geometry);

    QVBoxLayout* verticalLayout = new QVBoxLayout(layout);
    verticalLayout->setObjectName("grid");

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

    QLabel* label_vector_x = new QLabel(layout);
    label_vector_x->setObjectName("label_vector_x");
    label_vector_x->setText("Vector x:");
    label_vector_x->setFont(font);

    QHBoxLayout* labelsLayout = new QHBoxLayout();
    labelsLayout->setObjectName("labels_layout");
    labelsLayout->addWidget(label_matrix_A, 5);
    labelsLayout->addWidget(label_matrix_B, 2);
    labelsLayout->addWidget(label_vector_x, 2);

    QTableWidget* table_matrix_A = new QTableWidget(layout);
    table_matrix_A->setObjectName("table_matrix_A");
    table_matrix_A->setFont(font);

    QTableWidget* table_matrix_B = new QTableWidget(layout);
    table_matrix_B->setObjectName("table_matrix_B");
    table_matrix_B->setFont(font);

    QTableWidget* table_vector_x = new QTableWidget(layout);
    table_vector_x->setObjectName("table_vector_x");
    table_vector_x->setFont(font);

    QHBoxLayout* tablesLayout = new QHBoxLayout();
    tablesLayout->setObjectName("tables_layout");
    tablesLayout->addWidget(table_matrix_A, 5);
    tablesLayout->addWidget(table_matrix_B, 2);
    tablesLayout->addWidget(table_vector_x, 2);

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

    QPushButton* button_vector_x = new QPushButton(layout);
    button_vector_x->setObjectName("button_vector_x");
    button_vector_x->setText("Result");
    button_vector_x->setFont(font);
    createVectorXMenu();

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setObjectName("buttons_layout");
    buttonsLayout->addWidget(button_matrix_A);
    buttonsLayout->addWidget(button_matrix_B);
    buttonsLayout->addWidget(button_vector_x);

    verticalLayout->addLayout(labelsLayout);
    verticalLayout->addLayout(tablesLayout);
    verticalLayout->addLayout(buttonsLayout);
}

GaussSeidelMethodTab::~GaussSeidelMethodTab()
{
    delete layout;
}

void GaussSeidelMethodTab::loadMatrixA()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            sle->setMatrixA(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::saveMatrixA()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(sle->getMatrixA(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::addRowToMatrixA()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixA().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixA().addRow(index - 1);
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::addColumnToMatrixA()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixA().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixA().addColumn(index - 1);
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::removeRowFromMatrixA()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixA().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixA().removeRow(index - 1);
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::removeColumnFromMatrixA()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixA().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixA().removeColumn(index - 1);
            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::randomizeA()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
            sle->getMatrixA().randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());

        updateMatrixA();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::createMatrixA()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<SetMatrixSize> dialog = std::make_unique<SetMatrixSize>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            sle->setMatrixA(Matrix(dialog->getRowsNumber(), dialog->getColumnsNumber()));

            updateMatrixA();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::setCellA(int rowIndex, int columnIndex)
{
    sle->getMatrixA().at(rowIndex, columnIndex) = layout->findChild<QTableWidget*>("table_matrix_A")->item(rowIndex, columnIndex)->text().toDouble();
}

void GaussSeidelMethodTab::loadMatrixB()
{
    try
    {
        QString filepath = QFileDialog::getOpenFileName(this);

        if (!filepath.isEmpty())
        {
            sle->setMatrixB(Matrix::readFromFile(filepath.toStdString()));
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::saveMatrixB()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Matrix::writeToFile(sle->getMatrixB(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::addRowToMatrixB()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixB().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixB().addRow(index - 1);
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::addColumnToMatrixB()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixB().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixB().addColumn(index - 1);
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::removeRowFromMatrixB()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixB().getNumRows(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixB().removeRow(index - 1);
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::removeColumnFromMatrixB()
{
    try
    {
        auto ok = std::make_unique<bool>(false);
        size_t index = QInputDialog::getInt(this, "Choose position", "Input the index where to insert the new row:", 1, 1, sle->getMatrixB().getNumColumns(), 1, ok.get());

        if (*ok)
        {
            sle->getMatrixB().removeColumn(index - 1);
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::randomizeB()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<RandomizationDialog> dialog = std::make_unique<RandomizationDialog>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            sle->getMatrixB().randomize(dialog->getLeftBorderValue(), dialog->getRightBorderValue());
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::createMatrixB()
{
    try
    {
        std::unique_ptr<bool> ok = std::make_unique<bool>();
        std::unique_ptr<SetMatrixSize> dialog = std::make_unique<SetMatrixSize>(this, ok.get());
        dialog->exec();

        if (*ok)
        {
            sle->setMatrixB(Matrix(dialog->getRowsNumber(), dialog->getColumnsNumber()));
            updateMatrixB();
        }
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::setCellB(int rowIndex, int columnIndex)
{
    sle->getMatrixB().at(rowIndex, columnIndex) = layout->findChild<QTableWidget*>("table_matrix_B")->item(rowIndex, columnIndex)->text().toDouble();
}

void GaussSeidelMethodTab::saveVectorX()
{
    try
    {
        QString filepath = QFileDialog::getSaveFileName(this);

        if (!filepath.isEmpty())
            Vector::writeToFile(sle->getVectorX(), filepath.toStdString());
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::updateMatrixA()
{
    layout->findChild<QTableWidget*>("table_matrix_A")->setRowCount(sle->getMatrixA().getNumRows());
    layout->findChild<QTableWidget*>("table_matrix_A")->setColumnCount(sle->getMatrixA().getNumColumns());

    for (size_t i = 0; i < sle->getMatrixA().getNumRows(); ++i)
    {
        for (size_t j = 0; j < sle->getMatrixA().getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg((sle->getMatrixA().at(i, j))));
            layout->findChild<QTableWidget*>("table_matrix_A")->setItem(i, j, item);
        }
    }
}

void GaussSeidelMethodTab::updateMatrixB()
{
    layout->findChild<QTableWidget*>("table_matrix_B")->setRowCount(sle->getMatrixB().getNumRows());
    layout->findChild<QTableWidget*>("table_matrix_B")->setColumnCount(sle->getMatrixB().getNumColumns());

    for (size_t i = 0; i < sle->getMatrixB().getNumRows(); ++i)
    {
        for (size_t j = 0; j < sle->getMatrixB().getNumColumns(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle->getMatrixB().at(i, j)));
            layout->findChild<QTableWidget*>("table_matrix_B")->setItem(i, j, item);
        }
    }
}

void GaussSeidelMethodTab::updateVectorX()
{
    layout->findChild<QTableWidget*>("table_vector_x")->setRowCount(sle->getVectorX().size());
    layout->findChild<QTableWidget*>("table_vector_x")->setColumnCount(1);

    for (size_t i = 0; i < sle->getVectorX().size(); ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(sle->getVectorX()[i]));
        layout->findChild<QTableWidget*>("table_vector_x")->setItem(i, 0, item);
    }
}

void GaussSeidelMethodTab::solve()
{
    try
    {
        sle->solveGaussSeidelMethod();

        updateVectorX();
    }
    catch(const std::exception& ex)
    {
        helpers::showError(ex.what());
    }
}

void GaussSeidelMethodTab::createMatrixAMenu()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu();
    matrixToolset->setObjectName("menu_matrix_A");
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

    connect(actions["load"], &QAction::triggered, this, &GaussSeidelMethodTab::loadMatrixA);
    connect(actions["save"], &QAction::triggered, this, &GaussSeidelMethodTab::saveMatrixA);
    connect(actions["add_new_row"], &QAction::triggered, this, &GaussSeidelMethodTab::addRowToMatrixA);
    connect(actions["add_new_column"], &QAction::triggered, this, &GaussSeidelMethodTab::addColumnToMatrixA);
    connect(actions["remove_row"], &QAction::triggered, this, &GaussSeidelMethodTab::removeRowFromMatrixA);
    connect(actions["remove_column"], &QAction::triggered, this, &GaussSeidelMethodTab::removeColumnFromMatrixA);
    connect(actions["randomize"], &QAction::triggered, this, &GaussSeidelMethodTab::randomizeA);
    connect(actions["create_matrix"], &QAction::triggered, this, &GaussSeidelMethodTab::createMatrixA);
    connect(layout->findChild<QTableWidget*>("table_matrix_A"), &QTableWidget::cellChanged, this, &GaussSeidelMethodTab::setCellA);

    layout->findChild<QPushButton*>("button_matrix_A")->setMenu(matrixToolset);
}

void GaussSeidelMethodTab::createMatrixBMenu()
{
    auto actions = createMatrixToolset();

    QMenu* matrixToolset = new QMenu(layout);
    matrixToolset->setObjectName("menu_matrix_B");
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

    connect(actions["load"], &QAction::triggered, this, &GaussSeidelMethodTab::loadMatrixB);
    connect(actions["save"], &QAction::triggered, this, &GaussSeidelMethodTab::saveMatrixB);
    connect(actions["add_new_row"], &QAction::triggered, this, &GaussSeidelMethodTab::addRowToMatrixB);
    connect(actions["add_new_column"], &QAction::triggered, this, &GaussSeidelMethodTab::addColumnToMatrixB);
    connect(actions["remove_row"], &QAction::triggered, this, &GaussSeidelMethodTab::removeRowFromMatrixB);
    connect(actions["remove_column"], &QAction::triggered, this, &GaussSeidelMethodTab::removeColumnFromMatrixB);
    connect(actions["randomize"], &QAction::triggered, this, &GaussSeidelMethodTab::randomizeB);
    connect(actions["create_matrix"], &QAction::triggered, this, &GaussSeidelMethodTab::createMatrixB);
    connect(layout->findChild<QTableWidget*>("table_matrix_B"), &QTableWidget::cellChanged, this, &GaussSeidelMethodTab::setCellB);

    layout->findChild<QPushButton*>("button_matrix_B")->setMenu(matrixToolset);
}

void GaussSeidelMethodTab::createVectorXMenu()
{
    QFont font;
    font.setPointSize(11);

    QAction* action_save = new QAction(layout);
    action_save->setObjectName("action_save");
    action_save->setText("Save matrix");
    action_save->setFont(font);

    QAction* action_solve = new QAction(layout);
    action_solve->setObjectName("action_solve");
    action_solve->setText("Solve");
    action_solve->setFont(font);

    QMenu* matrixToolset = new QMenu(layout);
    matrixToolset->setObjectName("menu_vector_x");
    matrixToolset->addAction(action_save);
    matrixToolset->addSeparator();
    matrixToolset->addAction(action_solve);

    connect(action_save, &QAction::triggered, this, &GaussSeidelMethodTab::saveVectorX);
    connect(action_solve, &QAction::triggered, this, &GaussSeidelMethodTab::solve);

    layout->findChild<QPushButton*>("button_vector_x")->setMenu(matrixToolset);

    layout->findChild<QTableWidget*>("table_vector_x")->setEditTriggers(QTableWidget::NoEditTriggers);
}
