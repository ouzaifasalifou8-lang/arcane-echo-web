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

    const inputApkPath = req.file.path;
    const outputApkName = `Clone_${req.file.originalname}`;
    const outputApkPath = path.join(__dirname, 'outputs', outputApkName);

    if (!fs.existsSync('outputs')) fs.mkdirSync('outputs');

    // On appelle notre binaire C++ compilé par Render
    execFile('./moteur_cloner.out', [inputApkPath, outputApkPath], (error, stdout, stderr) => {
        if (error) {
            if (fs.existsSync(inputApkPath)) fs.unlinkSync(inputApkPath);
            return res.status(500).send('Erreur de manipulation binaire C++.');
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
