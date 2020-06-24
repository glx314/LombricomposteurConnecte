#include <LiquidCrystal.h> // on importe la bibliothèque
LiquidCrystal monEcran(12,11,A5,A4,A3,A2); // on crée l'objet écran

void setup() {
  monEcran.begin(16,2); //on initialise la communication avec 16 colonnes et deux lignes
  monEcran.clear(); // on efface l'écran
  monEcran.print("Bonjour,"); // on écrit Bonjour
  monEcran.setCursor(0,1); //on passe à la ligne suivante
  monEcran.print("OPENCLASSROOMS"); // on finit d'écrire
}

void loop() {
}
