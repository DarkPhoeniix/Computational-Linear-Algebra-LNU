#include "set_matrix_size.h"

#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>

#include <numeric>

SetMatrixSize::SetMatrixSize(QWidget *parent, bool* ok)
    : QDialog(parent)
    , ok(ok)
    , rowsNumber(1)
    , columnsNumber(1)
{
    this->setFont(QFont("Arial", 11));
    QFormLayout* form = new QFormLayout(this);

    QLabel *rowsNumberLabel = new QLabel(QString("Input number of rows:"), this);
    rowsNumberBox = new QSpinBox(this);
    rowsNumberBox->setMinimum(1);
    rowsNumberBox->setMaximum(std::numeric_limits<int>::max());
    rowsNumberBox->setValue(rowsNumber);
    QLabel *columnsNumberLabel = new QLabel(QString("Input number of columns:"), this);
    columnsNumberBox = new QSpinBox(this);
    columnsNumberBox->setMinimum(1);
    columnsNumberBox->setMaximum(std::numeric_limits<int>::max());
    columnsNumberBox->setValue(columnsNumber);

    form->insertRow(0, rowsNumberLabel);
    form->insertRow(1, rowsNumberBox);
    form->insertRow(2, columnsNumberLabel);
    form->insertRow(3, columnsNumberBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    form->insertRow(4, buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SetMatrixSize::accept);
    connect(rowsNumberBox, &QSpinBox::valueChanged, this, &SetMatrixSize::setRowsNumber);
    connect(columnsNumberBox, &QSpinBox::valueChanged, this, &SetMatrixSize::setColumnsNumber);
}

int SetMatrixSize::getRowsNumber() const
{
    return rowsNumber;
}

int SetMatrixSize::getColumnsNumber() const
{
    return columnsNumber;
}

void SetMatrixSize::accept()
{
    if (ok)
        *ok = true;

    this->done(0);
}

void SetMatrixSize::setRowsNumber()
{
    rowsNumber = rowsNumberBox->value();
}

void SetMatrixSize::setColumnsNumber()
{
    columnsNumber = columnsNumberBox->value();
}
