#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QTreeView>

#include "Calendar.h"

class TreeView : public QWidget{
Q_OBJECT

private:
    QScrollArea *scrollareaP, *scrollareaT, *scrollareaA;
    QTreeView *treeViewP, *treeViewT, *treeViewA;
    QStandardItemModel *modelP, *modelT, *modelA;
    QList<QString> treePLabels, treeTLabels, treeALabels;
    QStandardItem *rootNodeP, *rootNodeT, *rootNodeA;
    QLabel *infoTacheLabel, *infoTache;

    QVBoxLayout* layout;

public:
    explicit TreeView(QWidget *parent = 0);
    ~TreeView();
    void ajouterProjetTree(QStandardItem* root, Projet& projet);
    void ajouterTacheTree(QStandardItem* pere, Tache& tache);
    void ajouterActiviteTree(QStandardItem* root, Evenement& evt);

public slots:
    void afficherInfos(const QModelIndex& index);
    void updateTrees();
};

#endif // TREEVIEW_H
