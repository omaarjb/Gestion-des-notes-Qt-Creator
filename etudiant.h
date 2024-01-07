#ifndef ETUDIANT_H
#define ETUDIANT_H
#include <QString>
#include <QSqlQueryModel>
class Etudiant {
private:
    QString nom;
    QString prenom;
    QString numeroEtudiant;
    QString specialite;

public:
    Etudiant(QString nom, QString prenom, QString specialite);
    Etudiant();
    QString getNom() const;
    QString getPrenom() const;
    QString getNumeroEtudiant() const;
    QString getSpecialite() const;
    static bool checkEtudiantExiste(const QString& numeroEtudiant);
    static bool insererEtudiantDansBaseDeDonnees(const Etudiant &etudiant);
    static QSqlQueryModel* getListeEtudiant();
};

#endif // ETUDIANT_H
