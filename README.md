# Linux Pipelines

## Kompilacja przykładu

```
g++ -Wall -Wextra -pedantic main.cpp -o ex -lncurses -lpanel -std=c++17
```

_- Dlaczego nie `cmake`?_

_- Hmm, sam nie wiem._

## Specyfikacja bindowanych komend

- `<EDITION>` (przyjmuje `KEY`)
  
  Obsługuje klawisze strzałek (nawigacja) i `<DEL>` (usuwanie wybranego obiektu)
- `:process<ENTER>`
  
  Tworzy nowy (proces) prostokąt
- `:pipe ${ARG}<ENTER>` (przyjmuje `ARG`)
  
  Zaczyna (gdy `ARG == "0" || ARG == "1"`) lub kończy (gdy `ARG == ""`) tworzenie pipe'a (strzałki)
- `:edit ${ARG}<ENTER>` (przyjmuje `ARG`)
  
  Edytuje komendę wywołania procesu (tekst w prostokącie)
- `:remove<ENTER>`
  
  Usuwa wybrany obiekt

_Uwaga, ta specyfikacja może się zmieniać._

## Co naprawdę jest aktualnie bindowane

- `<EDITION>`
- `process`
- `pipe` (przyjmuje `ARG`)
- `edit` (przyjmuje `ARG`)
- `remove`

_Z tego, co wiem, nie ma jeszcze backendu, który parsowałby komendy z `${...}`, a ten przykładowy nie rozpoznaje `:` i `<ENTER>`._

## TODO

- Zapisywanie pliku "projektu programu Linux Pipelines"
- Eksportowanie "projektu programu Linux Pipelines" do pliku `.sh`
- Informacje o błędach
  
  (_swoją drogą, jak to robimy? przez jakieś `ERROR`, czy każdy po swojemu?_)
- Integracja z backendami
- Testy?