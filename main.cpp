#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Arguments manquants." << std::endl;
        return 1;
    }

    std::string apkEntree = argv[1];
    std::string apkSortie = argv[2];

    std::cout << "[C++] Réception de l'APK réussie." << std::endl;
    
    // Commande de copie temporaire pour tester la chaîne complète avant d'ajouter libzip
    std::string cmd = "cp " + apkEntree + " " + apkSortie;
    int res = std::system(cmd.c_str());

    if (res == 0) {
        std::cout << "[C++] Fichier dupliqué avec succès." << std::endl;
        return 0;
    } else {
        return 2;
    }
}
