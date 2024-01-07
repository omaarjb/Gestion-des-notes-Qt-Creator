#include "cours.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QtSql>
#include <QMessageBox>
Cours::Cours(QString titre, QString enseignantResponsable)
{
    this->titre = titre;
    this->enseignantResponsable = enseignantResponsable;
}

Cours :: Cours(){
    this->titre="";
    this->enseignantResponsable = "";
}

QString Cours::getTitre() const { return titre; }
QString Cours::getCodeCours() const { return codeCours; }
QString Cours::getEnseignantResponsable() const { return enseignantResponsable; }

bool Cours::enseignantExiste(int enseignantID)
{
    QSqlQuery query;
        query.prepare("SELECT id FROM Enseignant WHERE id = :id");
        query.bindValue(":id", enseignantID);


    if (query.exec() && query.next()) {
        return true;
    } else {
        return false;
    }
}

bool Cours::insererCoursDansBaseDeDonnees(const Cours &cours)
{

    int enseignantID = cours.getEnseignantResponsable().toInt();
    if (!enseignantExiste(enseignantID)) {
        QString errorMessage = "L'enseignant avec l'ID " + QString::number(enseignantID) + " n'existe pas.";
        QMessageBox::critical(nullptr, "Erreur", errorMessage);
        qDebug() << "L'enseignant avec l'ID" << enseignantID << "n'existe pas.";
        return false;
    }

    QSqlQuery checkCourseQuery;
        checkCourseQuery.prepare("SELECT COUNT(*) FROM Cours WHERE titre = :titre AND responsable = :responsable");
        checkCourseQuery.bindValue(":titre", cours.getTitre());
        checkCourseQuery.bindValue(":responsable", enseignantID);

        if (checkCourseQuery.exec() && checkCourseQuery.next()) {
            int courseCount = checkCourseQuery.value(0).toInt();
            if (courseCount > 0) {
                QString errorMessage = "Le cours \"" + cours.getTitre() + "\" existe déjà pour ce responsable.";
                QMessageBox::critical(nullptr, "Erreur", errorMessage);
                qDebug() << "Le cours \"" << cours.getTitre() << "\" existe déjà pour le responsable.";
                return false;
            }
        } else {
            qDebug() << "Erreur de vérification du cours:" << checkCourseQuery.lastError().text();
            return false;
        }

    QSqlQuery query;

    query.prepare("INSERT INTO Cours (titre, responsable) VALUES (:titre, :responsable)");
    query.bindValue(":titre", cours.getTitre());
    query.bindValue(":responsable", enseignantID);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur d'insertion:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Cours::getListeCours() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT Cours.titre AS 'Titre du Cours', Enseignant.nom AS 'Nom Du Responsable' "
                       "FROM Cours "
                       "JOIN Enseignant ON Cours.responsable = Enseignant.id");
    return model;
}

bool Cours::checkCoursExiste(const QString& titre) {
    QSqlQuery checkCoursQuery;
    checkCoursQuery.prepare("SELECT COUNT(*) FROM Cours WHERE titre = :titre");
    checkCoursQuery.bindValue(":titre", titre);

    if (checkCoursQuery.exec() && checkCoursQuery.next()) {
        int coursCount = checkCoursQuery.value(0).toInt();
        return (coursCount > 0);
    } else {
        qDebug() << "Cours Check Query Error: " << checkCoursQuery.lastError().text();
        return false;
    }
}
