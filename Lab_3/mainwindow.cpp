#include "mainwindow.h"

#include "matrix_operations_tab.h"
#include "gaussian_elimination_tab.h"
#include "gauss_seidel_method_tab.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("SLE Solver");

    tabs = new QTabWidget(this);
    tabs->setFixedSize(1000, 400);

    this->setCentralWidget(tabs);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->adjustSize();

    QWidget* matrixOperationTab = new MatrixOperationsTab(tabs);
    tabs->addTab(matrixOperationTab, "Matrix Operations");

    QWidget* gaussianEliminationTab = new GaussianEliminationTab(tabs);
    tabs->addTab(gaussianEliminationTab, "Gaussian Elimination");

    QWidget* gaussSeidelMethodTab = new GaussSeidelMethodTab(tabs);
    tabs->addTab(gaussSeidelMethodTab, "Gauss-Seidel Method");
}

MainWindow::~MainWindow()
{
    delete tabs;
}
