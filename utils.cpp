#include "utils.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QSpacerItem>

using namespace std;

void infoBox(string msg){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Compiler");
    msgBox.setText(msg.c_str());

    QSpacerItem* horizontalSpacer = new QSpacerItem(270, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    msgBox.exec();
}

void errorBox(string msg){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Compiler");
    msgBox.setText(msg.c_str());

    QSpacerItem* horizontalSpacer = new QSpacerItem(270, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    msgBox.exec();
}
