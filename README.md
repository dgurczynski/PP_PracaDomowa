# PP_PracaDomowa

Zintegrowany silnik gier planszowych i generatora grafiki SVG, napisany obiektowo w C++

### Główne Funkcje
* **Gry:** Kółko i Krzyżyk (PvP/PvE z botem) oraz Warcaby z logiką bicia.
* **SVG:** Dynamiczne generowanie plików `.svg` ze stanu gier oraz rysowanie figur z detekcją kolizji.
* **Systemy:** Zapis/odczyt stanu gry (Save/Load) oraz licznik czasu na wykonanie ruchu.

### Podział Obiektowy
* `TicTacToe` & `Checkers` – silniki gier i walidacja ruchów.
* `SVGenerator` – obsługa plików XML/SVG i geometria obliczeniowa.
* `Menu` – zarządzanie interfejsem i logiką aplikacji.

### Obsługa
1. Wybierz opcję 1-5 z menu głównego.
2. Współrzędne podawaj w formacie: `1 2` (TTT) lub `B3-A4` (Warcaby).
