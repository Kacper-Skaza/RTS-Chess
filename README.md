# RTS Chess

## 🔧 Building an Executable
This will compile both the client and server executables and place them in the `release/` folder.
* On *Ubuntu* or *Debian*:
```sh
sudo apt-get update
sudo apt-get install -y build-essential libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev

chmod +x ./build.sh
./build.sh
```
* On *Windows 10* or *Windows 11*:
```bash
.\build.bat
```

## Other
* Gracz łączy się do serwera i wysyła swój nick (nick jest dekoratywny).
* Serwer odbiera informacje i odsyła przypisane mu ID.
* Gracz trafia do lobby, w którym widzi listę pokoi, stany gier i ilość graczy w pokojach.
* Gracz ma wybór stworzenia nowego pokoju lub dołączenia do istniejącego już pokoju. Gracz z pokoju może wrócić do lobby.
* Jeżeli gra trwa, dołączenie do pokoju umożliwia jedynie obserwację meczu oraz udział w czacie. Jeżeli gra nie została rozpoczęta, to gracz ma wybór zostania obserwatorem lub graczem w jednej z drużyn.
* Gra rozpoczyna się, kiedy każdy z graczy będzie gotowy i drużyny będą zapełnione (***rozmiary drużyn wybierane są podczas tworzenia pokoju***, obserwator nie jest graczem).
* Po rozpoczęciu gry każdy z graczy może ruszyć każdą ***swoją*** figurą na szachownicy (która nie jest zamrożona). Po wykonaniu ruchu figura zostaje zamrożona na pewien czas. W międzyczasie gracze mogą ruszać pozostałymi figurami. Gra rozgrywa się zgodnie z zasadami szachów.
* Gra kończy się, gdy któryś z króli zostanie zbity.
* Po zakończeniu gry pokój przechodzi do stanu, w którym gra się nie rozpoczęła, gracze i obserwatorzy pozostają w pokoju.

## TODO

* Shared
	* ~~move.cpp <- Kacpers100~~
		* ~~Pola skad dokad jaka figura~~
		* ~~Setter i gettery na wszystkie pola~~
	* ~~Logika gry~~
		* ~~Kazda `pieces (nazwa folderu)` z walidacja + ruchy i bicia~~
			* ~~`bool validateMove(class &move);`~~
			* ~~const swojej literki i getter~~
				1. ~~Knight.cpp~~
				2. ~~Rook.cpp~~
				3. ~~King.cpp~~
				4. ~~Queen.cpp~~
				5. ~~Bishop.cpp~~
				6. ~~Pawn.cpp~~
				7. ~~Piece.cpp <- destrojer~~
					* ~~ID dodac (pole startowe)~~
					* ~~firstMove -> moveCount (int)~~
		* ~~board.cpp <- Kacpers100~~
			* ~~Pozycje figur, klasy figur, tablica null || figura~~
	* ~~room.cpp <- destrojer~~
	* ~~User <- destrojer~~
		* ~~wskaznik do connectionmanager~~
	* ConnectionManager (kontruktor bierze fd) <- Kacpers100
		* Pola
			* kolejke przychodzacych wiadomosci
			* kolejke wychodzacych wiadomosci
			* trzyma wskazniki polaczen
			* kiedy byl ostatni ping
		* Funkcje
			* liczy czas (wlacza liczenie czasu od last ping)
			* zamkniecie polaczenia

			* wysyla wiadomosci
			* wysyla ping

			* odbiera wiadomosci
			* odbiera ping

* Client
	* Main Loop <- destrojer (2 views functionality done)
	* poll <- destrojer
	* GUI - views folder
		* Connect <- destrojer (initially done)
		* RoomSelect / RoomList <- Kacpers100
		* Lobby <- Kacpers100
		* ~~Game <- destrojer~~
		* Opcjonalne:
			* Menu
			* Settings
			* Credits
	* Tekstury
		* ~~Bindowanie czcionek <- destrojer~~
		* ~~Bindowanie tesktur <- Kacpers100~~
		* Pliki <- destrojer (temporary files done)
* Server
	* Logika połączeń
		* Zestawienie połączenia
			* Nowe połączenie tworzy klase
		* Odbieranie danych
		* Obsługa danych
		* Odsyłanie danych
		* Zamknięcie połączenia
	* poll z obsluga calosci <- Kacpers100
	* nasluchujemy i przy nowym polaczeniu tworzymy nowy connectionmanager <- Kacpers100

* client -> server dane
	* utworz pokoj
	* dolacz do pokoju
	* wiadomosc jestem gotowy
	* wiadomosc chce byc graczem
	* wiadomosc nie chce byc graczem
	* chce wyslac wiadkomosc na chat
	* chce wykonac ruch X
	* chce sprawdzic czy dalej jest polaczenie z serwerem (ping)

* server -> client dane
	* wysylam rozne ACK
	* wysylam liste pokoi (po kazdej aktualizacji -> broadcast, na start -> do jednego)
	* wysylam stan pokoju
	* wysyla startowe boarda
	* wysylam update boarda (po kazdym ruchu dla kazdego w  pokoju)
	* wysylam odebrane wiadomosci na chat do kazdego (po kazdej wiadkomosci)
	* wysylam wiadomosc kto wygral (do kazdego w pokoju)
	* wysylam nowy stan lobby po grze (wszyscy laduja w obserwatorach)
	* wysylam informacje ze gracz sie zgubil w odmentach internetu

