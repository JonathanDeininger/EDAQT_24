#ifndef INSTALLERDIALOG_H
#define INSTALLERDIALOG_H

#include <QDialog>
#include "DataBase.h"
#include <QFileDialog>
#include <QTimer>
#include <QDir>
#include <QListWidgetItem>
#include <iostream>
#include <QMessageBox>
#include <QProcessEnvironment>
// #include "mediacontroller.h"
namespace Ui {
class InstallerDialog;
}

class InstallerDialog : public QDialog
{
    Q_OBJECT

public:

    explicit InstallerDialog(QWidget *parent = nullptr);
    ~InstallerDialog();
    void onChooseInstallFolderClicked();
    void onChooseMusicFolderClicked();
    void onNextButtonClicked();

private:
    Ui::InstallerDialog *ui;
    DataBase dataBase;
};

#endif // INSTALLERDIALOG_H
