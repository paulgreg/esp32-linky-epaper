# esp32-linky-epaper

> **N.B**: This tool is targeted for french people so the documentation is in french

Le but de ce projet est d’exposer la consommation électrique du foyer des 7 derniers jours.

Le [projet est sur la plateforme hackaday](https://hackaday.io/project/177065-linky-daily-consumption) pour sa partie « électronique ».

Ce projet nécessite un compteur linky, installé par Enedis (seulement en France à ma connaissance, d’où la documentation en français).

L’idée est d’utiliser un esp32 couplé à un écran à encre électronique. L’intérêt d’un tel écran est qu’il laisse les infos affichés même s’il n’est plus alimenté.

Le micro-controlleur peut ainsi récupèrer les dernières données, les mettre en forme sur l’écran puis se mettre en veille jusqu’au lendemain.

## Partie serveur

Étant donné qu’il semble compliqué de récupèrer directement les données du linky, j’utilise l’excellent paquet nodejs [bokub/linky](https://github.com/bokub/linky) pour extraire les données auprès d’enedis et de créer des fichiers json sur mon serveur web. Ces fichiers sont ensuite requêtés par l’esp32.

Il faut donc, sur le serveur, créer un répertoire et installer le paquet linky : `mkdir linky && npm i linky`

Lancer ensuite l’authentification via `./node_module/.bin/linky auth...` tel que décrit dans le fichier [bokub/linky/README.md](https://github.com/bokub/linky/blob/master/README.md).

Vous pouvez ensuite lancer un tel script tous les jours (via crontab) pour générer des fichiers json :

    #!/bin/bash
    START=$(date --date="7 days ago"  +%Y-%m-%d)
    END=$(date --date="0 days ago"  +%Y-%m-%d)
    ./node_modules/.bin/linky daily     -s $START -e $END -o output/daily.json
    ./node_modules/.bin/linky maxpower  -s $START -e $END -o output/maxpower.json

Vous aurez besoin de rendre le répertoire `output` accessible via https et protégé par un mot de passe.