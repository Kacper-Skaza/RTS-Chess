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