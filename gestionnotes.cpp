#include "gestionnotes.h"
#include "etudiant.h"
#include "cours.h"
#include "note.h"
#include "enseignant.h"

GestionNotes::GestionNotes() {

}

void GestionNotes::enregistrerNote(QString idEtud, QString Cours, float noteObtenue, QDateTime dateEvaluation) {
    Note* nouvelleNote = new Note(idEtud,  Cours,  noteObtenue,  dateEvaluation);
    listeNotes.append(nouvelleNote);
}

void GestionNotes::consulterNotes(Etudiant* etudiant) {

}

void GestionNotes::validerNotes() {

}

void GestionNotes::notifierEtudiants() {

}
