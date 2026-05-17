const express = require('express');
const multer = require('multer');
const cors = require('cors');
const { execFile } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const port = process.env.PORT || 3000;

app.use(cors());
const upload = multer({ dest: 'uploads/' });

app.post('/clone', upload.single('apk'), (req, res) => {
    if (!req.file) return res.status(400).send('Aucun fichier reçu.');

    // Récupération des données du formulaire web
    const { ancienPackage, nouveauPackage, ancienNom, nouveauNom } = req.body;

    if (!ancienPackage || !nouveauPackage || !ancienNom || !nouveauNom) {
        if (fs.existsSync(req.file.path)) fs.unlinkSync(req.file.path);
        return res.status(400).send('Données de ciblage manquantes.');
    }

    const inputApkPath = req.file.path;
    const outputApkName = `Clone_${req.file.originalname}`;
    const outputApkPath = path.join(__dirname, 'outputs', outputApkName);

    if (!fs.existsSync('outputs')) fs.mkdirSync('outputs');

    // On passe tous les paramètres au binaire C++
    execFile('./moteur_cloner.out', [
        inputApkPath, 
        outputApkPath, 
        ancienPackage, 
        nouveauPackage, 
        ancienNom, 
        nouveauNom
    ], (error, stdout, stderr) => {
        if (error) {
            console.error(stderr);
            if (fs.existsSync(inputApkPath)) fs.unlinkSync(inputApkPath);
            return res.status(500).send('Erreur lors de la modification binaire.');
        }

        res.download(outputApkPath, outputApkName, (err) => {
            if (fs.existsSync(inputApkPath)) fs.unlinkSync(inputApkPath);
            if (fs.existsSync(outputApkPath)) fs.unlinkSync(outputApkPath);
        });
    });
});

app.listen(port, () => {
    console.log(`Serveur actif sur le port ${port}`);
});
