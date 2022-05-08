# Gear Game Engine

## Kurzbeschreibung

Diese Game-Engine ist dafür erstellt worden um 2D-Spiele zu erstellen, vor allem im Pixelart-Artstyle.

## ECS

ECS ist eine Art der Datenverwaltung. Es ist eine Alternative zur objektorientierten Programmierung mit Vererbung und verwendet stattdessen Komponenten. Dadurch wird die Programmierung dynamischer, denn so eine Komponente kann beliebig oft wiederverwendet werden. ECS steht für Entity Component System. Ein Entity stellt ein Objekt dar. Diesem können Komponenten hizugefügt werden. Eine Komponente sollte möglichst nur Daten enthalten und keine Logik/Funktionalität. Die Logik befindet sich in den Systemen, welche die Daten der Komponenten verwenden.

## Ressourcenmanager

Mit dem Resourcenmanager kann man verschiedenste Ressourcen verwalten. In unserem Fall sind das Sprites, Animationen, Farbpaletten und Schriftarten. Alle diese Resourcen können von Dateien geladen werden. Diese Dateien haben ein gewisses Binär-Format (.gear). Beim Laden der Resource wird eine Refernz auf die Resource in einer Hash-Map anhand des Dateipfades relativ zum Wurzelverzeichnis des Spieles gespeichert. Wird die Resource nun erneut gebraucht, wird sie nicht neu geladen und nocheinmal abgespeichert, sondern die bereits existierende Resource in der Liste verwendet. Es gibt auch eine **unload**-Funktion, welche alle Resourcen, die nicht mehr gebraucht werden, aus der Liste löscht und den dazugehörigen Speicher freigibt.

### Farbpaletten

Farbpaletten sind die einfachste Form von Resource. Sie enthält eine Liste von bis zu 255 Farben im RGBA-Format.

### Sprites

Sprites stellen eigentlich nichts anderes als ein Bild dar. Dabei enthält das Sprite selbst nicht die Farben, sondern diese werden aus der angehängten Farbpalette entnommen. Jeder Pixel ist 1 Byte groß und stellt den Index in der Farbpalette dar. Farbpaletten können zur Laufzeit des Programmes hinzugefügt und geändert werden. Die Breite und Höhe können maximal 65536 Pixel sein.

### Animationen

Animationen funktionieren ähnlich wie Sprites, nur dass hier mehrere Sprites hintereinander angezeigt werden. Animationen haben eine gewisse Bildrate, welche eine Gleitkommazahl ist und die anzuzeigenden Bilder pro Sekunde angibt. Die maxiale Anzahl an Bildern beträgt 65536.

### Schriftarten

Die Schriftarten der Engine sind im Vergleich zu anderen Schriftarten sehr einfach aufgebaut. Der Text geht immer von links nach rechts und enthält nur ASCII-Zeichen, was für die englische Sprache in Ordnung ist. Man kann die Anzahl an Pixel zwischen zwei Zeichen und zwischen zwei Zeilen angeben. Eine Schriftart kann aus bis zu 255 Fraben bestehen, von denen die ersten 4 dynamisch veränderbar sind. 

## GRCV

Mit dem GRCV (Gear Resource Converter) können andere Dateien in dieses Format konvertiert werden. Dafür muss eine Datei mit der Endung .grcv erstellt werden, welche die dafür notwendigen Daten enthält. Diese Dateien werden dann automatisch von GRCV erkannt und falls notwendig konvertiert. Dabei wird auch der Pfad in den Orndern berücksichtigt.

Jede GEAR-Datei hat an erster Stelle einen 7 Zeichen langen Code, der den Typ der Datei angibt. Folgende Codes sind möglich:
- **GEARPLT**: Farbpalette
- **GEARSPT**: Sprite
- **GEARANM**: Animation
- **GEARFNT**: Schriftart

**Beispiel:**

Wir haben zwei Ordner:
- **unconverted_assets**: Dieser Ornder enthält alle unkonvertierten Dateien
- **assets**: Dieser Ornder enthält die Dateien im richtigen Dateiformat (.gear) die vom Spiel verwendet werden.

Der **unconverted_assets**-Ordner enthält unter anderem folgende Dateien:
- unconverted_assets/palettes/default_palette.grcv
- unconverted_assets/palettes/default_palette.png

*default_palette.png* wird in *default_palette.grcv* als Abhängigkeit definiert.

```
grcv unconverted_assets assets
```
Nun wird dieser Befehl ausgeführt, um die Dateien zu konvertieren. Dabei wird die Datei *assets/palettes/default_palette.gear* erstellt.

