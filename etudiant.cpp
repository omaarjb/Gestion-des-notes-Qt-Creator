#include "etudiant.h"
#include "mainwindow.h"

Etudiant::Etudiant(QString nom, QString prenom, QString specialite)
{
    this->nom = nom;
    this->prenom = prenom;
    this->specialite = specialite;
}

Etudiant :: Etudiant(){
    this->nom="";
    this->prenom = "";
    this->specialite = "";
}

QString Etudiant::getNom() const { return nom; }
QString Etudiant::getPrenom() const { return prenom; }
QString Etudiant::getNumeroEtudiant() const { return numeroEtudiant; }
QString Etudiant::getSpecialite() const { return specialite; }

bool Etudiant::checkEtudiantExiste(const QString& numeroEtudiant) {
    QSqlQuery checkStudentQuery;
    int numInt=numeroEtudiant.toInt();
    checkStudentQuery.prepare("SELECT COUNT(*) FROM Etudiant WHERE numEtud = :numInt");
    checkStudentQuery.bindValue(":numInt", numInt);

    if (checkStudentQuery.exec() && checkStudentQuery.next()) {
        int studentCount = checkStudentQuery.value(0).toInt();
        return (studentCount > 0);
    } else {
        qDebug() << "Student Check Query Error: " << checkStudentQuery.lastError().text();
        return false;
    }
}


bool Etudiant::insererEtudiantDansBaseDeDonnees(const Etudiant &etudiant)
{


    QSqlQuery checkEtudiantQuery;
        checkEtudiantQuery.prepare("SELECT COUNT(*) FROM Etudiant WHERE nom = :nom AND prenom = :prenom");
        checkEtudiantQuery.bindValue(":nom", etudiant.getNom());
        checkEtudiantQuery.bindValue(":prenom", etudiant.getPrenom());

        if (checkEtudiantQuery.exec() && checkEtudiantQuery.next()) {
            int etudiantCount = checkEtudiantQuery.value(0).toInt();
            if (etudiantCount > 0) {
                QString errorMessage = "l'Etudiant \"" + etudiant.getNom()+ " " + etudiant.getPrenom()+ "\" existe déjà .";
                QMessageBox::critical(nullptr, "Erreur", errorMessage);
                qDebug() << "L'Etudiant \"" << etudiant.getNom() << "\" existe .";
                return false;
            }
        } else {
            qDebug() << "Erreur de vérification du l'etudiant:" << checkEtudiantQuery.lastError().text();
            return false;
        }

    QSqlQuery query;

    query.prepare("INSERT INTO Etudiant (nom, prenom, specialite) VALUES (:nom, :prenom, :specialite)");
    query.bindValue(":nom", etudiant.getNom());
    query.bindValue(":prenom", etudiant.getPrenom());
    query.bindValue(":specialite", etudiant.getSpecialite());

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur d'insertion:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Etudiant::getListeEtudiant() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT Etudiant.numEtud AS 'N°Etudiant', Etudiant.nom AS 'Nom', Etudiant.prenom AS 'Prenom', Etudiant.specialite AS 'Specialite' "
                       "FROM Etudiant ");
    return model;
}
