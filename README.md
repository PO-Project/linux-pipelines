# Linux Pipelines

## Kompilacja przykładu

```
cmake . ; cmake --build . --config Release
```

## Specyfikacja bindowanych komend

- `<EDITION>` (przyjmuje `KEY`)
  
  Obsługuje klawisze strzałek (nawigacja) i `<DEL>` (usuwanie wybranego obiektu)
- `:process<ENTER>`
  
  Tworzy nowy proces (prostokąt)
- `:file<ENTER>`
  
  Tworzy nowy plik (też prostokąt)
- `:pipe ${ARG}<ENTER>` (przyjmuje `ARG`)
  
  Zaczyna (gdy _ARG ∈ {1, 2, stdin, stderr}_) lub kończy (gdy _ARG = ""_) tworzenie pipe'a (strzałki)
- `:swap<ENTER>`
  
  Zaczyna i kończy zamianę dwóch procesów lub plików.
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
- `swap`
- `#vim#navigate!EDIT` _czekam na `<ENTER>`_
- `pipe1` _czekam na `${ARG}`_
- `pipe2` _czekam na `${ARG}`_

_Z tego, co wiem, nie ma jeszcze backendu, który parsowałby komendy z `${...}`, a ten przykładowy nie rozpoznaje `:` i `<ENTER>`._

(_swoją drogą, jak robimy informacje o błędach? przez jakieś `ERROR`, czy każdy po swojemu?_)

## Integracja z `vim-backend`

### Co działa:

- `process`, `remove`, `list`, `swap`

### Co nie działa

- nawigacja strzałkami `<LARROW>`, `<UARROW>`
- cokolwiek co wymaga nawigacji
- cokolwiek co przyjmuje argument

## TODO

- Integracja z pozostałymi backendami
- Testy?
- Dokumentacja kodu i tysiąc innych bajek do opowiadania przed snem...