# Gear Game Engine

## Kurzbeschreibung

Diese Game-Engine ist dafür erstellt worden um 2D-Spiele zu erstellen, vor allem im Pixelart-Artstyle.

## ECS

ECS ist eine Art der Datenverwaltung. Es ist eine Alternative zur objektorientierten Programmierung mit Vererbung und verwendet stattdessen Komponenten. Dadurch wird die Programmierung dynamischer, denn so eine Komponente kann beliebig oft wiederverwendet werden. ECS steht für Entity Component System. Ein Entity stellt ein Objekt dar. Diesem können Komponenten hizugefügt werden. Eine Komponente sollte möglichst nur Daten enthalten und keine Logik/Funktionalität. Die Logik befindet sich in den Systemen, welche die Daten der Komponenten verwenden.

## Ressourcenmanager

Mit dem Resourcenmanager kann man verschiedenste Ressourcen verwalten. In unserem Fall sind das Sprites, Animationen, Farbpaletten und Schriftarten. Alle diese Resourcen können von Dateien geladen werden. Diese Dateien haben ein gewisses Binär-Format (.gear). Beim Laden der Resource wird eine Refernz auf die Resource in einer Hash-Map anhand des Dateipfades relativ zum Wurzelverzeichnis des Spieles gespeichert. Wird die Resource nun erneut gebraucht, wird sie nicht neu geladen und nocheinmal abgespeichert, sondern die bereits existierende Resource in der Liste verwendet. Es gibt auch eine **unload**-Funktion, welche alle Resourcen, die nicht mehr gebraucht werden, aus der Liste löscht und den dazugehörigen Speicher freigibt.

### GRCV

Mit dem GRCV (Gear Resource Converter) können andere Dateien in dieses Format konvertiert werden. Dafür muss eine Datei mit der Endung .grcv erstellt werden, welche die dafür notwendigen Daten enthält. Diese Dateien werden dann automatisch von GRCV erkannt und falls notwendig konvertiert. Dabei wird auch der Pfad in den Orndern berücksichtigt.

Beispiel:

Wir haben zwei Ordner:
- **unconverted_assets**: Dieser Ornder enthält alle unkonvertierten Dateien
- **assets**: Dieser Ornder enthält die Dateien im richtigen Dateiformat (.gear) die vom Spiel verwendet werden.

Der **unconverted_assets**-Ordner enthält unter anderem folgende Dateien:
- unconverted_assets/default_palette.grcv
- unconverted_assets/default_palette.png

```
grcv unconverted_assets assets
```

