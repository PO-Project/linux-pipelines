# Linux Pipelines

## Kompilacja

Klonowanie:

```
git clone --recursive https://github.com/PO-Project/linux-pipelines
```

Uaktualnianie submodułów gita:

```
git submodule update --recursive --remote
```

Kompilacja:

```
cmake . ; cmake --build . --config Release
```

_CMake nie lubi mieć wszystkiego w plikach `*.h` , może się przydać poniższa komenda:_

```
find . -name '*.o' -delete
```

## Specyfikacja bindowanych komend

- `<EDITION>` (przyjmuje `KEY`)
  
  Obsługuje klawisze strzałek (nawigacja), `<DEL>` (wymazywanie) i normalne znaki (edycja tekstu)

- `<UARROW>`, `<DARROW>`, `<RARROW>`, `<LARROW>`

  Obsługuje klawisze strzałek (nawigacja)

- `:process ${ARG}`
  
  Tworzy nowy proces (prostokąt z tekstem)

- `:file ${ARG}`
  
  Tworzy nowy plik (też prostokąt z tekstem)

- `:pipeout`
  
  Zaczyna tworzenie pipe'a (strzałki) wychodzącego z _stdout_ wybranego elementu.

- `:pipeerr`
  
  Zaczyna tworzenie pipe'a (strzałki) wychodzącego z _stderr_ wybranego elementu.

- `:pipe`
  
  Kończy tworzenie pipe'a (strzałki) wchodzącego do wybranego elementu.

- `:swap`
  
  Zaczyna i kończy zamianę dwóch procesów lub plików (kolene wywołania dla dwóch elementów).

- `:quickedit ${ARG}`
  
  Edytuje komendę wywołania procesu lub nazwy pliku (tekst w prostokącie)

- `:edit`
  
  Wchodzi w tryb edycji

- `:remove`, `<DEL>`
  
  Usuwa wybrany obiekt

- `:list`
  
  Wypisuje listę plików binarnych w `PATH` i plików w _cwd_

- `:insert ${ARG}`

  Wstawia element z listy

- `:save ${ARG}`
  
  Zapisuje plik "projektu programu Linux Pipelines"

- `:open ${ARG}`
  
  Otwiera plik "projektu programu Linux Pipelines"

- `:export ${ARG}`
  
  Eksportuje "projekt programu Linux Pipelines" do bashowego skryptu

- `#vim#:edit!EDIT`

  Tryb edycji

- `o`

  Akceptuje komunikat

_Uwaga, ta specyfikacja może się zmieniać._

## Co naprawdę jest aktualnie bindowane

**To co powyżej**

_**Uwaga** `example-linux-pipelines` nie wspiera aktualnie bindowanych komend._

(_swoją drogą, jak robimy informacje o błędach? przez jakieś `ERROR`, czy każdy po swojemu?_)

## Integracja z `vim-backend`

Działa, plik binarny to `vim-linux-pipelines`

### Co działa:

- Wszystko

### Co nie działa

- ...

## TODO

- Integracja z pozostałymi backendami
- Testy?
- Dokumentacja kodu i tysiąc innych bajek do opowiadania przed snem...