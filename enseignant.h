#ifndef ENSEIGNANT_H
#define ENSEIGNANT_H
#include <QSqlQueryModel>
#include <QString>

class Enseignant {
private:
    QString nom;
    QString prenom;
    QString identifiant;
public:
    Enseignant(QString nom, QString prenom);
    Enseignant();
    QString getNom() const;
    QString getPrenom() const;
    QString getIdentifiant() const;
    static QSqlQueryModel* getListeEnseignant();
    static bool checkEnseignantExiste(const QString& identifiant);
    static bool insererEnseignantDansBaseDeDonnees(const Enseignant &enseignant);
    static bool supprimerEnseignant(const QString &identifiant);

};

#endif // ENSEIGNANT_H
