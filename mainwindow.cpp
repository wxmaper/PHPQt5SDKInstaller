#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <quazipfile.h>

#include <QDebug>

#include "data.h"
#include <windows.h>
#include <quazip/quazip.h>
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    const QStringList defaultPaths = {
        QStringLiteral("C:/Qt/Qt5.8.0"),
        QStringLiteral("D:/Qt/Qt5.8.0"),
        QStringLiteral("E:/Qt/Qt5.8.0"),
        QStringLiteral("C:/Qt"),
        QStringLiteral("D:/Qt"),
        QStringLiteral("E:/Qt"),
        QStringLiteral("C:/")
    };

    QDir defaultDirPath;
    foreach (const QString &defaultPath, defaultPaths) {
        defaultDirPath.setPath(defaultPath);
        if (defaultDirPath.exists()) {
            ui->lineEdit_installPath->setText(defaultDirPath.toNativeSeparators(defaultPath));
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_next_clicked()
{
    int idx = ui->stackedWidget->currentIndex();

    switch (idx) {
    case 0:
        break;

    case 1:
        if (!testInstallPath()) return;
        break;

    case 5:
        close();
        break;
    }

    ui->stackedWidget->setCurrentIndex(idx+1);
}

void MainWindow::on_stackedWidget_currentChanged(int idx)
{
    ui->pushButton_back->setEnabled(idx != 0);
    ui->pushButton_next->setText(QLatin1String("Next"));
    ui->pushButton_next->setEnabled(true);

    switch (idx) {
    case 2:
        ui->pushButton_next->setEnabled(ui->radioButton_accept->isChecked());
        break;

    case 3:
        ui->pushButton_next->setText(QLatin1String("Install"));
        break;

    case 4:
        ui->pushButton_back->setEnabled(false);
        ui->pushButton_next->setEnabled(false);
        startInstall();
        break;

    case 5:
        ui->pushButton_next->setText(QLatin1String("Finish"));
        break;

    case 6:
        ui->pushButton_back->setEnabled(false);
        ui->pushButton_next->setEnabled(false);
        ui->pushButton_cancel->setEnabled(true);
        ui->pushButton_next->setText(QLatin1String("Finish"));
        break;

    default: //nothing
        break;
    }
}

bool MainWindow::testInstallPath()
{
    QDir installPath(ui->lineEdit_installPath->text());

    if (installPath.exists()) {
        if (!installPath.exists(QLatin1String("Tools/QtCreator/bin/qtcreator.exe"))) {
            QMessageBox::warning(this, QLatin1String("Error"),
                                 QLatin1String("Please choose the valid folder where the Qt Creator is located!"));
            return false;
        }

        if (!installPath.exists(QLatin1String("Tools/QtCreator/share/qtcreator"))) {
            QMessageBox::warning(this, QLatin1String("Error"),
                                 QLatin1String("Please choose the valid folder where the Qt Creator is located!"));
            return false;
        }

    }

    m_installPath = installPath.absolutePath();
    return true;
}

void MainWindow::on_pushButton_browse_clicked()
{
    QDir defaultLoc;

    if (!ui->lineEdit_installPath->text().isEmpty()) {
        defaultLoc.setPath(ui->lineEdit_installPath->text());
    }

    QString dir = QFileDialog::getExistingDirectory(this, QLatin1String("Choose the Qt Creator folder"),
                                                    defaultLoc.path(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->lineEdit_installPath->setText(QDir::toNativeSeparators(dir));
}

void MainWindow::on_pushButton_cancel_clicked()
{
    int i = QMessageBox::question(this, QLatin1String("Exit installation"),
                                  QLatin1String("Are you sure you want to exit installation?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (i == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::on_pushButton_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(qMax(0, ui->stackedWidget->currentIndex()-1));
}

void MainWindow::on_radioButton_accept_clicked()
{
    ui->pushButton_next->setEnabled(ui->radioButton_accept->isChecked());
}

void MainWindow::on_radioButton_decline_clicked()
{
    ui->pushButton_next->setEnabled(ui->radioButton_accept->isChecked());
}

void MainWindow::startInstall()
{
    HRSRC res = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(DATA_RESOURCE), RT_RCDATA);
    if (!res) {
        qDebug() << "can't find resource data";
        ui->stackedWidget->setCurrentIndex(6);
        return ;
    }

    HGLOBAL resHandle = LoadResource(NULL, res);
    if (!resHandle) {
        qDebug() << "can't load resource data";
        ui->stackedWidget->setCurrentIndex(6);
        return ;
    }

    DWORD resSize = SizeofResource(NULL, res);
    QByteArray resData = QByteArray::fromRawData((char*) LockResource(resHandle), resSize);

    QBuffer buffer(&resData);
    buffer.open(QIODevice::ReadWrite);

    QuaZip zip(&buffer);
    if (zip.open(QuaZip::mdUnzip)) {
        QuaZipFile file(&zip);
        QString appendString, filePath;
        QDir creatingDir;
        QFile creatingFile;
        QFileInfo creatingFileInfo;

        QString errorString;
        bool _break = false;

        ui->progressBar->setMaximum(zip.getEntriesCount());
        zip.goToFirstFile();
        for (int entry = 0; zip.goToNextFile(); entry++) {
            QApplication::processEvents();

            if (!file.open(QIODevice::ReadOnly)) {
                errorString = QStringLiteral("Cannot read resource data \"%1\"")
                        .arg(QDir::toNativeSeparators(zip.getCurrentFileName()));
                break;
            }

            filePath = file.getActualFileName();
            creatingFileInfo.setFile(file.getActualFileName());
            if (filePath.right(1) == QStringLiteral("/")) {
                creatingDir.setPath(filePath);
                creatingDir.setPath(QStringLiteral("%1/%2")
                                    .arg(m_installPath)
                                    .arg(filePath));
                if (!creatingDir.mkpath(".")) {
                    errorString = QStringLiteral("Cannot create path \"%1\"")
                            .arg(QDir::toNativeSeparators(creatingDir.absolutePath()));
                    _break = true;
                }
            }
            else {
                creatingDir.setPath(QStringLiteral("%1/%2")
                                    .arg(m_installPath)
                                    .arg(creatingFileInfo.path()));

                if (creatingDir.mkpath(".")) {
                    appendString = QStringLiteral("Extract <b>%1</b> to %2...")
                            .arg(filePath)
                            .arg(creatingDir.absolutePath());

                    ui->plainTextEdit->appendHtml(appendString);
                    QApplication::processEvents();

                    creatingFile.setFileName(QStringLiteral("%1/%2").arg(m_installPath).arg(filePath));
                    if (!creatingFile.open(QIODevice::WriteOnly)) {
                        errorString = QStringLiteral("Cannot write file \"%1\"")
                                .arg(QDir::toNativeSeparators(filePath));
                        _break = true;
                    }
                    else {
                        creatingFile.write(file.readAll());
                        creatingFile.close();
                    }
                }
                else {
                    errorString = QStringLiteral("Cannot create path \"%1\"")
                            .arg(QDir::toNativeSeparators(creatingDir.absolutePath()));
                    _break = true;
                }
            }

            file.close();
            ui->progressBar->setValue(entry);
            if (_break) break;
        }

        if (zip.getZipError() != UNZ_OK) {
            QMessageBox::critical(this, QLatin1String("Critical error"),
                                  QLatin1String("Something went wrong! :|"));
            ui->stackedWidget->setCurrentIndex(6);
        }
        else {
            if (errorString.isEmpty()) {
                ui->progressBar->setValue(ui->progressBar->maximum());
                ui->plainTextEdit->appendHtml("<br><b>Success!</b>");

                QFile qtversion(QStringLiteral("%1/%2")
                                .arg(m_installPath)
                                .arg(QStringLiteral("Tools/QtCreator/share/qtcreator/QtProject/qtcreator/qtversion.xml")));

                if (qtversion.exists() && qtversion.open(QFile::ReadWrite)) {
                    QByteArray qtversion_ba = qtversion.readAll();
                    qtversion_ba = qtversion_ba.replace(QStringLiteral("%{QtPath}"), m_installPath.toUtf8());
                    qtversion.close();
                    qtversion.open(QFile::WriteOnly|QFile::Truncate);
                    qtversion.write(qtversion_ba);
                    qtversion.close();
                }
                else {
                    QMessageBox::warning(this, QStringLiteral("Warning"),
                                         QStringLiteral("Can't update configurations: file not found\n%1")
                                         .arg(qtversion.fileName()));
                }
                
                QFile toolchains(QStringLiteral("%1/%2")
                                .arg(m_installPath)
                                .arg(QStringLiteral("Tools/QtCreator/share/qtcreator/QtProject/qtcreator/toolchains.xml")));

                if (toolchains.exists() && toolchains.open(QFile::ReadWrite)) {
                    QByteArray toolchains_ba = toolchains.readAll();
                    toolchains_ba = toolchains_ba.replace(QString("%{QtPath}"), m_installPath.toUtf8());
                    toolchains.close();
                    toolchains.open(QFile::WriteOnly|QFile::Truncate);
                    toolchains.write(toolchains_ba);
                    toolchains.close();
                }
                else {
                    QMessageBox::warning(this, QStringLiteral("Warning"),
                                         QStringLiteral("Can't update configurations: file not found\n%1")
                                         .arg(toolchains.fileName()));
                }
                
                ui->pushButton_next->setEnabled(true);
                ui->pushButton_cancel->setEnabled(false);
            }
            else {
                QMessageBox::critical(this, QLatin1String("Critical error"), errorString);
                ui->stackedWidget->setCurrentIndex(6);
            }
        }
    }
    else {
        QMessageBox::critical(this, QLatin1String("Critical error"),
                              QStringLiteral("Cannot read resource data. Exit with error %1")
                              .arg(zip.getZipError()));
        ui->stackedWidget->setCurrentIndex(6);
    }
}

bool MainWindow::createPath(const QString &path)
{
    QDir dir(path);
    if (!dir.mkpath(".")) {
        QMessageBox::critical(this, QStringLiteral("Critical error"),
                              QStringLiteral("Cannot create path \"%1\"")
                              .arg(dir.absolutePath()));
        return false;
    }

    return true;
}

void MainWindow::on_lineEdit_installPath_returnPressed()
{
    if (ui->pushButton_next->isEnabled())
        ui->pushButton_next->click();
}
