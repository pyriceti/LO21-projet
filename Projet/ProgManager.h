#ifndef PROGMANAGER_H
#define PROGMANAGER_H

#include "Calendar.h"

#include <QHash>

class ProgManager : public Singleton<ProgManager>{
private:
    ListEvent programmations;
    QHash<const TacheUnitaire*, QTime> tabDuree;

    friend class TacheUnitaire;

    void ajouterProgrammation(Evenement& E);
public:
    ~ProgManager();

    void ajouterProgrammationT(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& TU);
    void ajouterProgrammationA(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l);

    bool programmationExists(const QDate& d, const QTime& h, const QTime& fin) const;

    ProgrammationTache* trouverProgrammationT(const TacheUnitaire& TU) const;
    ProgrammationActivite* trouverProgrammationA(const ProgrammationActivite& PA) const;

    void updateDuree(const TacheUnitaire& TU, QTime d);

    ListEventConst getProgWeek(const QDate& jour) const;
    ListEventConst getProgProj(const Projet& projet) const;

    void loadEvts(QXmlStreamReader& xml);
    void loadDurees(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml) const;

    bool isEmpty() const { return programmations.isEmpty(); }

    class iterator{
        ListEvent::iterator current;
        iterator(ListEvent::iterator u):current(u){}
        friend class ProgManager;
    public:
        iterator(){}
        Evenement& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current != it.current; }
        iterator& operator++(){ ++current ; return *this; }
    };

    iterator begin(){ return iterator(programmations.begin()); }
    iterator end(){ return iterator(programmations.end()); }

    class const_iterator{
        ListEvent::const_iterator current;
        const_iterator(ListEvent::const_iterator u):current(u){}
        friend class ProgManager;
    public:
        const_iterator(){}
        const Evenement& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current != it.current; }
        const_iterator& operator++(){ ++current; return *this; }

    };

    const_iterator begin() const { return const_iterator(programmations.begin()); }
    const_iterator end() const { return const_iterator(programmations.end()); }

    class tabDureeIterator{
        QHash<const TacheUnitaire*, QTime>::iterator current;
        tabDureeIterator(QHash<const TacheUnitaire*, QTime>::iterator u):current(u){}
        friend class ProgManager;
    public:
        tabDureeIterator(){}
        QHash<const TacheUnitaire*, QTime>::iterator operator*() const { return current; }
        bool operator!=(tabDureeIterator it) const { return current != it.current; }
        tabDureeIterator& operator++(){ ++current ; return *this; }
    };
    tabDureeIterator tabDureeBegin(){ return tabDureeIterator(tabDuree.begin()); }
    tabDureeIterator tabDureeEnd(){ return tabDureeIterator(tabDuree.end()); }

    class const_tabDureeIterator{
        QHash<const TacheUnitaire*, QTime>::const_iterator current;
        const_tabDureeIterator(QHash<const TacheUnitaire*, QTime>::const_iterator u):current(u){}
        friend class ProgManager;
    public:
        const_tabDureeIterator(){}
        QHash<const TacheUnitaire*, QTime>::const_iterator operator*() const { return current; }
        bool operator!=(const_tabDureeIterator it) const { return current != it.current; }
        const_tabDureeIterator& operator++(){ ++current ; return *this; }
    };
    const_tabDureeIterator tabDureeBegin() const { return const_tabDureeIterator(tabDuree.begin()); }
    const_tabDureeIterator tabDureeEnd() const { return const_tabDureeIterator(tabDuree.end()); }
};

#endif // PROGMANAGER_H
