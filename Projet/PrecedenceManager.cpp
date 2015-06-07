#include "PrecedenceManager.h"

void PrecedenceManager::ajouterPrecedence(const Tache &Tpred, const Tache &Tsucc){
    if (Tpred.getDateDisponibilite() > Tsucc.getDateEcheance())
        throw CalendarException("erreur, PrecedenceManager, tentative de programmer une précédence avec une tache disponible après échéance de sa successeur.");
    for (int i = 0; i < precedences.size(); ++i) {
        if (&precedences[i]->getPredecesseur() == &Tpred && &precedences[i]->getSuccesseur() == &Tsucc)
            throw CalendarException("erreur, PrecedenceManager, précédence déjà existante");
        if (&precedences[i]->getPredecesseur() == &Tsucc && &precedences[i]->getSuccesseur() == &Tpred)
            throw CalendarException("erreur, PrecedenceManager, tentative d'implémenter un circuit de précédences");
    };
    Precedence* P = new Precedence(Tpred, Tsucc);
    precedences.append(P);
}

void PrecedenceManager::supprimerPrecedence(const Tache& Tpred, const Tache& Tsucc){
    if (PrecedenceManager::isPrecedence(Tpred, Tsucc))
        for (int i = 0; i < precedences.size(); ++i)
            if (&precedences[i]->getPredecesseur() == &Tpred && &precedences[i]->getSuccesseur() == &Tsucc)
                precedences.removeAt(i);
    else
        throw CalendarException("erreur, PrecedenceManager, tentative d'effacer une precedence qui n'existe pas.");

}

ListTachesConst PrecedenceManager::trouverPrecedences(const Tache& Tsucc) const{
    ListTachesConst LT;
    for (int i = 0; i < precedences.size(); ++i)
        if (&precedences[i]->getSuccesseur() == &Tsucc)
            LT.append(&precedences[i]->getPredecesseur());
    return LT;
}

bool PrecedenceManager::isPrecedence(const Tache& Tpred, const Tache& Tsucc) const {
    for (int i = 0; i < precedences.size(); ++i)
        if ((&precedences[i]->getPredecesseur() == &Tpred) && (&precedences[i]->getSuccesseur() == &Tsucc))
            return true;
    return false;
}