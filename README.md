# RTS Chess

* Gracz łączy się do serwera i wysyła swój nick (nick jest dekoratywny).
* Serwer odbiera informacje i odsyła przypisane mu ID.
* Gracz trafia do lobby, w którym widzi listę pokoi, stany gier i ilość graczy w pokojach.
* Gracz ma wybór stworzenia nowego pokoju lub dołączenia do istniejącego już pokoju. Gracz z pokoju może wrócić do lobby.
* Jeżeli gra trwa, dołączenie do pokoju umożliwia jedynie obserwację meczu oraz udział w czacie. Jeżeli gra nie została rozpoczęta, to gracz ma wybór zostania obserwatorem lub graczem w jednej z drużyn.
* Gra rozpoczyna się, kiedy każdy z graczy będzie gotowy i drużyny będą zapełnione (***rozmiary drużyn wybierane są podczas tworzenia pokoju***, obserwator nie jest graczem).
* Po rozpoczęciu gry każdy z graczy może ruszyć każdą ***swoją*** figurą na szachownicy (która nie jest zamrożona). Po wykonaniu ruchu figura zostaje zamrożona na pewien czas. W międzyczasie gracze mogą ruszać pozostałymi figurami. Gra rozgrywa się zgodnie z zasadami szachów.
* Gra kończy się, gdy któryś z króli zostanie zbity.
* Po zakończeniu gry pokój przechodzi do stanu, w którym gra się nie rozpoczęła, gracze i obserwatorzy pozostają w pokoju.

## Fazy

### Faza 1

* Shared
	* move.cpp <- Kacpers100
		* Pola skad dokad jaka figura
		* Setter i gettery na wszystkie pola
	* Logika gry
		* Kazda `pieces (nazwa folderu)` z walidacja + ruchy i bicia
			* `bool validateMove(class &move);`
			* const swojej literki i getter
			* Kacpers100
				1. Knight.cpp
				2. Rook.cpp
				3. King.cpp
			* destrojer
				1. Queen.cpp
				2. Bishop.cpp
				3. Pawn.cpp
		* board.cpp <- Kacpers100
			* Pozycje figur, klasy figur, tablica null || figura
	* room.cpp <- destrojer
	* Obsługa JSON
		* Poszukać biblioteki
		* readJSON.cpp
		* writeJSON.cpp
* Client
	* GUI - views folder
		* Connect <- destrojer
		* RoomSelect / RoomList <- Kacpers100
		* Lobby <- Kacpers100
		* Game <- destrojer
		* Opcjonalne:
			* Menu
			* Settings
			* Credits
	* Tekstury
		* Bindowanie czcionek <- destrojer
		* Bindowanie tesktur <- Kacpers100
		* Pliki <- destrojer
* Server
	* Logika połączeń
		* Zestawienie połączenia
			* Nowe połączenie tworzy klase
		* Odbieranie danych
		* Obsługa danych
		* Odsyłanie danych
		* Zamknięcie połączenia
	* Przerwania / Pętla zdarzeń

### Faza 2

* Shared
	* ...
* Client
	* Logika połączeń
		* Zestawienie połączenia
		* Wysyłanie danych
		* Oczekiwanie na dane
		* Odbieranie danych
	* Zamknięcie połączenia / Wyjście z gry
* Server
	* Logika połączeń
		* Obsługa danych

### Faza 3

* Testy na Linuksie

## Nazewnictwo

* -Wall -Wextra -O2
* Używamy znaczków
* `isThatFun()`
* `int isThatFun = 0;`
* `const int MAX_VALUE = 0;`
* `#define MAX_VALUE 0`
* `class isThatFun` \n `{}`
* `using std::string`
* `using namespace std;` <- ale nie globalnie
* `const std::string& name;` <- brak wskaznikow
* `int &var;`
* `player.hpp`
```C++
class player {
private:
    std::string name;
    int score;

public:
    // --- konstruktory ---
    player();
    player(const std::string& name, int score = 0);

    // --- gettery/settery ---
    std::string getName() const;
    int getScore() const;
    void setScore(int newScore);

    // --- metody ---
    void addScore(int points);
};
```
* imo.cpp
```C++
class imo{
  int var
  void fun {
    //do smthing
     return 0;
  }

}
```
