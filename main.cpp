#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

// Fonction utilitaire pour chercher et remplacer une chaîne exacte dans un fichier binaire
bool patchFichierBinaire(const std::string& cheminFichier, const std::string& ancienneChaine, const std::string& nouvelleChaine) {
    if (ancienneChaine.length() != nouvelleChaine.length()) {
        std::cerr << "Erreur : Pour éviter la corruption, les deux chaînes doivent avoir exactement la même longueur." << std::endl;
        return false;
    }

    std::ifstream fichierIn(cheminFichier, std::ios::binary);
    if (!fichierIn) return false;

    // Lecture de tout le fichier en mémoire
    std::vector<char> contenu((std::istreambuf_iterator<char>(fichierIn)), std::istreambuf_iterator<char>());
    fichierIn.close();

    bool modifie = false;
    size_t tailleAncienne = ancienneChaine.length();

    // Recherche de la chaîne d'octets
    for (size_t i = 0; i <= contenu.size() - tailleAncienne; ++i) {
        bool correspondance = true;
        for (size_t j = 0; j < tailleAncienne; ++j) {
            if (contenu[i + j] != ancienneChaine[j]) {
                correspondance = false;
                break;
            }
        }

        if (correspondance) {
            // Remplacement des octets sur place
            for (size_t j = 0; j < tailleAncienne; ++j) {
                contenu[i + j] = nouvelleChaine[j];
            }
            modifie = true;
            i += tailleAncienne - 1; // Avancer l'index
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
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <apk_entree> <apk_sortie>" << std::endl;
        return 1;
    }

    std::string apkEntree = argv[1];
    std::string apkSortie = argv[2];

    std::cout << "[Moteur C++] Début de la chirurgie binaire..." << std::endl;

    // 1. Création d'un dossier temporaire de travail
    std::system("mkdir -p temp_extraction");

    // 2. Décompression native de l'APK d'entrée
    std::string cmdUnzip = "unzip -q " + apkEntree + " -d temp_extraction";
    if (std::system(cmdUnzip.c_str()) != 0) {
        std::cerr << "Erreur lors de la décompression de l'APK." << std::endl;
        return 1;
    }

    // 3. Application des patchs binaires
    // Exemple : Remplacement du package name (longueur identique obligatoire, ex: 15 caractères)
    // À adapter dynamiquement selon l'APK cible
    std::string ancienPackage = "com.original.app"; 
    std::string nouveauPackage = "com.original.apc"; 
    patchFichierBinaire("temp_extraction/AndroidManifest.xml", iancienPackage, nouveauPackage);

    // Exemple : Remplacement du nom de l'application s'il est défini dans le String Pool de resources.arsc
    std::string ancienNom = "ApplicationOriginale";
    std::string nouveauNom = "ApplicationClonageee";
    patchFichierBinaire("temp_extraction/resources.arsc", ancienNom, nouveauNom);

    // 4. Re-compression de l'APK modifié
    std::string cmdZip = "cd temp_extraction && zip -r -q ../" + apkSortie + " .";
    std::system(cmdZip.c_str());

    // 5. Nettoyage du dossier temporaire
    std::system("rm -rf temp_extraction");

    std::cout << "[Moteur C++] Clonage et empaquetage terminés avec succès." << std::endl;
    return 0;
}
