#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sle.h"

#include <QMainWindow>

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

private:
    void showError(const std::string& message);

    void updateMatrixA_MatrixOperations();
    void updateMatrixB_MatrixOperations();
    void updateMatrixC_MatrixOperations();

    void createMenubarForMatrixOperations();
    void createMenubarForSLESolver();

    void createMatrixAMenu_MatrixOperations();
    void createMatrixBMenu_MatrixOperations();
    void createMatrixCMenu_MatrixOperations();

private:
    Ui::MainWindow *ui;

    Matrix A;
    Matrix B;
    Matrix C;

    SLE sle;
};
#endif // MAINWINDOW_H
