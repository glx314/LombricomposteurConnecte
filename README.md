# ArduinoCompost
=======
Quelques informations sur le compost :

Paramètres optimaux du compost:

	Avec capteurs :
		- Température -> entre 15 et 25 celsius
		- Humidité -> entre 75 et 85%
		- gaz -> 30:1 carbon/azote + 10% oxygène
		- pH percolat (et général) -> entre 6.5 et 8. (besoin de traitement ultérieur, percolat doit faire del'engrais acide)

	Sans capteurs :
		- Lumière -> endroit sombre
		- Bruit -> endroit calme
		- Aération -> endroit bien aéré

A PROPOS DU PROJET :

	CONTRAINTES : 
		Le compost doit être retourné régulièrement + cuve fermée (pour la lumière).
		durée de vie des équipements
		prix des équipements

	CAPTEURS RETENUS:
		Voir Hardware.txt ( https://github.com/GuitouBDA/ArduinoCompost/blob/guillaume/Hardware.txt )

	PLACEMENT DES CAPTEURS : /!\ contraintes
    
		Temperature/humidité : deux sondes filaires plongées dans le compost

		Sonde pH : pour une mesure, à placer dans le seau qui récolte le percolat.Les relevés sont ponctuels, la sonde ne doit pas restée immergée.

	UTILISATION : Voir Notice.txt ( https://github.com/GuitouBDA/ArduinoCompost/blob/guillaume/Notice.txt ) 

	AMELIORATIONS POSSIBLE : 
		Notification quand un paramètre sort des normes ou connait une évolution anormale.
		Mesurer la masse du compost pour determiner la masse de terreau/de vers.

	INTERFACE UTILISATEUR
		Données poussées dans Stellio (voir wiki).
		Dashboard à implémenter.
