#ifndef INSTALLERDIALOG_H
#define INSTALLERDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QTimer>
#include "DataBase.h"
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
    bool isDatabaseEmpty(); // Add this method declaration

private:
    Ui::InstallerDialog *ui;
    DataBase dataBase;
    void addAllSongsToPlaylist(); // Add this method declaration
    int totalSongsAdded;
};

#endif // INSTALLERDIALOG_H
