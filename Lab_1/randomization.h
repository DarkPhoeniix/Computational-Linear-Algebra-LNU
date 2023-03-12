#ifndef RANDOMIZATION_H
#define RANDOMIZATION_H

#include <QDialog>

class QDoubleSpinBox;
class QLabel;

class RandomizationDialog : public QDialog
{
    Q_OBJECT
public:
    RandomizationDialog(QWidget *parent = nullptr, bool* ok = nullptr);

    double getLeftBorderValue() const;
    double getRightBorderValue() const;

private slots:
    void accept() override;

    void setLeftBorderValue();
    void setRightBorderValue();

private:
    bool* ok;

    double leftBorderValue;
    double rightBorderValue;

    QDoubleSpinBox* leftBorderBox;
    QDoubleSpinBox* rightBorderBox;
};

#endif // RANDOMIZATION_H
