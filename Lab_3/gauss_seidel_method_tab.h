#ifndef GAUSS_SEIDEL_METHOD_TAB_H
#define GAUSS_SEIDEL_METHOD_TAB_H

#include "sle.h"

#include <QWidget>

#include <memory>

class GaussSeidelMethodTab : public QWidget
{
    Q_OBJECT

public:
    explicit GaussSeidelMethodTab(QWidget *parent = nullptr);
    ~GaussSeidelMethodTab();

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

    void saveVectorX();
    void solve();

private:
    void updateMatrixA();
    void updateMatrixB();
    void updateVectorX();

    void createMatrixAMenu();
    void createMatrixBMenu();
    void createVectorXMenu();

private:
    QWidget* layout;

    std::unique_ptr<SLE> sle;
};

#endif // GAUSS_SEIDEL_METHOD_TAB_H
