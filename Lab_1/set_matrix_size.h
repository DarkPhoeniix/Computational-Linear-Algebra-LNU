

#ifndef SETMATRIXSIZE_H
#define SETMATRIXSIZE_H

#include <QDialog>

class QSpinBox;
class QLabel;

class SetMatrixSize : public QDialog
{
    Q_OBJECT
public:
    SetMatrixSize(QWidget *parent = nullptr, bool* ok = nullptr);

    int getRowsNumber() const;
    int getColumnsNumber() const;

private slots:
    void accept() override;

    void setRowsNumber();
    void setColumnsNumber();

private:
    bool* ok;

    int rowsNumber;
    int columnsNumber;

    QSpinBox* rowsNumberBox;
    QSpinBox* columnsNumberBox;
};

#endif // SETMATRIXSIZE_H
