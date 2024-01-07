#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>
#include "etudiant.h"
#include "cours.h"
#include "enseignant.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_afficherNoteButton_clicked();

    void on_afficherNoteButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString fetchEtudiantNom(int);
    Etudiant e;
    Enseignant en;
    Cours c;
    QString fetchEnseignantNom(int);
    QString fetchNote(int);
};

#endif // MAINWINDOW_H
