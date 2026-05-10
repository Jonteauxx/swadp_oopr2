# Architectuur-onderbouwing (SWADP)

Dit document is **dé** referentie voor je SWADP-mondeling. Per architecturele
keuze: wat, waarom, en welk SOLID-principe of pattern erbij hoort.

---

## 1. Layered Architecture (Head First SA, H6)

Vier lagen, dependencies wijzen **alleen naar beneden**:

```
Presentation  →  Application  →  Domain  ←  Infrastructure
                                   ↑
                              core regels
```

| Laag | Verantwoordelijkheid | Mag afhankelijk zijn van |
|---|---|---|
| Presentation | Qt UI: knoppen, painting, layout | Application |
| Application  | Use-cases: 'open de schuifdeur' | Domain, Infrastructure (via interface) |
| Domain       | Pure OO uit OOPR2 — Deur, Slot, IdKaart, ... | Niets behalve standaard C++ |
| Infrastructure | Hardware-toegang via `IGpio` | Domain (om interfaces te implementeren) |

**Waarom belangrijk voor het mondeling:** stel de docent vraagt
*"waarom heb je `Deur` niet direct van pigpio gebruik laten maken?"* —
dan is je antwoord: **dependency rule** — domeinklassen hangen niet af
van implementatie-details (zoals een specifieke GPIO-library), zodat ze
testbaar blijven en de hardware-keuze later kan veranderen zonder de
domeinlogica te raken.

---

## 2. SOLID — toegepast in dit project

### S — Single Responsibility Principle

Elke klasse heeft één reden om te veranderen.

| Klasse | Verantwoordelijkheid |
|---|---|
| `Deur` | weet hoe een deur zich gedraagt (open/dicht/sloten checken) |
| `Servo` | vertaalt "open"/"dicht" naar PWM-pulsen |
| `MainWindow` | tekent UI en routeert clicks |

**Niet** gesplitst (bewust): `Sensor` doet zowel dataopslag (`x`, `y`,
`geactiveerd`) als tekenen. Splitsen zou voor dit project
over-engineering zijn — het is één concept.

### O — Open/Closed Principle

Code is open voor uitbreiding, dicht voor wijziging.

**Voorbeeld:** in opdracht 4 voegen we `HerkenningsSlot` toe. We hoeven
**niets** in `Deur` aan te passen — `Deur` praat alleen tegen de
`Slot`-interface. Strategy pattern.

### L — Liskov Substitution Principle

Subklassen mogen overal vervangen worden waar de superklasse staat.

**Voorbeeld:** `vector<shared_ptr<Slot>>` werkt voor élk slot-type.
`Deur::open()` weet niet of het een `SleutelSlot` of een `KaartSlot` is.

### I — Interface Segregation Principle

Klanten dwingen niet te grote interfaces af.

**Voorbeeld:** `Afdrukker` heeft slechts twee methodes (`toonText`,
`clearMedium`). Niet 10 methodes voor een hypothetische printer.

### D — Dependency Inversion Principle

High-level code hangt niet af van low-level details, maar van abstracties.

**Voorbeeld:** `Servo` neemt een `IGpio&` aan, niet een `PigpioGpio`.
Daardoor kunnen unit-tests een `MockGpio` injecteren.

---

## 3. Design Patterns — minimaal 3 (refactoring.guru)

### Pattern 1 — Strategy

| | |
|---|---|
| **Categorie** | Behavioral |
| **Waar** | `Slot` (interface) + `SleutelSlot`, `CodeSlot`, `HerkenningsSlot`, `KaartSlot` |
| **Probleem** | Een deur kan op vele manieren beveiligd worden (sleutel, code, kaart). Een `if/else` of `switch` zou groeien bij elk nieuw type. |
| **Oplossing** | Definieer `Slot` als interface met `ontgrendel()`, en stop de variatie in concrete subklassen. `Deur` praat alleen met de interface. |
| **Voordelen** | Open/Closed afgedwongen, nieuwe slot-types toevoegen zonder bestaande code te raken. |
| **Trade-off** | Meer kleine klassen i.p.v. één grote — meer files, maar elk veel kleiner. |

