:- [gen_db].
:- use_module(library(json)).
:- use_module(library(pairs)).

has_index(Array, Index) :-
    nth0(Index, Array, _).

is_unique([]).
is_unique([X|Xs]) :-
    \+ member(X, Xs),
    is_unique(Xs).

pokemon_move_valid(Name, Move) :-
    pokemon(Name),
    move(Move),
    has_move(Name, Move).

pokemon_all_moves_valid(Name, Moves) :-
    (\+ has_index(Moves, 0) ; nth0(0, Moves, M0), pokemon_move_valid(Name, M0)),
    (\+ has_index(Moves, 1) ; nth0(1, Moves, M1), pokemon_move_valid(Name, M1)),
    (\+ has_index(Moves, 2) ; nth0(2, Moves, M2), pokemon_move_valid(Name, M2)),
    (\+ has_index(Moves, 3) ; nth0(3, Moves, M3), pokemon_move_valid(Name, M3)).

pokemon_ability_valid(Name, Ability) :-
    pokemon(Name),
    ability(Ability),
    has_ability(Name, Ability).

pokemon_evs_valid(EVs) :-
    sum_list([EVs.hp, EVs.atk, EVs.def, EVs.spa, EVs.spd, EVs.spe], Sum),
    Sum =< 512.

is_pokemon_valid(Pokemon) :-
    pokemon(Pokemon.name),
    item(Pokemon.item),
    nature(Pokemon.nature),
    pokemon_ability_valid(Pokemon.name, Pokemon.ability),
    pokemon_all_moves_valid(Pokemon.name, Pokemon.moves),
    pokemon_evs_valid(Pokemon.evs).

is_team_valid(PokemonTeam) :-
    maplist(is_pokemon_valid, PokemonTeam).

read_pokemon_file(Path, OutputTeam) :-
    setup_call_cleanup(
        open(Path, read, Stream),
        json_read_dict(Stream, OutputTeam, [value_string_as(atom)]),
        close(Stream)
    ).

print_pokemon(P) :-
    format("~w @ ~w~n", [P.name, P.item]),
    format("  Ability: ~w~n", [P.ability]),
    format("  Nature: ~w~n", [P.nature]),
    format("  EVs: ~w~n", [P.evs]),
    format("  IVs: ~w~n", [P.ivs]),
    format("  Moves: ~w~n", [P.moves]).

main(Argv) :-
    Argv = [PlayerJSONPath, OpponentJSONPath],
    read_pokemon_file(PlayerJSONPath, PlayerTeam),
    read_pokemon_file(OpponentJSONPath, OpponentTeam),
    format("Team 1 (~w): ~w Pokemon~n", [PlayerJSONPath, PlayerTeam]),
    format("Team 2 (~w): ~w Pokemon~n", [OpponentJSONPath, OpponentTeam]),
    maplist(print_pokemon, PlayerTeam),
    maplist(print_pokemon, OpponentTeam),
    is_team_valid(PlayerTeam),
    is_team_valid(OpponentTeam).

:- initialization(main, main).