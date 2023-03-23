#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sle.h"

#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadMatrixA_MatrixOperations();
    void saveMatrixA_MatrixOperations();
    void addRowToMatrixA_MatrixOperations();
    void addColumnToMatrixA_MatrixOperations();
    void removeRowFromMatrixA_MatrixOperations();
    void removeColumnFromMatrixA_MatrixOperations();
    void randomizeA_MatrixOperations();
    void createMatrixA_MatrixOperations();
    void setCellA_MatrixOperations(int rowIndex, int columnIndex);

    void loadMatrixB_MatrixOperations();
    void saveMatrixB_MatrixOperations();
    void addRowToMatrixB_MatrixOperations();
    void addColumnToMatrixB_MatrixOperations();
    void removeRowFromMatrixB_MatrixOperations();
    void removeColumnFromMatrixB_MatrixOperations();
    void randomizeB_MatrixOperations();
    void createMatrixB_MatrixOperations();
    void setCellB_MatrixOperations(int rowIndex, int columnIndex);

    void saveMatrixC_MatrixOperations();
    void addMatrices_MatrixOperations();
    void subtractMatrices_MatrixOperations();
    void multiplyMatrices_MatrixOperations();

    void loadMatrixA_SLESolver();
    void saveMatrixA_SLESolver();
    void addRowToMatrixA_SLESolver();
    void addColumnToMatrixA_SLESolver();
    void removeRowFromMatrixA_SLESolver();
    void removeColumnFromMatrixA_SLESolver();
    void randomizeA_SLESolver();
    void createMatrixA_SLESolver();
    void setCellA_SLESolver(int rowIndex, int columnIndex);

    void loadMatrixB_SLESolver();
    void saveMatrixB_SLESolver();
    void addRowToMatrixB_SLESolver();
    void addColumnToMatrixB_SLESolver();
    void removeRowFromMatrixB_SLESolver();
    void removeColumnFromMatrixB_SLESolver();
    void randomizeB_SLESolver();
    void createMatrixB_SLESolver();
    void setCellB_SLESolver(int rowIndex, int columnIndex);

    void saveVectorX_SLESolver();
    void solveVectorX_SLESolver();

private:
    void updateMatrixA_MatrixOperations();
    void updateMatrixB_MatrixOperations();
    void updateMatrixC_MatrixOperations();

    void createMatrixAMenu_MatrixOperations();
    void createMatrixBMenu_MatrixOperations();
    void createMatrixCMenu_MatrixOperations();

    void updateMatrixA_SLESolver();
    void updateMatrixB_SLESolver();
    void updateVectorX_SLESolver();

    void createMatrixAMenu_SLESolver();
    void createMatrixBMenu_SLESolver();
    void createVectorXMenu_SLESolver();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Matrix> A;
    std::unique_ptr<Matrix> B;
    std::unique_ptr<Matrix> C;

    SLE sle;
};
#endif // MAINWINDOW_H
