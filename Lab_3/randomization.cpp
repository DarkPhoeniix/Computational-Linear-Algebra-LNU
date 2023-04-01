#include "randomization.h"

#include <QLabel>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

#include <numeric>

RandomizationDialog::RandomizationDialog(QWidget *parent, bool* ok)
    : QDialog(parent)
    , ok(ok)
    , leftBorderValue(-1.0)
    , rightBorderValue(1.0)
{
    this->setFont(QFont("Arial", 11));
    QFormLayout* form = new QFormLayout(this);

    QLabel *leftBorderLabel = new QLabel(QString("Left border:"), this);
    leftBorderBox = new QDoubleSpinBox(this);
    leftBorderBox->setMinimum(std::numeric_limits<double>::lowest());
    leftBorderBox->setMaximum(std::numeric_limits<double>::max());
    leftBorderBox->setValue(leftBorderValue);
    QLabel *rightBorderLabel = new QLabel(QString("Right border:"), this);
    rightBorderBox = new QDoubleSpinBox(this);
    rightBorderBox->setMinimum(std::numeric_limits<double>::lowest());
    rightBorderBox->setMaximum(std::numeric_limits<double>::max());
    rightBorderBox->setValue(rightBorderValue);

    form->insertRow(0, leftBorderLabel);
    form->insertRow(1, leftBorderBox);
    form->insertRow(2, rightBorderLabel);
    form->insertRow(3, rightBorderBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    form->insertRow(4, buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &RandomizationDialog::accept);
    connect(leftBorderBox, &QDoubleSpinBox::valueChanged, this, &RandomizationDialog::setLeftBorderValue);
    connect(rightBorderBox, &QDoubleSpinBox::valueChanged, this, &RandomizationDialog::setRightBorderValue);
}

double RandomizationDialog::getLeftBorderValue() const
{
    return leftBorderValue;
}

double RandomizationDialog::getRightBorderValue() const
{
    return rightBorderValue;
}

void RandomizationDialog::accept()
{
    if (ok)
        *ok = true;

    this->done(0);
}

void RandomizationDialog::setLeftBorderValue()
{
    leftBorderValue = leftBorderBox->value();
}

void RandomizationDialog::setRightBorderValue()
{
    rightBorderValue = rightBorderBox->value();
}
