#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sle.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void removeRowFromMatrixB();
    void randomizeB();
    void createMatrixB();
    void setCellB(int rowIndex, int columnIndex);

    void solve();

private:
    void showError(const std::string& message);

    void updateMatrixA();
    void updateMatrixB();
    void updateVectorX();

private:
    Ui::MainWindow *ui;
    SLE sle;
};
#endif // MAINWINDOW_H
