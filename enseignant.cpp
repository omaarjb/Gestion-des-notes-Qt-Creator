#include "enseignant.h"
#include "mainwindow.h"
Enseignant::Enseignant(QString nom, QString prenom)
{
    this->nom = nom;
    this->prenom = prenom;

}

Enseignant :: Enseignant(){
    this->nom="";
    this->prenom = "";
}

QString Enseignant::getNom() const { return nom; }
QString Enseignant::getPrenom() const { return prenom; }
QString Enseignant::getIdentifiant() const { return identifiant; }

QSqlQueryModel* Enseignant::getListeEnseignant() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT Enseignant.id AS 'ID', Enseignant.nom AS 'Nom', Enseignant.prenom AS 'Prenom' "
                     "FROM Enseignant ");
    return model;
}

bool Enseignant::checkEnseignantExiste(const QString &identifiant) {
    QSqlQuery checkEnseignantQuery;
    int numInt=identifiant.toInt();
    checkEnseignantQuery.prepare("SELECT COUNT(*) FROM Enseignant WHERE id = :numInt");
    checkEnseignantQuery.bindValue(":numInt", numInt);

    if (checkEnseignantQuery.exec() && checkEnseignantQuery.next()) {
        int studentCount = checkEnseignantQuery.value(0).toInt();
        return (studentCount > 0);
    } else {
        qDebug() << "Student Check Query Error: " << checkEnseignantQuery.lastError().text();
        return false;
    }
}

bool Enseignant::insererEnseignantDansBaseDeDonnees(const Enseignant &enseignant)
{


    QSqlQuery checkEnseignantQuery;
        checkEnseignantQuery.prepare("SELECT COUNT(*) FROM Enseignant WHERE nom = :nom AND prenom = :prenom");
        checkEnseignantQuery.bindValue(":nom", enseignant.getNom());
        checkEnseignantQuery.bindValue(":prenom", enseignant.getPrenom());

        if (checkEnseignantQuery.exec() && checkEnseignantQuery.next()) {
            int enseignantCount = checkEnseignantQuery.value(0).toInt();
            if (enseignantCount > 0) {
                QString errorMessage = "l'Enseignant \"" + enseignant.getNom()+ " " + enseignant.getPrenom()+ "\" existe déjà .";
                QMessageBox::critical(nullptr, "Erreur", errorMessage);
                qDebug() << "L'Etudiant \"" << enseignant.getNom() << "\" existe .";
                return false;
            }
        } else {
            qDebug() << "Erreur de vérification du l'enseignant:" << checkEnseignantQuery.lastError().text();
            return false;
        }

    QSqlQuery query;

    query.prepare("INSERT INTO Enseignant (nom, prenom) VALUES (:nom, :prenom)");
    query.bindValue(":nom", enseignant.getNom());
    query.bindValue(":prenom", enseignant.getPrenom());
    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur d'insertion:" << query.lastError().text();
        return false;
    }
}

bool Enseignant::supprimerEnseignant(const QString &identifiant)
{
    QSqlQuery deleteQuery;
    int numInt = identifiant.toInt();
    deleteQuery.prepare("DELETE FROM Enseignant WHERE id = :numInt");
    deleteQuery.bindValue(":numInt", numInt);

    if (deleteQuery.exec()) {
        return true;
    } else {
        qDebug() << "Erreur de suppression de l'enseignant:" << deleteQuery.lastError().text();
        return false;
    }
}
