#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_next_clicked();

    void on_stackedWidget_currentChanged(int idx);

    bool testInstallPath();

    void on_pushButton_browse_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_back_clicked();

    void on_radioButton_accept_clicked();

    void on_radioButton_decline_clicked();

    void startInstall();
    bool createPath(const QString &path);

    void on_lineEdit_installPath_returnPressed();

private:
    Ui::MainWindow *ui;
    QString m_installPath;
};

#endif // MAINWINDOW_H
