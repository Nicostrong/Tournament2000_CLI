🇫🇷 **Français** | [🇬🇧 Read in English](readme.en.md)

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white) ![Build](https://img.shields.io/badge/build-passing-brightgreen)

# Tournament2000 (CLI)

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/screen/mainTitle.png">

Une application en ligne de commande pour gérer un tournoi sportif entièrement paramétrable.

---

## Sommaire

+ **[Fonctionnalités](#fonctionnalites)** => Toutes les fonctionnalités disponibles du programme.
+ **[Installation](#installation)** => Comment l'installer et l'exécuter.
+ **[Lancement](#lancement)**
   + *[Settings](#settings)* => Réglage des settings.
   + *[Players](#players)* => Enregistrement des players.
   + *[Tournament](#tournament)* => Lancement du tournoi.
      - *[Teams](#teams)* => Gestion des teams.
      - *[Pools](#pools)* => Gestion des matchs de pools.
      - *[Matchs](#matchs)* => Gestion des matchs.
      - *[Export](#export)* => Gestion des exportations.
      - *[Import](#import)* => Gestion des importations.
      - *[Show](#show)* => Gestion de l'affichage des différentes phases et éléments.
+ **[Évolutions futures](#évolutions-futures)** => La suite a implementer.
+ **[Bug](#bug)** => En cas de bug.

---

## Fonctionnalités

+ Choix du type de rencontres (simple / duo)
+ Choix de la mixité des rencontres (unigenre / mixte)
+ Paramétrage fin des rencontres en phase de poules
+ Paramétrage fin des rencontres en phases intermédiaires
+ Paramétrage de la finale
+ Paramétrage de la petite finale
+ Import / export de listes de joueurs
+ Affichage et exportation des résultats
+ Système de disqualification d'une équipe

*[remonter](#sommaire)*

---

## Installation

1. Cloner le dépôt :
   ```bash
   git clone git@github.com:Nicostrong/Tournament2000_CLI.git
   ```
2. Rentrer dans le dossier cloné :
   ```bash
   cd Tournament2000_CLI
   ```
3. Exécuter le Makefile :
   ```bash
   make
   ```
4. Après la compilation du programme, exécutez-le :
   ```bash
   ./Tournament2000
   ```

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/videos/compiling.gif">

*[remonter](#sommaire)*

---

## Lancement

> [!CAUTION] Lorsque vous lancez le programme, vous êtes invité à paramétrer les settings du tournoi.

> [!IMPORTANT] Les settings sont immuables, une synthèse vous est présentée à la fin de la prise de settings pour validation. Une fois validés, il est impossible de modifier les settings.

Voici les étapes à suivre pour lancer un tournoi :

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/mainMenu.png">

*[remonter](#sommaire)*

### Settings

Lorsque vous lancez le programme, vous êtes invité à paramétrer les settings. Voici les différents paramètres à renseigner pendant cette étape :

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/settingsMenu.png">

Et lorsque vous aurez paramétré les settings sans erreurs, le programme vous affichera un résumé des settings. Si vous voulez faire des modifications, il suffit de dire "Non", sinon vous passerez à l'étape suivante.

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/screen/settingsSummary.png">

*[remonter](#sommaire)*

### Players

Voici l'étape où vous enregistrez les players pour participer au tournoi. Le nombre de players ainsi que leur genre sont définis dans les settings.

Le menu Players est dynamique, c'est-à-dire qu'il n'affichera que les actions possibles à l'instant T.

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/playersMenu.png">

Le menu **Add** vous permet d'enregistrer un player manuellement.
Les menus **Modify** et **Delete** vous affichent la liste des players déjà inscrits et vous demandent de renseigner l'id du player que vous voulez modifier/supprimer.
Les menus **Import** et **Export** vous demandent le chemin du fichier à lire ou à créer.

> [!NOTE] Pensez à exporter vos participants, il vous suffira seulement de les importer pour gagner du temps.

Le menu **Show** vous permet d'afficher un ou tous les players déjà enregistrés.

> [!NOTE] Le programme gère automatiquement les imports multiples en respectant les paramètres des settings.

> [!NOTE] L'importation n'ajoutera pas plus de players que défini dans les settings.

> [!NOTE] L'importation respecte les settings aussi bien pour le nombre de players que pour le genre.

*[remonter](#sommaire)*

### Tournament

Enfin vous voici au cœur du programme : le *tournoi*. Le menu **Tournament** paraît assez grand et complexe, mais en réalité, il est dynamique, donc il évoluera au fil du déroulement du tournoi.

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/tournamentMenu.png">

#### Teams

Après avoir renseigné l'id de la team avec laquelle vous voulez interagir, vous aurez ce menu qui apparaîtra :

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/teamsMenu.png">

Dans le menu **Teams** vous pourrez renommer le nom par défaut de la team, modifier un membre ou la disqualifier. 

*[remonter au menu Tournament](#tournament)*
*[remonter](#sommaire)*

#### Pools

Après avoir renseigné l'id de la pool avec laquelle vous voulez interagir, le programme vous affiche un tableau avec tous les matchs qui doivent être joués.
 
<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/screen/pools.png">

#### Matchs

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/screen/matches.png">

*ici, les matchs de la pool 1* 

Après avoir sélectionné l'id du match pour lequel vous voulez interagir, vous avez ce menu qui vous permet de renseigner le score du match ou de le modifier en cas d'erreur.

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/matchMenu.png">

> [!note] Dans toutes les phases du tournoi, il y a la même logique de prise de score.

*[remonter au menu Tournament](#tournament)*
*[remonter](#sommaire)*

### Export

Un programme de gestion de tournoi sans possibilité d'exporter les différentes données n'est pas vraiment un programme de gestion de tournoi.

Dans ce menu vous aurez la possibilité d'exporter les différentes données sous différents formats (csv, txt).

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/exportMenu.png">

> [!NOTE] Pour chaque menu, on vous demandera le nom du fichier de sortie dans lequel les données seront sauvegardées.

> [!NOTE] De plus, ce menu est dynamique, donc vous ne pourrez exporter que ce qui est possible.

#### Show

J'ai ajouté ce menu afin de tester les différents visuels du programme et je l'ai laissé car il peut être intéressant pour une visualisation en cours de tournoi ou même à la fin.

<img src="https://raw.githubusercontent.com/Nicostrong/Tournament2000_CLI/main/docs/images/menu/showMenu.png">

> [!note] Ce menu est dynamique donc il vous affichera uniquement ce qui est possible d'être affiché.

*[remonter au menu](#menu)*
*[remonter](#sommaire)*

---

## Évolutions futures

> [!NOTE] Un programme vit et est en perpétuelle évolution, c'est pourquoi je listerai ici toutes les futures améliorations du programme.

+ Support multilingue
+ ...

*[remonter](#sommaire)*

---

## Bug

> [!note] La perfection n'existant pas, si vous rencontrez un problème, merci d'ouvrir une *[ISSUE](https://github.com/Nicostrong/Tournament2000_CLI/issues)*

> [!tip] Si vous avez des idées d'améliorations, ce sera avec plaisir de les implémenter pour rendre ce programme le plus polyvalent possible.

*[remonter](#sommaire)*
