# Mini-Minuteur Pomodoro

Un minuteur Pomodoro compact doté d'un boîtier imprimé en 3D pour booster ta productivité.
un disposif pensé au élèves pour éviter de tomber dans la procrastination en utilisant un smartphone comme miuteur.
##  Fonctionnement

1. Démarage : l'écran affiche POMODORO.
2. Appuie sur le bouton correspondant à CONCENTRATION : le buzzer émet un bip et le compte à rebours de 25 minutes démarre.
3. À 00:00, le buzzer sonne trois fois.
4. Appuie sur le bouton correspondant à la PAUSE : le buzzer signale le début de la pause de 5 minutes.
5. À la fin de la pause, le buzzer sonne à nouveau trois fois.
Tu peux appuyer sur l'un ou l'autre bouton pour démarrer son compte à rebours, même si un autre est déjà en cours. Pour respecter le cycle Pomodoro, appuie sur PAUSE lorsque la concentration est terminée.
## 📸 Aperçu
<img width="810" height="1080" alt="WhatsApp Image 2026-10-11 at 11 11 48 PM" src="https://github.com/user-attachments/assets/b433bc03-7fea-4481-884a-2542a3cc070d" />

<img width="810" height="1080" alt="WhatsApp Image 2026-10-11 at 11 13 18 PM" src="https://github.com/user-attachments/assets/673e5ab2-3a53-4d23-a808-9a0c82104ebb" />

## ✨ Fonctionnalités
- Décompte des sessions Pomodoro (25 min de travail / 5 min de pause).
- Interface simple (boutons pour choisir la pause ou le Travail, écran OLED).
- Boîtier ergonomique et design imprimé en 3D.

## 🛠️ Composants utilisés
- **Microcontrôleur :** Esp32 c3 supermini.
- **Affichage :** Écran OLED I2C 0.96"
- **Boîtier :** Imprimé en 3D (Fichiers disponibles dans "3D du boitier"
- **Alimentation :** Batterie Lithium / USB-C

## 📐 Schéma de câblage
Composant	Broche ESP32-C3 Super Mini
OLED SDA	GPIO 8
OLED SCL	GPIO 9
Bouton CONCENTRATION	GPIO 3
Bouton PAUSE	GPIO 4
Buzzer (+)	GPIO 5
OLED GND	GND
OLED VCC	3V3
Boutons, deuxième borne	GND
Buzzer (-)	GND
Les boutons n'ont pas besoin de résistances externes : le programme utilise les résistances internes INPUT_PULLUP.

## 🚀 Installation et Utilisation
1. Clone le dépôt :
   ```bash
   git clone [https://github.com/Gbte/Minuteur_Pomodoro.git](https://github.com/Gbte/Minuteur_Pomodoro.git)
