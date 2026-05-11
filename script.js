import { pipeline } from 'https://cdn.jsdelivr.net/npm/@xenova/transformers@2.17.1';

const output = document.getElementById('output');
const input = document.getElementById('input');
const btn = document.getElementById('generate-btn');
const micBtn = document.getElementById('mic-btn');

let generator;

// 1. Initialisation de l'IA avec un modèle plus "intelligent" pour le dialogue
async function initIA() {
    try {
        output.innerText = "Téléchargement du cerveau technique (78MB)...";
        // LaMini-Flan-T5 est bien plus performant que GPT-2 pour répondre à des questions
        generator = await pipeline('text2text-generation', 'Xenova/LaMini-Flan-T5-78M');
        output.innerText = "Je suis prêt. Posez-moi une question sur vos circuits ou votre code.";
    } catch (e) {
        output.innerText = "Erreur de chargement. Vérifiez votre connexion.";
    }
}

// 2. Reconnaissance Vocale
const recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
recognition.lang = 'fr-FR';

micBtn.addEventListener('click', () => {
    recognition.start();
    output.innerText = "Je vous écoute...";
});

recognition.onresult = (event) => {
    const speechToText = event.results[0][0].transcript;
    input.value = speechToText;
    btn.click(); // Lance la génération automatiquement
};

// 3. Logique de génération avec contexte "Expert"
btn.addEventListener('click', async () => {
    if (!generator) return;
    
    const userText = input.value;
    output.innerText = "Analyse technique en cours...";

    // On force l'IA à rester dans son rôle d'expert
    const contextPrompt = `Expert technique NigerAgri : Répond de manière précise et pro à : ${userText}`;
    
    const result = await generator(contextPrompt, {
        max_new_tokens: 150,
        temperature: 0.4, // Plus précis, moins d'invention
    });
    
    const finalResponse = result[0].generated_text;
    output.innerText = finalResponse;

    // Optionnel : L'IA parle aussi !
    const speech = new SpeechSynthesisUtterance(finalResponse);
    speech.lang = 'fr-FR';
    window.speechSynthesis.speak(speech);
});

initIA();
