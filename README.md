# Sternsystem-Simulation

Eine Echtzeit-2D-Gravitationssimulation mit prozedural 
generierten Sternsystemen.

## Funktionen
- Newtonsche Gravitationssimulation (F = G × m1 × m2 / r²)
- Prozedurale Sternsystemgenerierung
- Orbitalmechanik mit erster kosmischer Geschwindigkeit
- Eigene ECS-Architektur (Sparse Set)
- Zoom-Unterstützung

## Technologien
- C++17
- SFML 3.0.2
- CMake 4.0+

## Build & Ausführung
git clone https://github.com/m-kobylianskyi/stern-system-simulation.git
cd stern-system-simulation
cmake -S source -B build
cmake --build build
cd source && ../build/Star

## Screenshots
<img width="2560" height="1277" alt="image" src="https://github.com/user-attachments/assets/2fa03c7a-ff34-4183-9df0-93ace7ddb4ea" />

## Architektur
Das Projekt verwendet ein eigenes Entity Component System (ECS) 
mit Sparse-Set-Implementierung für effiziente Komponentenspeicherung 
und -abfrage mit O(1)-Zugriffszeit.
