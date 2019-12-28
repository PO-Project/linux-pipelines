# Linux Pipelines

## Kompilacja przykładu

```
g++ -Wall -Wextra -pedantic main.cpp -o ex -lncurses -lpanel -lstdc++fs -std=c++17
```

_- Dlaczego nie `cmake`?_

_- Hmm, sam nie wiem._

## Specyfikacja bindowanych komend

- `<EDITION>` (przyjmuje `KEY`)
  
  Obsługuje klawisze strzałek (nawigacja) i `<DEL>` (usuwanie wybranego obiektu)
- `:process<ENTER>`
  
  Tworzy nowy proces (prostokąt)

- `:file<ENTER>`
  
  Tworzy nowy plik (też prostokąt)
- `:pipe ${ARG}<ENTER>` (przyjmuje `ARG`)
  
  Zaczyna (gdy _ARG ∈ {1, 2, stdin, stderr}_) lub kończy (gdy _ARG = ""_) tworzenie pipe'a (strzałki)
- `:edit ${ARG}<ENTER>` (przyjmuje `ARG`)
  
  Edytuje komendę wywołania procesu lub nazwy pliku (tekst w prostokącie)
- `:remove<ENTER>`
  
  Usuwa wybrany obiekt
- `:list<ENTER>`
  
  Wypisuje listę plików binarnych w `PATH` i plików w _cwd_
- `:save ${FILENAME}<ENTER>` (uwaga, aktualnie `${ARG}`)
  
  Zapisuje plik "projektu programu Linux Pipelines"
- `:open ${FILENAME}<ENTER>` (uwaga, aktualnie `${ARG}`)
  
  Otwiera plik "projektu programu Linux Pipelines"
- `:export ${SCRIPTNAME}<ENTER>` (uwaga, aktualnie `${ARG}`)
  
  Eksportuje plik "projektu programu Linux Pipelines" do bashowego skryptu 

_Uwaga, ta specyfikacja może się zmieniać._

## Co naprawdę jest aktualnie bindowane

- `<EDITION>`
- `process`
- `pipe` (przyjmuje `ARG`)
- `edit` (przyjmuje `ARG`)
- `remove`
- `list`
- `save` (przyjmuje `ARG`)
- `open` (przyjmuje `ARG`)
- `export` (przyjmuje `ARG`)

_Z tego, co wiem, nie ma jeszcze backendu, który parsowałby komendy z `${...}`, a ten przykładowy nie rozpoznaje `:` i `<ENTER>`._

(_swoją drogą, jak robimy informacje o błędach? przez jakieś `ERROR`, czy każdy po swojemu?_)

## TODO

- `:swap<ENTER>`
- Integracja z backendami
- Testy?
- Dokumentacja kodu i tysiąc innych bajek do opowiadania przed snem...