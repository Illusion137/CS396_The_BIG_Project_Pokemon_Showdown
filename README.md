# CS 396 - The BIG Project - Pokemon Showdown

## Requirements
- Racket
- Prolog
- g++ or other c++ compiler
- CMake
- ninja (for CMake)
- ts-node (not for use but if you want to expand generation; everything generated comes prebuilt)

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
cmake -S cpp_player -B cpp_player/build -G Ninja
cmake --build cpp_player/build
./cpp_player/build/Showdown_Player ./teams/player.showdown ./teams/opponent.showdown
```

## How to play
Use `WASD` to move around the `cursor` and press `ENTER` to select your action.
Use `q` to exit the application.

## Notes
- This was tested both on a Windows machine and a Linux machine