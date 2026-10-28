#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

using namespace std;
using namespace std::chrono;

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droit;

    Noeud(int val) {
        valeur = val;
        gauche = nullptr;
        droit = nullptr;
    }
};

Noeud* inserer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return new Noeud(valeur);
    }

    if (valeur < racine->valeur) {
        racine->gauche = inserer(racine->gauche, valeur);
    }
    else if (valeur > racine->valeur) {
        racine->droit = inserer(racine->droit, valeur);
    }

    return racine;
}

bool rechercher(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return false;
    }

    if (racine->valeur == valeur) {
        return true;
    }

    if (valeur < racine->valeur) {
        return rechercher(racine->gauche, valeur);
    }
    else {
        return rechercher(racine->droit, valeur);
    }
}

Noeud* trouverMin(Noeud* racine) {
    while (racine != nullptr && racine->gauche != nullptr) {
        racine = racine->gauche;
    }
    return racine;
}

// 3. SUPPRESSION
Noeud* supprimer(Noeud* racine, int valeur) {
    if (racine == nullptr) return nullptr;

    if (valeur < racine->valeur) {
        racine->gauche = supprimer(racine->gauche, valeur);
    }
    else if (valeur > racine->valeur) {
        racine->droit = supprimer(racine->droit, valeur);
    }
    else {
        // Nœud trouvé !

        // Cas 1 : 0 ou 1 enfant droit
        if (racine->gauche == nullptr) {
            Noeud* temp = racine->droit;
            delete racine;
            return temp;
        }
        // Cas 2 : 1 enfant gauche
        else if (racine->droit == nullptr) {
            Noeud* temp = racine->gauche;
            delete racine;
            return temp;
        }

        // Cas 3 : 2 enfants
        // On récupère le successeur (minimum du sous-arbre droit)
        Noeud* temp = trouverMin(racine->droit);
        racine->valeur = temp->valeur;
        // On supprime le successeur
        racine->droit = supprimer(racine->droit, temp->valeur);
    }
    return racine;
}

void afficherTrie(Noeud* racine) {
    if (racine == nullptr) return;

    afficherTrie(racine->gauche);
    cout << racine->valeur << " ";
    afficherTrie(racine->droit);
}

void libererArbre(Noeud* racine) {
    if (racine == nullptr) return;
    libererArbre(racine->gauche);
    libererArbre(racine->droit);
    delete racine;
}

