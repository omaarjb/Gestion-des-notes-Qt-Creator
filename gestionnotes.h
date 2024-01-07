#ifndef GESTIONNOTES_H
#define GESTIONNOTES_H

#include <QList>

class Etudiant;
class Cours;
class Note;
class Enseignant;

class GestionNotes {
private:
    QList<Etudiant*> listeEtudiants;
    QList<Cours*> listeCours;
    QList<Note*> listeNotes;
    QList<Enseignant*> listeEnseignants;
public:
    GestionNotes();
    void enregistrerNote(QString idEtud, QString Cours, float noteObtenue, QDateTime dateEvaluation);
    void consulterNotes(Etudiant* etudiant);
    void validerNotes();
    void notifierEtudiants();


};

#endif // GESTIONNOTES_H