### Pattern 2 — Facade

| | |
|---|---|
| **Categorie** | Structural |
| **Waar** | `IGpio` met implementaties `PigpioGpio` en `MockGpio` |
| **Probleem** | pigpio is een C-library met dozijnen functies (`gpioWrite`, `gpioServo`, `gpioSetMode`, ...). De rest van de codebase moet daar niet door geplaagd worden. |
| **Oplossing** | Eén nette interface `IGpio` met alleen de 5 calls die we nodig hebben. Implementatie verbergt complexiteit. |
| **Voordelen** | Dependency Inversion afgedwongen, mock-baar voor tests, hardware-library vervangbaar. |
| **Trade-off** | Eén extra abstractielaag — je verliest direct toegang tot pigpio-specifieke features (geen probleem voor dit project). |

### Pattern 3 — Observer (via Qt signals/slots)

| | |
|---|---|
| **Categorie** | Behavioral |
| **Waar** | `KnopFysiek` zendt signal `ingedrukt()`; `MainWindow` luistert. Ook `Sensor`-veranderingen kunnen zo. |
| **Probleem** | Hardware-knop weet niet wie geïnteresseerd is in zijn drukken. Tightly coupling = `KnopFysiek` referent zou houden naar specifieke deur. |
| **Oplossing** | Qt's signal/slot is een built-in observer. `KnopFysiek` zendt blind, geïnteresseerden connecten zelf. |
| **Voordelen** | Losse koppeling, meerdere listeners mogelijk, runtime-bindbaar. |
| **Trade-off** | Iets minder type-veiligheid (Qt SIGNAL/SLOT was string-based, modern functor-syntax is wel veilig). |

### Optioneel pattern 4 — Factory Method

`DeurFactory::maakSchuifdeur(...)` om constructor-explosie te voorkomen
en de hardware-koppeling (Servo + Sensor) op één plek te configureren.
Vermeld als bonus indien de docent doorvraagt.

---

## 4. Wat we BEWUST niet hebben toegepast

> Dit is goud voor een mondeling — laat zien dat je trade-offs maakt en
> niet alle patterns inzet 'omdat het kan'.

- **Singleton** — vermijden, behalve voor de statische `idKaarten`-map
  in `KaartSlot` (omdat de **opdracht** die voorschrijft).
- **Repository pattern** — geen persistentie nodig in dit project.
- **CQRS / Mediator** — overkill voor < 20 klassen.
- **Microservices** — gewoon nee.

---

## 5. Quality attributes (boek H4-H6)

| Attribuut | Hoe we het halen |
|---|---|
| **Testability** | `IGpio` mockbaar → domein 100% test-baar zonder Pi |
| **Maintainability** | Lagen + Strategy → nieuwe slot-types raken niets bestaands |
| **Portability** | `MockGpio` laat de app op Windows compileren voor IDE-feedback |
| **Modifiability** | Plattegrond-coördinaten in één file (`plattegrond.md`) |

---

## 6. Klassendiagrammen

| Diagram | Bestand | Doel |
|---|---|---|
| Domeinmodel (alleen OOPR2-klassen) | `diagrams/klassen-domain.puml` | Voor OOPR2-portfolio + assessment |
| Volledig systeem (incl. infra/app) | `diagrams/klassen-systeem.puml` | Voor SWADP-deliverable |
| Layered architecture | `diagrams/architectuur-lagen.puml` | Voor SWADP-uitleg |
| Plattegrond-coördinaten | `diagrams/plattegrond.md` | Single source of truth |

Renderen tot PNG:
```bash
sudo apt install plantuml   # eenmalig
plantuml docs/diagrams/*.puml
```
PNGs verschijnen naast de `.puml` bestanden.
