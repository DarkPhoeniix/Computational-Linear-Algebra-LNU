#ifndef MATRIXOPERATIONSTAB_H
#define MATRIXOPERATIONSTAB_H

#include "matrix.h"

#include <QWidget>

#include <memory>

class MatrixOperationsTab : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixOperationsTab(QWidget *parent = nullptr);
    ~MatrixOperationsTab();

private slots:
    void loadMatrixA();
    void saveMatrixA();
    void addRowToMatrixA();
    void addColumnToMatrixA();
    void removeRowFromMatrixA();
    void removeColumnFromMatrixA();
    void randomizeA();
    void createMatrixA();
    void setCellA(int rowIndex, int columnIndex);

    void loadMatrixB();
    void saveMatrixB();
    void addRowToMatrixB();
    void addColumnToMatrixB();
    void removeRowFromMatrixB();
    void removeColumnFromMatrixB();
    void randomizeB();
    void createMatrixB();
    void setCellB(int rowIndex, int columnIndex);

    void saveMatrixC();
    void addMatrices();
    void subtractMatrices();
    void multiplyMatrices();

private:
    void updateMatrixA();
    void updateMatrixB();
    void updateMatrixC();

    void createMatrixAMenu();
    void createMatrixBMenu();
    void createMatrixCMenu();

private:
    QWidget* layout;

    std::unique_ptr<Matrix> A;
    std::unique_ptr<Matrix> B;
    std::unique_ptr<Matrix> C;
};

#endif // MATRIXOPERATIONSTAB_H
