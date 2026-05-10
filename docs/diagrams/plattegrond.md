# Plattegrond - aanpak en coördinaten

## Aanpak: PNG-achtergrond + 4 dynamische elementen

De originele OOPR2-opdracht levert een kant-en-klare plattegrond als
afbeelding (`Gebouw.png`, 500×400 px). We tekenen **niet** de muren met
de hand — we laden de PNG als achtergrond in `MainWindow::paintEvent` en
tekenen alleen de **bewegende dingen** eroverheen:

1. **vd** — schuifdeur (lijnsegment in het gat van de rechter buitenwand)
2. **d1** — draaideur (lijnsegment in het gat van de wand rechts van kamer 1)
3. **d2** — draaideur (lijnsegment in het gat van de wand links van kamer rechtsonder)
4. **s1** — halsensor (gele/blauwe cirkel naast vd, buiten het gebouw)

Voordeel: identiek aan de docent-afbeelding, geen risico op afwijking.

## PNG-locatie in het project

```
swadp_oopr2/
└── assets/
    └── Gebouw.png        ← 500×400 px, groene plattegrond
```

In de Qt-build wordt deze als resource (`.qrc`) ingebakken zodat het
executable hem altijd vindt — geen hardcoded pad zoals in de oude code.

## Venster-layout

```
Qt-venster (900 x 600):
┌──────────────────────────────────────────────────────────┐
│  ┌─────────────────────────────┐                          │
│  │                             │  [btn vd]                │
│  │     Gebouw.png (500x400)    │   ● s1 (halsensor)       │
│  │     geplaatst op (20, 20)   │  [btn d1]                │
│  │                             │  [btn d2]                │
│  │     deur-segmenten +        │  [btn halsensor]         │
│  │     sensor-cirkel hier      │                          │
│  │     overheen getekend       │                          │
│  └─────────────────────────────┘                          │
└──────────────────────────────────────────────────────────┘
```

## Coördinaten van de 4 elementen (in venster-pixels)

> Schatting; we tunen tijdens Fase 3 visueel terwijl de app draait.
> Eén stap aanpassen → `.\deploy.ps1` → kijken op VNC → bijstellen.

| Element | Anker (x, y) | Lengte / Diameter | Oriëntatie wand | Notitie |
|---|---|---|---|---|
| **vd** (schuifdeur) | (500, 195) | 55 px (verticaal) | rechter buitenmuur | dicht = volle lengte, open = halve lengte |
| **d1** (draaideur)  | (200, 95)  | 35 px | wand rechts van kamer 1 (verticaal) | scharnier bovenkant, draait open naar rechts |
| **d2** (draaideur)  | (310, 260) | 30 px | wand links van rechtsonder kamer (verticaal) | scharnier bovenkant, draait open naar rechts |
| **s1** (halsensor)  | (520, 215) | 18 px diameter (cirkel) | --- | rechts naast vd-gat |

## Tekenregels per object (zoals docent voorschrijft)

### Schuifdeur (vd)
- **Dicht**: verticaal lijnsegment over volle lengte van het gat
- **Open**: verticaal lijnsegment over halve lengte (rest is "weggeschoven")
- Implementatie: `painter.drawLine(x, y, x, y + (isOpen ? lengte/2 : lengte))`

### Draaideur (d1, d2)
- **Dicht**: lijnsegment in lijn met de wand (verticaal, vult het gat)
- **Open**: lijnsegment haaks op de wand (horizontaal, draait 90°)
- Scharnier = vast ankerpunt; ander uiteinde verplaatst bij open/dicht.
- Implementatie:
  - Dicht: `painter.drawLine(scharnier, scharnier + (0, lengte))`
  - Open: `painter.drawLine(scharnier, scharnier + (lengte, 0))`

### HallSensor (s1)
- **Niet geactiveerd**: gele cirkel
- **Geactiveerd**: blauwe cirkel
- Implementatie: `painter.drawEllipse(x, y, diameter, diameter)` met juiste brush

## UI-knoppen (rechts van plattegrond)

| Label | Functie | Positie (geschat) |
|---|---|---|
| "vd" | klik = open/dicht schuifdeur | (560, 220) |
| "d1" | klik = open/dicht draaideur 1 | (560, 95)  |
| "d2" | klik = open/dicht draaideur 2 | (560, 260) |
| "halsensor" | klik = (de)activeer hallsensor | (560, 340) |

## Visuele referentie

| Toestand | Bron-afbeelding (in `Remedial/opdracht1/.../`) |
|---|---|
| Plattegrond zonder deuren | `opdracht1_1.png` |
| Deuren OPEN | `opdracht1_2.png` |
| Deuren DICHT | `opdracht1_3.png` |
| Tekenregels (schuifdeur dicht/open, draaideur dicht/open) | `opdracht1_6.png` |

Tijdens Fase 3 hebben we de Qt-app live → kunnen we coördinaten in
secondes corrigeren tot vd/d1/d2/s1 visueel exact op de gaten van de
PNG vallen.
