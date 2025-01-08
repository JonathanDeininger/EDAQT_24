#ifndef INSTALLERDIALOG_H
#define INSTALLERDIALOG_H

#include <QDialog>
#include "DataBase.h"
#include <QFileDialog>
#include <QTimer>
#include <QDir>
#include <QListWidgetItem>
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
    bool isDatabaseEmpty(); // Add this method declaration

private:
    Ui::InstallerDialog *ui;
    DataBase dataBase;
    void addAllSongsToPlaylist(); // Add this method declaration
    void loadSongsIntoDatabase(); // Add this method declaration
};

#endif // INSTALLERDIALOG_H
