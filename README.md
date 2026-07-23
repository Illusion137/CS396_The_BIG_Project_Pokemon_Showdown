# CS 396 - The BIG Project - Pokemon Showdown

## Usage Example (Individual)
### Scheme Parser (Racket)
```bash
racket ./scheme_parser/parser.rkt ./teams/player.showdown ./teams/opponent.showdown
```

### Prolog Vaidator
```bash
swipl ./prolog_validator/validator.pl -- ./.tmp/player.json ./.tmp/opponent.json
```

### Showdown Player (MAIN)

```bash
cmake -S cpp_player -B cpp_player/build -G Ninja -DCMAKE_CXX_COMPILER=g++
cmake --build cpp_player/build
./cpp_player/build/Showdown_Player.exe ./teams/player.showdown ./teams/opponent.showdown
```