* standaryzacja przesylow
	* przesylam nick
		* SEND
			* type: REQUEST_NICK
			* data: `std::string` nick
		* RECEIVE
			* type: ACK_REQUEST_NICK
			* data: `unsigned int` id
	* utworz pokoj
		* SEND
			* type: ROOM_CREATE
			* data: `std::string` room_name
		* RECEIVE
			* type: ACK_ROOM_CREATE
			* data: `null`
		* RECEIVE
			* type: ERR_ROOM_CREATE
			* data: `std::string` reason
	* lista pokoi
		* SEND
			* type: REQUEST_ROOMS
			* data: `null`
		* RECEIVE
			* type: ACK_REQUEST_ROOMS
			* data: `vector<Room>` rooms
	* dolacz do pokoju
		* SEND
			* type: ROOM_JOIN
			* data: `std::string` room_name
		* RECEIVE
			* type: ACK_ROOM_JOIN
			* data: `Room` room
		* RECEIVE
			* type: ERR_ROOM_JOIN
			* data: `std::string` reason
	* wyjdz z pokoju
		* SEND
			* type: ROOM_LEAVE
			* data: `std::string` room_name
		* RECEIVE
			* type: ACK_ROOM_LEAVE
			* data: `Room` room
	* wiadomosc jestem gotowy
		* SEND
			* type: PLAYER_READY
			* data: `null`
		* RECEIVE
			* type: ACK_PLAYER_READY
			* data: `null`
	* wiadomosc chce/nie chce byc graczem
		* SEND
			* type: PLAYER_WANT
			* data: `bool` true/false
		* RECEIVE
			* type: ACK_PLAYER_WANT
			* data: `null`
		* RECEIVE
			* type: ERR_PLAYER_WANT
			* data: `std::string` reason
	* chce wyslac wiadkomosc na chat
		* SEND
			* type: CHAT_MESSAGE
			* data: `std::string` wiadomosc
		* RECEIVE
			* type: ACK_CHAT_MESSAGE
			* data: `null`
	* chce wykonac ruch X
		* SEND
			* type: MAKE_MOVE
			* data: `Move` move
		* RECEIVE
			* type: ACK_MAKE_MOVE
			* data: `null`
		* RECEIVE
			* type: ERR_MAKE_MOVE
			* data: `Board` board
	* chce sprawdzic czy dalej jest polaczenie z serwerem (ping) (`connectionManager` robi to)
		* SEND
			* type: PING
			* data: `null`
		* RECEIVE
			* type: ACK_PING
			* data: `null`
	* wysylam liste pokoi (po kazdej aktualizacji -> broadcast, na start -> do jednego)
		* SEND
			* type: UPDATE_ROOMS
			* data: `vector<Room>` rooms
		* RECEIVE
			* type: ACK_UPDATE_ROOMS
			* data: `null`
	* wysylam stan pokoju -> jedno wyzej
	* wysylam update boarda (po kazdym ruchu dla kazdego w  pokoju)
		* SEND
			* type: MOVE_MADE
			* data: `Move` move
		* RECEIVE
			* type: ACK_MOVE_MADE
			* data: `null`
		* RECEIVE
			* type: ERR_MOVE_MADE
			* data: `null`
		* SEND
			* type: ACK_ERR_MOVE_MADE
			* data: `Board` board
	* wysylam odebrane wiadomosci na chat do kazdego (po kazdej wiadkomosci)
		* SEND
			* type: UPDATE_CHAT
			* data: `std::string` message, `User` user
		* RECEIVE
			* type: ACK_UPDATE_CHAT
			* data: `null`
	* wysylam wiadomosc kto wygral (do kazdego w pokoju)
		* SEND
			* type: GAME_FINALE
			* data: `MatchEndReasons` endReason
		* RECEIVE
			* type: ACK_GAME_FINALE
			* data: `null`
	* wysylam nowy stan lobby po grze (wszyscy laduja w obserwatorach) -> wysylam liste pokoi

	* wysylam informacje ze gracz sie zgubil w odmentach internetu -> wysylam wiadomosc kto wygral

* Przesylane uzywamy smiesznej biblioteki to jSONowo <- kazdy swoje
	* ~~piece(figury)~~
	* board
	* room
	* user
	* move

## Nazewnictwo

* -Wall -Wextra -O2
* Biale figury dostaja wilekie litery
* Czarne figury dostaja male litery
* Używamy znaczków
* `isThatFun()`
* `int isThatFun = 0;`
* `const int MAX_VALUE = 0;`
* `#define MAX_VALUE 0`
* `class IsThatFun` \n `{}` <- Klasy z wielkiej litery
* `using std::string`
* `using namespace std;` <- ale nie globalnie
* `const std::string &name;` <- brak wskaznikow
* `int &var;`
* `Move.hpp`
```C++
#ifndef MOVE_HPP
#define MOVE_HPP
#include <utility>

class Move
{
private:
	char piece;
	std::pair<int, int> from;
	std::pair<int, int> to;

public:
	// Konstruktor
	Move(char piece, std::pair<int, int> from, std::pair<int, int> to);

	// Gettery
	char getPiece() const noexcept;
	std::pair<int, int> getFrom() const noexcept;
	std::pair<int, int> getTo() const noexcept;
};

#endif // MOVE_HPP
```
* Move.hpp
```C++
#include "../headers/move.hpp"

// Konstruktor
Move::Move(char piece, std::pair<int, int> from, std::pair<int, int> to)
	: piece(piece), from(from), to(to) {}

// Gettery
char Move::getPiece() const noexcept
{
	return piece;
}
```
