import { pipeline } from 'https://cdn.jsdelivr.net/npm/@xenova/transformers@2.17.1';

const output = document.getElementById('output');
const input = document.getElementById('input');
const btn = document.getElementById('generate-btn');

let generator;

// Chargement du modèle au démarrage
async function initIA() {
    output.innerText = "Chargement du modèle source (veuillez patienter)...";
    // On utilise un modèle très léger (ex: GPT-2 ou SmolLM)
    generator = await pipeline('text-generation', 'Xenova/gpt2');
    output.innerText = "Modèle prêt ! Posez une question.";
}

btn.addEventListener('click', async () => {
    if (!generator) return;
    
    const text = input.value;
    output.innerText = "L'IA réfléchit...";
    
    const result = await generator(text, {
        max_new_tokens: 50,
        temperature: 0.7,
    });
    
    output.innerText = result[0].generated_text;
});

initIA();
