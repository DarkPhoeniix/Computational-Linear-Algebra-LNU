
#include <QMessageBox>

namespace helpers
{
void showError(const std::string& message)
{
    QMessageBox messageBox;
    messageBox.critical(nullptr, "Error", QString::fromUtf8(message.c_str()));
    messageBox.show();
}
}