int main() {
    Noeud* racine = nullptr;

    ///elements tries sans map
    const int N = 10000;

    vector<int> valeurs(N);
    for (int i = 0; i < N; i++) {
        valeurs[i] = i + 1;
    }

    // 1. MESURE DE L'INSERTION
    auto start_insert = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        racine = inserer(racine, valeurs[i]);
    }
    auto end_insert = high_resolution_clock::now();
    auto duree_insert = duration_cast<nanoseconds>(end_insert - start_insert).count();
    cout << "Insertion de " << N << " elements tries terminee." << endl;

    /*
    cout << "Arbre apres insertion : ";
    afficherTrie(racine);
    cout << endl;
    */

    // 2. MESURE DE LA RECHERCHE
    int cible_recherche = N;
    auto start_search = high_resolution_clock::now();
    bool trouve = rechercher(racine, cible_recherche);
    auto end_search = high_resolution_clock::now();
    auto duree_search = duration_cast<nanoseconds>(end_search - start_search).count();

    // 3. MESURE DE LA SUPPRESSION
    int cible_suppr = N/2;
    auto start_suppr = high_resolution_clock::now();
    racine = supprimer(racine, cible_suppr);
    auto end_suppr = high_resolution_clock::now();
    auto duree_suppr = duration_cast<nanoseconds>(end_suppr - start_suppr).count();

    /*
    cout << "Arbre apres suppression de (" << cible_suppr << ") : ";
    afficherTrie(racine);
    cout << "\n" << endl;
    */

    // AFFICHAGE RECAPITULATIF DES 3 TEMPS
    cout << "\n=== RECAPITULATIF DES PERFORMANCES (N = " << N << " elements tries) ===" << endl;
    cout << "1. Temps d'INSERTION : " << duree_insert << " ns" << endl;
    cout << "2. Temps de RECHERCHE (cle " << cible_recherche << ")   : " << duree_search << " ns (Trouve : " << (trouve ? "Oui" : "Non") << ")" << endl;
    cout << "3. Temps de SUPPRESSION (cle " << cible_suppr << ") : " << duree_suppr << " ns" << endl;
    libererArbre(racine);


    cout << "\n---------------------------------------------------\n" << endl;
    ///elements aleatoires sans map
    {
        Noeud* racineAleatoire = nullptr;
        vector<int> valeursAleatoires(N);
        for (int i = 0; i < N; i++) {
            valeursAleatoires[i] = i + 1;
        }

        // Mélange aléatoire des éléments
        random_device rd;
        mt19937 g(rd());
        shuffle(valeursAleatoires.begin(), valeursAleatoires.end(), g);

        // 1. Insertion
        auto start_insert = high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            racineAleatoire = inserer(racineAleatoire, valeursAleatoires[i]);
        }
        auto end_insert = high_resolution_clock::now();
        auto duree_insert = duration_cast<nanoseconds>(end_insert - start_insert).count();
        cout << "Insertion de " << N << " elements aleatoires terminee." << endl;

        // 2. Recherche
        int cible_recherche = valeursAleatoires[N-1];
        auto start_search = high_resolution_clock::now();
        bool trouve = rechercher(racineAleatoire, cible_recherche);
        auto end_search = high_resolution_clock::now();
        auto duree_search = duration_cast<nanoseconds>(end_search - start_search).count();

        // 3. Suppression
        int cible_suppr = valeursAleatoires[N/2];
        auto start_suppr = high_resolution_clock::now();
        racineAleatoire = supprimer(racineAleatoire, cible_suppr);
        auto end_suppr = high_resolution_clock::now();
        auto duree_suppr = duration_cast<nanoseconds>(end_suppr - start_suppr).count();

        // Affichage récapitulatif 2
        cout << "\n=== RECAPITULATIF : ELEMENTS ALEATOIRES (N = " << N << ") ===" << endl;
        cout << "1. Temps d'INSERTION : " << duree_insert << " ns" << endl;
        cout << "2. Temps de RECHERCHE (cle " << cible_recherche << ") : " << duree_search << " ns (Trouve : " << (trouve ? "Oui" : "Non") << ")" << endl;
        cout << "3. Temps de SUPPRESSION (cle " << cible_suppr << ") : " << duree_suppr << " ns" << endl;

        libererArbre(racineAleatoire);
    }

    cout << "\n---------------------------------------------------\n" << endl;

    ///elements tries avec map
    {
        map<int, int> monMapTrie;
        vector<int> valeurs(N);
        for (int i = 0; i < N; i++) {
            valeurs[i] = i + 1;
        }

        // 1. Insertion
        auto start_insert = high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            monMapTrie[valeurs[i]] = valeurs[i];
        }
        auto end_insert = high_resolution_clock::now();
        auto duree_insert = duration_cast<nanoseconds>(end_insert - start_insert).count();
        cout << "Insertion de " << N << " elements tries (std::map) terminee." << endl;

        // 2. Recherche (std::find / map::find)
        int cible_recherche = N;
        auto start_search = high_resolution_clock::now();
        auto it_f = monMapTrie.find(cible_recherche);
        bool trouve = (it_f != monMapTrie.end());
        auto end_search = high_resolution_clock::now();
        auto duree_search = duration_cast<nanoseconds>(end_search - start_search).count();

        // 3. Suppression (map::erase)
        int cible_suppr = N / 2;
        auto start_suppr = high_resolution_clock::now();
        monMapTrie.erase(cible_suppr);
        auto end_suppr = high_resolution_clock::now();
        auto duree_suppr = duration_cast<nanoseconds>(end_suppr - start_suppr).count();

        cout << "\n=== RECAPITULATIF : STD::MAP - TRIES (N = " << N << ") ===" << endl;
        cout << "1. Temps d'INSERTION : " << duree_insert << " ns" << endl;
        cout << "2. Temps de RECHERCHE (cle " << cible_recherche << ")   : " << duree_search << " ns (Trouve : " << (trouve ? "Oui" : "Non") << ")" << endl;
        cout << "3. Temps de SUPPRESSION (cle " << cible_suppr << ") : " << duree_suppr << " ns" << endl;
    }

    cout << "\n---------------------------------------------------\n" << endl;

    ///elements tries avec map
    {
        map<int, int> monMapAleatoire;
        vector<int> valeursAleatoires(N);
        for (int i = 0; i < N; i++) {
            valeursAleatoires[i] = i + 1;
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(valeursAleatoires.begin(), valeursAleatoires.end(), g);

        // 1. Insertion
        auto start_insert = high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            monMapAleatoire[valeursAleatoires[i]] = valeursAleatoires[i];
        }
        auto end_insert = high_resolution_clock::now();
        auto duree_insert = duration_cast<nanoseconds>(end_insert - start_insert).count();
        cout << "Insertion de " << N << " elements aleatoires (std::map) terminee." << endl;

        // 2. Recherche
        int cible_recherche = valeursAleatoires[N - 1];
        auto start_search = high_resolution_clock::now();
        auto it_f = monMapAleatoire.find(cible_recherche);
        bool trouve = (it_f != monMapAleatoire.end());
        auto end_search = high_resolution_clock::now();
        auto duree_search = duration_cast<nanoseconds>(end_search - start_search).count();

        // 3. Suppression
        int cible_suppr = valeursAleatoires[N / 2];
        auto start_suppr = high_resolution_clock::now();
        monMapAleatoire.erase(cible_suppr);
        auto end_suppr = high_resolution_clock::now();
        auto duree_suppr = duration_cast<nanoseconds>(end_suppr - start_suppr).count();

        cout << "\n=== RECAPITULATIF : STD::MAP - ALEATOIRES (N = " << N << ") ===" << endl;
        cout << "1. Temps d'INSERTION : " << duree_insert << " ns" << endl;
        cout << "2. Temps de RECHERCHE (cle " << cible_recherche << ")   : " << duree_search << " ns (Trouve : " << (trouve ? "Oui" : "Non") << ")" << endl;
        cout << "3. Temps de SUPPRESSION (cle " << cible_suppr << ") : " << duree_suppr << " ns" << endl;
    }



    return 0;
}
