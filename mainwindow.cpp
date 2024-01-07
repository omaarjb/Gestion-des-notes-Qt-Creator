#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cours.h"
#include "note.h"
#include "enseignant.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/omarj/OneDrive/Desktop/gestion_des_notes.sqlite");

    if(db.open()){
        QMessageBox::information(this, "Connection", "Connected");
    } else {
        QMessageBox::critical(this, "Connection Error", "Not connected");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::fetchEtudiantNom(int etudiantId)
{
    QSqlQuery query;
    query.prepare("SELECT nom FROM Etudiant WHERE numEtud = :etudiantId");
    query.bindValue(":etudiantId", etudiantId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Query Error: " << query.lastError().text();
        return QString();  // Return an empty string in case of an error
    }
}

QString MainWindow::fetchNote(int idNote)
{
    QSqlQuery query;
    query.prepare("SELECT noteObtenue FROM Note WHERE id = :idNote");
    query.bindValue(":idNote", idNote);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Query Error: " << query.lastError().text();
        return QString();
    }
}



void MainWindow::on_afficherNoteButton_clicked()
{
    QString numeroEtudiantstr = ui->lineEdit->text().trimmed();
    QString coursTitre = ui->lineEdit_2->text().trimmed();
    int numeroEtudiant = numeroEtudiantstr.toInt();

    if (numeroEtudiantstr.isEmpty() || coursTitre.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    else if(!e.checkEtudiantExiste(numeroEtudiantstr)){
        QMessageBox::warning(this, "Erreur", "Etudiant avec ce ID n'existe pas.");
                return;
    }
    else{
        QSqlQuery query;
        query.prepare("SELECT Note.id, Etudiant.nom, Note.noteObtenue, Note.dateEvaluation, Cours.titre "
                      "FROM Note "
                      "JOIN Cours ON Note.idCours = Cours.codeCours "
                      "JOIN Etudiant ON Note.idEtud = Etudiant.numEtud "
                      "WHERE Note.idEtud = :numeroEtudiant AND Cours.titre = :coursTitre");
        query.bindValue(":numeroEtudiant", numeroEtudiant);
        query.bindValue(":coursTitre", coursTitre);

        if (query.exec())
        {
            QStandardItemModel *model = new QStandardItemModel(this);

                    QStringList horizontalHeader;
                    horizontalHeader.append("ID Note");
                     horizontalHeader.append("Nom");
                    horizontalHeader.append("Note");
                    horizontalHeader.append("Date");
                    horizontalHeader.append("Cours");

                    model->setHorizontalHeaderLabels(horizontalHeader);

                    while (query.next())
                           {
                               int id = query.value(0).toInt();
                               QString nomEtudiant = query.value(1).toString();
                               double noteObtenue = query.value(2).toDouble();
                               QString dateEvaluation = query.value(3).toString();
                               QString courseTitle = query.value(4).toString();

                               QList<QStandardItem *> items;
                               items.append(new QStandardItem(QString::number(id)));
                               items.append(new QStandardItem(nomEtudiant));
                               items.append(new QStandardItem(QString::number(noteObtenue)));
                               items.append(new QStandardItem(dateEvaluation));
                               items.append(new QStandardItem(courseTitle));

                               model->appendRow(items);
                           }

                    ui->tableView->setModel(model);


        }
        else
        {
            qDebug() << "Query Error: " << query.lastError().text();
        }
    }


}

void MainWindow::on_afficherNoteButton_3_clicked()
{
    QString numeroEtudiantstr = ui->lineEdit_3->text().trimmed();
    int numeroEtudiant = numeroEtudiantstr.toInt();
    if (numeroEtudiantstr.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Entrer votre numero d'etudiant");
        return;
    }

    else if(!e.checkEtudiantExiste(numeroEtudiantstr)){
        QMessageBox::warning(this, "Erreur", "Etudiant avec ce ID n'existe pas.");
                return;
    }

    else{
        QString etudiantNom = fetchEtudiantNom(numeroEtudiant);
        ui->label_4->setText(etudiantNom);

        QSqlQuery query;
        query.prepare("SELECT Cours.titre, Note.noteObtenue, Note.dateEvaluation  "
                      "FROM Note "
                      "JOIN Cours ON Note.idCours = Cours.codeCours "
                      "WHERE Note.idEtud = :numeroEtudiant");
        query.bindValue(":numeroEtudiant", numeroEtudiant);

        if (query.exec())
        {
            QStandardItemModel *model = new QStandardItemModel(this);

                    QStringList horizontalHeader;

                    horizontalHeader.append("Cours");
                    horizontalHeader.append("Note");
                    horizontalHeader.append("Date");
                    model->setHorizontalHeaderLabels(horizontalHeader);

                    while (query.next())
                           {
                               QString courseTitle = query.value(0).toString();
                               double noteObtenue = query.value(1).toDouble();
                               QString dateEvaluation = query.value(2).toString();


                               QList<QStandardItem *> items;
                               items.append(new QStandardItem(courseTitle));
                               items.append(new QStandardItem(QString::number(noteObtenue)));
                               items.append(new QStandardItem(dateEvaluation));


                               model->appendRow(items);
                           }

                    ui->tableView_2->setModel(model);

        }
        else
        {
            qDebug() << "Query Error: " << query.lastError().text();
        }

    }


}

void MainWindow::on_pushButton_clicked()
{
    QString titre = ui->lineEdit_4->text();
    QString responsable = ui->lineEdit_5->text();

    if (titre.isEmpty() || responsable.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
           return;
       }

    Cours nouveauCours(titre,responsable);
    if(Cours::insererCoursDansBaseDeDonnees(nouveauCours)){
        QMessageBox::information(this, "Inserer", "Cours Inseré !");
    }else{
        QMessageBox::warning(this, "Erreur", "Echec d'insertion !");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQueryModel* model = Cours::getListeCours();
    ui->tableView_3->setModel(model);
    ui->tableView_3->setColumnWidth(0, 200);
    ui->tableView_3->setColumnWidth(1, 300);


}

void MainWindow::on_pushButton_3_clicked()
{
    QString nom=ui->lineEdit_6->text();
    QString prenom=ui->lineEdit_7->text();
    QString specialite=ui->lineEdit_8->text();

    if (nom.isEmpty() || prenom.isEmpty() || specialite.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
           return;
       }

    Etudiant nouveauEtudiant(nom,prenom,specialite);
    if(Etudiant::insererEtudiantDansBaseDeDonnees(nouveauEtudiant)){
        QMessageBox::information(this, "Inserer", "Etudiant Inseré !");
    }
    else{
            QMessageBox::warning(this, "Erreur", "Echec d'insertion !");
        }
}

void MainWindow::on_pushButton_7_clicked()
{
    QString idEtud=ui->lineEdit_9->text();
    QString cours=ui->lineEdit_10->text();
    QString note=ui->lineEdit_11->text();
    QString dateEvaluation=ui->lineEdit_12->text();

    if (idEtud.isEmpty() || cours.isEmpty() || note.isEmpty() || dateEvaluation.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
           return;
       }
    else if(!e.checkEtudiantExiste(idEtud)){
        QMessageBox::warning(this, "Erreur", "Etudiant avec ce ID n'existe pas.");
                return;
    }

    else if(!c.checkCoursExiste(cours)){
        QMessageBox::warning(this, "Erreur", "Ce cours n'existe pas.");
                return;
    }

    Note nouvelleNote(idEtud,cours,note,dateEvaluation);
    if(Note::insererNoteDansBaseDeDonnees(nouvelleNote)){
        QMessageBox::information(this, "Inserer", "Note Inserée !");
    }
    else{
            QMessageBox::warning(this, "Erreur", "Echec d'insertion !");
        }
}

void MainWindow::on_pushButton_9_clicked()
{
    QString cours = ui->lineEdit_28->text();
    if (cours.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Entrer un cours");
        return;
    }

    else if(!c.checkCoursExiste(cours)){
        QMessageBox::warning(this, "Erreur", "Ce cours n'existe pas.");
                return;
    }

    else{

        QSqlQuery query;
        query.prepare("SELECT Note.idEtud AS 'N° Etudiant', Etudiant.nom AS 'Nom', Etudiant.prenom AS 'Prenom', Cours.titre AS 'Cours', Note.noteObtenue AS 'Note', Note.dateEvaluation AS 'Date evaluation' "
                      "FROM Note "
                      "JOIN Cours ON Note.idCours = Cours.codeCours "
                      "JOIN Etudiant ON Note.idEtud = Etudiant.numEtud "
                      "WHERE Cours.titre= :cours ");
        query.bindValue(":cours", cours);

        if (query.exec())
        {
            QStandardItemModel *model = new QStandardItemModel(this);

                    QStringList horizontalHeader;

                    horizontalHeader.append("N° Etudiant");
                    horizontalHeader.append("Nom");
                    horizontalHeader.append("Prenom");
                    horizontalHeader.append("Cours");
                    horizontalHeader.append("Note");
                    horizontalHeader.append("Date");
                    model->setHorizontalHeaderLabels(horizontalHeader);

                    while (query.next())
                           {
                               int numEtud=query.value(0).toInt();
                               QString nom = query.value(1).toString();
                               QString prenom = query.value(2).toString();
                               QString cours = query.value(3).toString();
                               double note = query.value(4).toDouble();
                               QString dateEvaluation = query.value(5).toString();


                               QList<QStandardItem *> items;
                               items.append(new QStandardItem(QString::number(numEtud)));
                               items.append(new QStandardItem(nom));
                               items.append(new QStandardItem(prenom));
                               items.append(new QStandardItem(cours));
                               items.append(new QStandardItem(QString::number(note)));
                               items.append(new QStandardItem(dateEvaluation));


                               model->appendRow(items);
                           }

                    ui->tableView_7->setModel(model);

        }
        else
        {
            qDebug() << "Query Error: " << query.lastError().text();
        }

    }


}

void MainWindow::on_pushButton_8_clicked()
{
    QString idEtud=ui->lineEdit_25->text();
    QString cours=ui->lineEdit_26->text();
    QString nvNote=ui->lineEdit_27->text();
    int idNote;
    if (idEtud.isEmpty() || cours.isEmpty() || nvNote.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
           return;
       }
    else if(!e.checkEtudiantExiste(idEtud)){
        QMessageBox::warning(this, "Erreur", "Etudiant avec ce ID n'existe pas.");
                return;
    }

    else if(!c.checkCoursExiste(cours)){
        QMessageBox::warning(this, "Erreur", "Ce cours n'existe pas.");
                return;
    }
    else{
        QSqlQuery query;
        QSqlQuery updateQuery;
        double NVnote=nvNote.toDouble();
        query.prepare("SELECT id "
                      "FROM Note "
                      "WHERE idEtud = :idEtud "
                      "AND idCours = (SELECT codeCours FROM Cours WHERE titre = :cours)");
        query.bindValue(":idEtud", idEtud);
        query.bindValue(":cours", cours);
        if (query.exec() && query.next()) {
            idNote=query.value(0).toInt();
            qDebug()<<idNote;
            double noteActuelle=fetchNote(idNote).toDouble();
            qDebug()<<noteActuelle;
            qDebug()<<NVnote;
            ui->label_43->setNum(noteActuelle);
            updateQuery.prepare("UPDATE Note "
                          "SET noteObtenue = :nvNote "
                          "WHERE id= :idNote ");
            updateQuery.bindValue(":nvNote", NVnote);
            updateQuery.bindValue(":idNote", idNote);
            if(updateQuery.exec()){
                QMessageBox::information(this, "Modifier", "Note Modifiée !");
            }else{
                qDebug() << "Query Error: " << updateQuery.lastError().text();
            }

        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QSqlQueryModel* model = Etudiant::getListeEtudiant();
    ui->tableView_4->setModel(model);

}

void MainWindow::on_pushButton_5_clicked()
{
    QSqlQueryModel* model = Enseignant::getListeEnseignant();
    ui->tableView_5->setModel(model);
    ui->tableView_5->setColumnWidth(1, 200);
    ui->tableView_5->setColumnWidth(2, 200);

}

void MainWindow::on_pushButton_6_clicked()
{
    QString nom=ui->lineEdit_13->text();
    QString prenom=ui->lineEdit_14->text();


    if (nom.isEmpty() || prenom.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
           return;
       }

    Enseignant nouveauEnseignant(nom,prenom);
    if(Enseignant::insererEnseignantDansBaseDeDonnees(nouveauEnseignant)){
        QMessageBox::information(this, "Inserer", "Enseignant Inseré !");
    }
    else{
            QMessageBox::warning(this, "Erreur", "Echec d'insertion !");
        }
}




void MainWindow::on_pushButton_10_clicked()
{
    QString idEns=ui->lineEdit_15->text();


    if (idEns.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Entrer l'identifiant de l'enseignant.");
           return;
       }
    else if(!en.checkEnseignantExiste(idEns)){
        QMessageBox::warning(this, "Erreur", "L'enseignant avec ce ID n'existe pas.");
                return;
    }

    QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cet enseignant?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            if (en.supprimerEnseignant(idEns)) {
                QMessageBox::information(this, "Succès", "Enseignant supprimé avec succès.");

            } else {
                QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'enseignant.");
            }
        }

}
