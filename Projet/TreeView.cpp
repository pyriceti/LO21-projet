#include "TreeView.h"
#include "TacheManager.h"
#include "ProgManager.h"

TreeView::TreeView(QWidget *parent) : QWidget(parent){

    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();
    ProgManager& ProgM = *ProgManager::getInstance();

    setMinimumHeight(600);

    // treeViewProjets
    treeViewP = new QTreeView(this);
    modelP = new QStandardItemModel(this);
    treePLabels << "Projets et tâches associées";
    modelP->setHorizontalHeaderLabels (QStringList(treePLabels));
    rootNodeP = modelP->invisibleRootItem();

    // treeViewTaches
    treeViewT = new QTreeView(this);
    modelT = new QStandardItemModel(this);
    treeTLabels << "Tâches hors projet";
    modelT->setHorizontalHeaderLabels (QStringList(treeTLabels));
    rootNodeT = modelT->invisibleRootItem();

    // treeViewActivitees
    treeViewA = new QTreeView(this);
    modelA = new QStandardItemModel(this);
    treeALabels << "Type" << "Titre" << "Description" << "Lieu" << "Date" << "Début" << "Fin" << "Préemptive";
    modelA->setHorizontalHeaderLabels (QStringList(treeALabels));
    rootNodeA = modelA->invisibleRootItem();


    // Initialisation
        // Construction de treeViewP et treeViewT
        for (ProjetManager::iterator i = PM.begin(); i != PM.end(); ++i)
            ajouterProjetTree(rootNodeP, *i);
        for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
            if (!TM.getTacheMere(*i)){
                Projet* P = PM.getProjet(*i);
                if (P) ajouterTacheTree(modelP->findItems(P->getTitre()).first(), *i);
                else ajouterTacheTree(rootNodeT, *i);
            }
        }
        // Construction de treeViewA
        for (ProgManager::iterator i = ProgM.begin(); i != ProgM.end(); ++i)
            ajouterActiviteTree(rootNodeA, *i);

    treeViewP->setModel(modelP);
    treeViewP->expandAll();
    treeViewT->setModel(modelT);
    treeViewT->expandAll();
    treeViewA->setModel(modelA);

    // Informations

    infoTacheLabel = new QLabel("Informations pour la sélection :", this);
    QFont f;
    f.setFamily("Times");
    f.setBold(true);
    infoTacheLabel->setFont(f);
    infoTacheLabel->setIndent(24);

    infoTache = new QLabel("Sélection vide", this);

    // Layout & Scroll

    scrollareaP = new QScrollArea;
    scrollareaT = new QScrollArea;
    scrollareaA = new QScrollArea;

    scrollareaP->setWidget(treeViewP);
    scrollareaP->setWidgetResizable(true);
    scrollareaP->setFixedHeight(160);

    scrollareaT->setWidget(treeViewT);
    scrollareaT->setWidgetResizable(true);
    scrollareaT->setFixedHeight(160);

    scrollareaA->setWidget(treeViewA);
    scrollareaA->setWidgetResizable(true);
    scrollareaA->setFixedHeight(160);

    layout = new QVBoxLayout;

    layout->addWidget(scrollareaP);
    layout->addWidget(scrollareaT);
    layout->addWidget(scrollareaA);
    layout->addWidget(infoTacheLabel);
    layout->addWidget(infoTache);
    layout->addStretch();

    this->setLayout(layout);

    // Signaux

    QObject::connect(treeViewP, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
    QObject::connect(treeViewT, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
    QObject::connect(treeViewA, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
}

TreeView::~TreeView(){}

void TreeView::ajouterProjetTree(QStandardItem* root, Projet& projet){
    QStandardItem* newItem =  new QStandardItem(projet.getTitre());
    root->appendRow(newItem);
    newItem->setEditable(false);
    newItem->setData(QVariant::fromValue(projet.getTitre()));
}

void TreeView::ajouterTacheTree(QStandardItem* pere, Tache& tache){
    QStandardItem* newItem =  new QStandardItem(tache.getTitre());
    pere->appendRow(newItem);
    newItem->setEditable(false);
    if(!tache.isTacheUnitaire()){
        TacheComposite& TC = dynamic_cast<TacheComposite&>(tache);
        if (!TC.getSousTaches().isEmpty()){
            for (int i = 0; i < TC.getSousTaches().size(); ++i)
                ajouterTacheTree(newItem, *TC.getSousTaches()[i]);
        }
    }
}

void TreeView::ajouterActiviteTree(QStandardItem* root, Evenement& evt){
    QList<QStandardItem*> row;
    QStandardItem* item;
    if (!evt.isProgTache()){
        ProgrammationActivite& progA = dynamic_cast<ProgrammationActivite&>(evt);
            item = new QStandardItem("Activité");
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getTitre());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getDescription());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getLieu());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getDate().toString("dd/MM/yyyy"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getHoraire().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getHoraireFin().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
    }
    else{
        ProgrammationTache& progT = dynamic_cast<ProgrammationTache&>(evt);
            item = new QStandardItem("Tâche");
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getTitre());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getDescription());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem();
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getDate().toString("dd/MM/yyyy"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getHoraire().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getHoraireFin().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
            if (progT.getTache().isPreemptive())
                item = new QStandardItem("Oui");
            else item = new QStandardItem("Non");
            item->setEditable(false);
            row.append(item);
    }
    root->appendRow(row);
}

void TreeView::afficherInfos(const QModelIndex& index){
    QString infos;
    if ((index.model() == modelP && index.parent().isValid()) || index.model() == modelT){
        QString titre = index.data().toString();
        infos.append("Tâche ");
        const Tache& tache = TacheManager::getInstance()->getTache(titre);
        if (tache.isTacheUnitaire()) infos.append("unitaire :\n");
        else infos.append("composite :\n");
        tache.ajouterInfos(infos);
    }
    else if (index.model() == modelP){
        QString titre = index.data().toString();
        infos.append("Projet :\n");
        const Projet& projet = ProjetManager::getInstance()->getProjet(titre);
        projet.ajouterInfos(infos);
    }
    else{
        infos.append("Cf. ci-dessus pour les détails de la programmation sélectionnée.");
    }
    infoTache->setText(infos);
}

void TreeView::updateTrees(){
    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();
    ProgManager& ProgM = *ProgManager::getInstance();

    // Cleaning
    modelP->clear();
    modelT->clear();
    modelA->clear();

    // Construction de treeViewP et treeViewT
    for (ProjetManager::iterator i = PM.begin(); i != PM.end(); ++i)
        ajouterProjetTree(rootNodeP, *i);
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        if (!TM.getTacheMere(*i)){
            Projet* P = PM.getProjet(*i);
            if (P) ajouterTacheTree(modelP->findItems(P->getTitre()).first(), *i);
            else ajouterTacheTree(rootNodeT, *i);
        }
    }

    // Construction de treeViewA
    for (ProgManager::iterator i = ProgM.begin(); i != ProgM.end(); ++i)
        ajouterActiviteTree(rootNodeA, *i);
}
