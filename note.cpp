#include "note.h"
#include "mainwindow.h"

Note::Note(QString idEtud, QString cours, QString noteObtenue, QString dateEvaluation)
{
    this->idEtud = idEtud;
    this->cours = cours;
    this->noteObtenue = noteObtenue;
    this->dateEvaluation=dateEvaluation;
}


QString Note::getNumeroEtudiant() const { return idEtud; }
QString Note::getCours() const { return cours; }
QString Note::getNote() const { return noteObtenue; }
QString Note::getDateEvaluation() const { return dateEvaluation; }


bool Note::insererNoteDansBaseDeDonnees(const Note &note)
{

    QSqlQuery checkNoteQuery;
    checkNoteQuery.prepare("SELECT COUNT(*) FROM Note WHERE idEtud = :idEtud AND idCours = (SELECT codeCours FROM Cours WHERE titre = :cours)");
    checkNoteQuery.bindValue(":idEtud", note.getNumeroEtudiant());
    checkNoteQuery.bindValue(":cours", note.getCours());

    if (!checkNoteQuery.exec() || !checkNoteQuery.next()) {
        qDebug() << "Erreur de vérification de l'étudiant:" << checkNoteQuery.lastError().text();
        return false;
    }



    if (checkNoteQuery.value(0).toInt() > 0) {
        QString errorMessage = "L'étudiant avec l'ID \"" + note.getNumeroEtudiant() + "\" a déjà une note dans ce cours.";
        QMessageBox::critical(nullptr, "Erreur", errorMessage);
        qDebug() << errorMessage;
        return false;
    }


    QSqlQuery query;
    query.prepare("INSERT INTO Note (idEtud, idCours, noteObtenue, dateEvaluation) VALUES (:idEtud, :idCours, :noteObtenue, :dateEvaluation)");
    query.bindValue(":idEtud", note.getNumeroEtudiant());

    QSqlQuery coursQuery;
    coursQuery.prepare("SELECT codeCours FROM Cours WHERE titre = :cours ");
    coursQuery.bindValue(":cours", note.getCours());

    if (coursQuery.exec() && coursQuery.next()) {
        query.bindValue(":idCours", coursQuery.value(0).toInt());
    }

    double NOTE=note.getNote().toDouble();

    if(NOTE<0 || NOTE>20){
        QMessageBox::critical(nullptr, "Erreur", "Note non valide !");
        return false;
    }

    query.bindValue(":noteObtenue", note.getNote());
    query.bindValue(":dateEvaluation", note.getDateEvaluation());

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur d'insertion:" << query.lastError().text();
        return false;
    }
}



