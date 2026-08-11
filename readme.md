🇫🇷 **Français** | [🇬🇧 Read in English](readme.en.md)

# Tournament2000 (CLI)

<img src="./docs/images/title.png" width="400" height="300">

Une application en ligne de commande pour gerer les tournois de bad au sein d un club.

---

## Sommaire

+ **[Fonctionnalites](#fonctionnalites)** => toutes les fonctionnalites disponibles du programme.
+ **[Installation](#installation)** => Comment l installer et l executer.
+ **[Menus](#menus)** => arbre des menus.
   + *[Menu players](#players)*
   + *[Menu Tournament](#tournament)*
   + *[Menu Export](#export)*
+ **[Parametrage](#parametrage)** => toute la puissance de la personnalisation.
+ **[Joueurs](#joueurs)** => import / export des joueurs.
+ **[Avancement](#avancement)** => Avancement du projet.
+ **[Bug](#bug)** => En cas de bug.

---

## Fonctionnalites

+ Personnalisation complete du tournoi
+ Import / export de listes de joueurs
+ Affichage et exportations des resultats
+ ...
[remonter](#sommaire)

---

## Installation

1. Cloner le depot:
   ```bash
   git clone git@github.com:Nicostrong/Tournament2000_CLI.git
   ```
2. Rentrer dans le dossier clone:
   ```bash
   cd Tournament2000_CLI
   ```
3. Executer le Makefile:
   ```bash
   make
   ```
4. Apres la compilation du programme, executer-le:
   ```bash
   ./Tournament2000
   ```

<img src="./docs/animations/compiling.gif" width="800" height="100">

*[remonter](#sommaire)*

---

## Menu

> [!CAUTION] Vous devez imperativement definir les settings avant de lancer un tournoi.

> [!IMPORTANT] Certains menus sont accessibles selon les reglages definis dans les settings.

*[remonter](#sommaire)*

### Menu principal

Lorsque vous lancez le programme, vous avez :

<p><img src="./docs/images/menu/MainMenuInitial.png" width="200" height="100"></p>

Et lorsque vous aurez parametre les settings sans erreurs et que le nombre de participants est suffisant, vous aurez :

<p><img src="./docs/images/menu/MainMenu.png" width="200" height="100"></p>

*[remonter au menu](#menu)*
*[remonter](#sommaire)*

### Players

C est dans le menu ***players*** que se fait la gestion complete des participants.
Vous pouvez les saisir a la volee ou les importer depuis un fichier csv.

> [!TIP] Pensez a exporter vos participants, il vous suffira seulement de les importer pour gagner du temps.

<p><img src="./docs/images/menu/PlayersMenu.png" width="200" height="200"></p>

*[remonter au menu](#menu)*
*[remonter](#sommaire)*

### Tournament

Ce menu s active des lors que les settings sont corrects et que le nombre de participants est suffisant pour lancer un tournoi.

<p><img src="./docs/images/menu/TournamentMenu.png" width="200" height="200"></p>

Ce menu contient lui aussi des menus qui seront actifs au fur et a mesure du deroulement du tournoi.

Le menu ***Team*** vous permet d afficher les differentes equipes.
Le menu ***Pools*** gere les rencontres des matchs de poules.
Le menu ***1***/16 gere les matchs de 1/16 (visibles sous conditions)
Le menu ***1***/8 gere les matchs de 1/8 (visibles sous conditions)
Le menu ***1***/4 gere les matchs de 1/4 (visibles sous conditions)
Le menu ***1***/2 gere les matchs de 1/2 (visibles sous conditions)
Le menu ***Final*** gere le match de finale (visible sous conditions)
Le menu ***3rd*** place gere le match pour la troisieme place (visible sous conditions)
Le menu ***Export*** gere toutes les exportations possibles.

Pour les differentes phases du tournoi, chaque menu a le meme sous-menu qui permet de gerer la phase en cours.

<p><img src="./docs/images/menu/StageMenu.png" width="200" height="200"></p>

*[remonter au menu](#menu)*
*[remonter](#sommaire)*

### Export

> [!NOTE] Ce menu vous permet d exporter la quasi-totalite des donnees liees au tournoi.

<p><img src="./docs/images/menu/ExportMenu.png" width="200" height="200"></p>

### Arboressance complete

<p><img src="./docs/images/menu/Menu.png" width="200" height="200"></p>

*[remonter au menu](#menu)*
*[remonter](#sommaire)*

---

## Parametrage

Voici la liste des settings a configurer imperativement:

> [!WARNING] Attention il y a un checker qui verifie la logique des settings, si vous voulez un tournoi de 12 joueurs, il en va de soi que le tournoi ne peut pas etre en double.

### Configuration de tournoi

+ Intitule du tournoi

#### Parametrage des joueurs

+ Nombre total de participant [12:92]
+ Tournoi en double [o/n]
+ Tournoi mixte [o/n]

> [!TIP] Si vous n etes pas certain d avoir le nombre suffisant de participants cette option permet a des joueurs de jouer dans plusieurs equipes.

+ Joueur multi-team [o/n] 

### Parametrage des pools

+ Nombre de pool[4/8/16]
+ Nombre de joueur / equipes par pool [3:8]

### Parametrage des matchs

+ Nombre de terrain disponible [1:12]
+ Score pour gagner un set [5:30]
+ Score maxi pour gagner un set [5:30]
+ Ecart de points necessaire [1:5]

### Parametrage des phases

+ Nombre de set en pool [1:3]
+ Nombre de set en 1/16^eme^ [1:3]
+ Nombre de et en 8^eme^ [1:3]
+ Nombre de set en quarts [1:3]
+ Nombre de set en demis [1:3]
+ Nombre de set en final [1:5]
+ Match pour la 3^eme^ place [o/n]
+ Nombre de set en petite final [1:3]

*[remonter](#sommaire)*

---

## Avancement

- [x] definir les differentes classes du programme.
- [x] definir les differentes relations entre les classes.
- [x] implementer la logique metier d un tournoi.
- [x] implementer l import et l export des joueurs.
- [x] implementer les menus.
- [x] implementer les differents affichages des stages du tournoi.
- [x] implementer les differents exports des stages du tournoi.
- [ ] gerer l affichage contitionnel lie aux settings.
- [ ] gerer les afichages conditionnels lie a l avancement du tournoi.
- [ ] tester avec Valgrind.

*[remonter](#sommaire)*

---

## Bug

> [!CAUTION] La perfection n existant pas, si vous recontrer un probleme, merci d ouvrir une [PR](https://github.com/Nicostrong/Tournament2000_CLI/pulls)


---

# HELPER

> [!note]
> note

> [!warning]
> warning

> [!CAUTION]
> caution

> [!tip]
> tip

> [!IMPORTANT]
> important

