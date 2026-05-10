# SWADP + OOPR2 Remedial — GebouwBeheer Firma L&B

> Kandidaat-portfolio voor twee assessments:
> - **OOPR2 (Remedial C++ Programmeren)** — werkende Qt-app op Raspberry Pi met fysieke hardware (servomotoren, schakelaars, LED, halsensor).
> - **SWADP (Software Architecture & Design Patterns)** — herontwerp met Layering / MVC / SOLID en design patterns; klassendiagram als deliverable.

Beide assessments delen dezelfde codebase. Zie `docs/verslag.md` voor de inhoudelijke onderbouwing.

## Architectuur in één blik

```
┌─────────────────────────────────────┐
│  Presentation   (Qt UI)              │  src/ui/
├─────────────────────────────────────┤
│  Application    (use-cases)          │  src/app/
├─────────────────────────────────────┤
│  Domain         (Deur, Slot, ...)    │  src/domain/
├─────────────────────────────────────┤
│  Infrastructure (IGpio, Servo, ...)  │  src/infra/
└─────────────────────────────────────┘
```

## Hardware-pins (BCM-nummering)

| Functie | BCM | Fysieke pin |
|---|---|---|
| Servo schuifdeur (vd) | 18 | 12 |
| Servo draaideur d1 | 23 | 16 |
| Servo draaideur d2 | 24 | 18 |
| Knop schuifdeur | 17 | 11 |
| Knop draaideur d1 | 27 | 13 |
| Knop draaideur d2 | 22 | 15 |
| Knop halsensor | 5 | 29 |
| LED rood (exception) | 25 | 22 |

Servo-voeding extern (5V), Pi-GND aan voedings-GND aansluiten.

## Bouwen op de Pi

```bash
cmake -B build -S .
cmake --build build -j$(nproc)
./build/gebouw
```

## Deployen vanuit Windows

```powershell
.\deploy.ps1 "korte commit-boodschap"
```

Het script committed, pushed naar GitHub en SSH't naar de Pi om daar `git pull` + `cmake --build` te draaien. Build-output verschijnt direct op je terminal.

## Doxygen-documentatie genereren

Op de Pi:
```bash
doxygen Doxyfile
xdg-open docs/doxygen/html/index.html
```

## Branch-strategie

- `main` — alleen werkende code, gemerge'd vanaf feature-branches.
- `feat/opdracht-N-naam` — werk per opdracht, terug-merge na voltooiing.
- `feat/swadp-architectuur` — refactoring naar lagen + patterns.