Wird der Befehl erneut ausgeführt, so wird die Ausgabedatei nicht neu erstellt. Dies passiert erst sobald sich *default_palette.png* oder *default_palette.grcv* ändern.

## GEAR-Formate

### Farbpalette

| Typ | Offset(Bytes) | Größe(Bytes) | Name | Beschreibung
|---|---|---|---|---|
| CHAR[] | 0x0 | 7 | Code | Code zum identifizieren einer Farbpalette (muss den Wert **GEARPLT** haben) |
| UINT8 | 0x7 | 1 | Anzahl | Anzahl der Farben |
| UINT8[] | 0x8 | *Anzahl* * 4 | Daten | Enthält die 32-bit RGBA-Farben der Palette |

### Sprite

| Typ | Offset(Bytes) | Größe(Bytes) | Name | Beschreibung
|---|---|---|---|---|
| CHAR[] | 0x0 | 7 | Code | Code zum identifizieren eines Sprites (muss den Wert **GEARSPT** haben) |
| UINT16 | 0x7 | 2 | Breite | Breite des Sprites in Pixel |
| UINT16 | 0x9 | 2 | Höhe | Höhe des Sprites in Pixel |
| UINT8[] | 0xb | *Breite* * *Höhe* | Daten | Enthält die 8-bit Pixel des Sprites |

### Animation

| Typ | Offset(Bytes) | Größe(Bytes) | Name | Beschreibung
|---|---|---|---|---|
| CHAR[] | 0x0 | 7 | Code | Code zum identifizieren einer Animation (muss den Wert **GEARANM** haben) |
| UINT16 | 0x7 | 2 | Breite | Breite des Animation in Pixel |
| UINT16 | 0x9 | 2 | Höhe | Höhe des Animation in Pixel |
| UINT16 | 0xb | 2 | Anzahl | Anzahl der vorhandenen Bilder |
| UINT16 | 0xd | 1 | Typ | Typ der Animation (0 = LOOP, 1 = FORWARD, 2 = PING_PONG) |
| FLOAT | 0xe | 4 | Bildrate | Bildrate der Animation in Bilder pro Sekunde |
| UINT8[] | 0x12 | *Breite* * *Höhe* * *Anzahl* | Daten | Enthält die 8-bit Pixel der Animation |

### Schriftart

| Typ | Offset(Bytes) | Größe(Bytes) | Name | Beschreibung
|---|---|---|---|---|
| CHAR[] | 0x0 | 7 | Code | Code zum identifizieren einer Schriftart (muss den Wert **GEARFNT** haben) |
| UINT8 | 0x7 | 1 | Flags | Wird aktuell noch nicht benutzt(in Zukunft für Eigenschaften wie zum Beispiel Blockschrift geplant) |
| UINT8 | 0x8 | 1 | Farbanzahl | Anzahl der verwendeten Farben der Schriftart |
| UINT8 | 0x9 | 1 | BitsProPixel | Anzahl der Bits die pro Pixel verwendet werden (kann entweder 1, 2, 4 oder 8 sein)) |
| INT16 | 0xa | 2 | Zeichenabstand | Abstand in Pixel zwischen zwei Zeichen |
| INT16 | 0xc | 2 | Zeilenabstand | Abstand in Pixel zwischen zwei Zeilen |
| INT16 | 0xe | 2 | Zeilenabstand | Abstand in Pixel zwischen zwei Zeilen |
| UINT16 | 0x10 | 2 | Breite | Breite der Zeichenreihe in Pixel |
| UINT16 | 0x12 | 2 | Höhe | Höhe der Zeichenreihe in Pixel |
| UINT32 | 0x14 | 4 | Datengröße | Anzahl an Bytes in der Zeichenreihe |
| UINT8 | 0x18 | 1 | Zeichenanzahl | Anzahl der Zeichen in der Zeichenreihe |
| UINT8 | 0x19 | 1 | Farbanzahl | Anzahl der Farben in der Schriftart |
| {CHAR, UINT16}[] | 0x1a | *Zeichenanzahl* * 3 | ZeichenDaten | Eine Liste von Zeichen mit der dazugehörigen Breite in Pixel |
| UINT8[4][] | 0x1a + *Zeichenanzahl* * 3 | *Farbanzahl* * 4 | FarbdatenDaten | Die Liste von 32-bit RGBA-Farben |
| UINT8[] | 0x1a + *Zeichenanzahl* * 3 + *Farbanzahl* * 4 | *Datengröße* | Daten | Enthält die Daten der Zeichenreihe |