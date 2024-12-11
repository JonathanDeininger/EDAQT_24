/********************************************************************************
** Form generated from reading UI file 'installerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTALLERDIALOG_H
#define UI_INSTALLERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_InstallerDialog
{
public:
    QPushButton *CancelButton;
    QPushButton *NextButton;
    QLabel *label;
    QTextEdit *MusicPath;
    QPushButton *ChoosePath;
    QLabel *label_3;

    void setupUi(QDialog *InstallerDialog)
    {
        if (InstallerDialog->objectName().isEmpty())
            InstallerDialog->setObjectName("InstallerDialog");
        InstallerDialog->resize(533, 364);
        CancelButton = new QPushButton(InstallerDialog);
        CancelButton->setObjectName("CancelButton");
        CancelButton->setGeometry(QRect(100, 310, 80, 24));
        NextButton = new QPushButton(InstallerDialog);
        NextButton->setObjectName("NextButton");
        NextButton->setGeometry(QRect(340, 310, 80, 24));
        label = new QLabel(InstallerDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 30, 401, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        label->setFont(font);
        label->setFrameShape(QFrame::Shape::Box);
        label->setLineWidth(1);
        label->setTextFormat(Qt::TextFormat::AutoText);
        MusicPath = new QTextEdit(InstallerDialog);
        MusicPath->setObjectName("MusicPath");
        MusicPath->setGeometry(QRect(40, 150, 221, 31));
        ChoosePath = new QPushButton(InstallerDialog);
        ChoosePath->setObjectName("ChoosePath");
        ChoosePath->setGeometry(QRect(340, 150, 80, 31));
        label_3 = new QLabel(InstallerDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 110, 391, 16));

        retranslateUi(InstallerDialog);

        QMetaObject::connectSlotsByName(InstallerDialog);
    } // setupUi

    void retranslateUi(QDialog *InstallerDialog)
    {
        InstallerDialog->setWindowTitle(QCoreApplication::translate("InstallerDialog", "Dialog", nullptr));
        CancelButton->setText(QCoreApplication::translate("InstallerDialog", "Abbrechen", nullptr));
        NextButton->setText(QCoreApplication::translate("InstallerDialog", "Abschlie\303\237en", nullptr));
        label->setText(QCoreApplication::translate("InstallerDialog", "Willkommen im Installer von dem Musikspieler 05", nullptr));
        ChoosePath->setText(QCoreApplication::translate("InstallerDialog", "W\303\244hle Pfad", nullptr));
        label_3->setText(QCoreApplication::translate("InstallerDialog", "W\303\244hle den Ordner aus, wo deine Musik sich befindet: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InstallerDialog: public Ui_InstallerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTALLERDIALOG_H
