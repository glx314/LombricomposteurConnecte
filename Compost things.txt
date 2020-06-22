
Paramètres :
Avec capteurs :
	- Température -> entre 15 et 25 celsius
	- Humidité -> entre 75 et 85%
	- gaz -> 30:1 carbon/nitrogen + 10% oxygène
	- pH percolat (et général) -> entre 6.5 et 8. (besoin de traitement ultérieur, percolat doit faire del'engrais acide)

Sans capteurs :
	- Lumière -> endroit sombre
	- Bruit -> endroit calme
	- Aération -> endroit bien aéré (processus aérobie)


CONTRAINTES : 
	Le compost doit être retourné régulièrement + cuve fermée (pour la lumière).
	prix ?
	durée de vie des équipements

CAPTEURS :
	Voir capteurs

PLACEMENT DES CAPTEURS : /!\ contraintes
    
    Temperature/humidité :		
	Dans un premier temps : deux sondes "à main", fils suffisament long. Arduino -> boitier (dedans, dehors ?)

    Capteurs gaz : placés au dessus (haut de la cuve)

    Sonde pH : dans le seau qui récolte le percolat (immersion constante != tests réguliers) -> reliés à la même arduino (problèmes de fils)


TRAVAIL SUR LE CODE :

	détecter les trop grandes variations : moyenne des valeurs sur une heure ? une journée ? stocker données pour vérifier à la main
	Si variation : prévenir si on approche d'un seuil. Rappeler les causes possibles et les solutions envisageables.
	Mettre en place la communication arduino/machines (avec Lora)


TRAITEMENT DES DONNEES





INTERFACE UTILISATEUR