#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

bool patchFichierBinaire(const std::string& cheminFichier, const std::string& ancienneChaine, const std::string& nouvelleChaine) {
    if (ancienneChaine.length() != nouvelleChaine.length()) {
        std::cerr << "Erreur de taille : " << ancienneChaine.length() << " vs " << nouvelleChaine.length() << std::endl;
        return false;
    }

    std::ifstream fichierIn(cheminFichier, std::ios::binary);
    if (!fichierIn) return false;

    std::vector<char> contenu((std::istreambuf_iterator<char>(fichierIn)), std::istreambuf_iterator<char>());
    fichierIn.close();

    bool modifie = false;
    size_t tailleAncienne = ancienneChaine.length();

    for (size_t i = 0; i <= contenu.size() - tailleAncienne; ++i) {
        bool correspondance = true;
        for (size_t j = 0; j < tailleAncienne; ++j) {
            if (contenu[i + j] != ancienneChaine[j]) {
                correspondance = false;
                break;
            }
        }

        if (correspondance) {
            for (size_t j = 0; j < tailleAncienne; ++j) {
                contenu[i + j] = nouvelleChaine[j];
            }
            modifie = true;
            i += tailleAncienne - 1;
        }
    }

    if (modifie) {
        std::ofstream fichierOut(cheminFichier, std::ios::binary);
        fichierOut.write(contenu.data(), contenu.size());
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    // Vérification des arguments : entrée, sortie, ancien_pkg, nouveau_pkg, ancien_nom, nouveau_nom
    if (argc < 7) {
        std::cerr << "Arguments manquants pour le clonage." << std::endl;
        return 1;
    }

    std::string apkEntree = argv[1];
    std::string apkSortie = argv[2];
    std::string ancienPackage = argv[3];
    std::string nouveauPackage = argv[4];
    std::string ancienNom = argv[5];
    std::string nouveauNom = argv[6];

    std::cout << "[C++] Début de la modification de l'APK..." << std::endl;

    std::system("mkdir -p temp_extraction");

    std::string cmdUnzip = "unzip -q " + apkEntree + " -d temp_extraction";
    if (std::system(cmdUnzip.c_str()) != 0) {
        std::cerr << "Échec de l'extraction." << std::endl;
        return 1;
    }

    // Application des patchs avec les valeurs reçues
    patchFichierBinaire("temp_extraction/AndroidManifest.xml", ancienPackage, nouveauPackage);
    patchFichierBinaire("temp_extraction/resources.arsc", ancienNom, nouveauNom);

    std::string cmdZip = "cd temp_extraction && zip -r -q ../" + apkSortie + " .";
    std::system(cmdZip.c_str());
    std::system("rm -rf temp_extraction");

    std::cout << "[C++] Processus terminé avec succès." << std::endl;
    return 0;
}
