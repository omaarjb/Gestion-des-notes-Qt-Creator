#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QSqlQueryModel>

class Cours {
private:
    QString titre;
    QString codeCours;
    QString enseignantResponsable;
    static bool enseignantExiste(int enseignantID);

public:
    Cours();
    Cours(QString titre, QString enseignantResponsable);
    QString getTitre() const;
    QString getCodeCours() const;
    QString getEnseignantResponsable() const;
    static bool insererCoursDansBaseDeDonnees(const Cours &cours);
    static QSqlQueryModel* getListeCours();
    static bool checkCoursExiste(const QString& titre);

};

#endif // COURS_H
