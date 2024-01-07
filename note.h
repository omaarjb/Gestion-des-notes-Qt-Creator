#ifndef NOTE_H
#define NOTE_H

#include <QDateTime>


class Note {
private:
            QString idEtud;
            QString cours;
            QString noteObtenue;
            QString dateEvaluation;
public:
    Note(QString idEtud, QString Cours, QString noteObtenue, QString dateEvaluation);
    QString getNumeroEtudiant() const;
    QString getCours() const;
    QString getNote() const;
    QString getDateEvaluation() const;
    static bool insererNoteDansBaseDeDonnees(const Note &note);


};

#endif // NOTE_H
