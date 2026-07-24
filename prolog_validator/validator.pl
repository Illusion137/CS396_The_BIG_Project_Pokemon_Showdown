:- [gen_db].
:- [hidden_power_db].
:- use_module(library(json)).
:- use_module(library(pairs)).

hidden_power_types(['Fighting', 'Flying', 'Poison', 'Ground', 'Rock', 'Bug', 'Ghost', 'Steel', 'Fire', 'Water', 'Grass', 'Electric', 'Psychic', 'Ice', 'Dragon', 'Dark']).

hidden_power_type_from_ivs(IVs, Type) :-
    Bits is (IVs.hp mod 2) 
        + 2 * (IVs.atk mod 2) 
        + 4 * (IVs.def mod 2) 
        + 8 * (IVs.spe mod 2) 
        + 16 * (IVs.spa mod 2) 
        + 32 * (IVs.spd mod 2),
    Index is (Bits * 15) // 63,
    hidden_power_types(Types),
    nth0(Index, Types, Type).

has_index(Array, Index) :-
    nth0(Index, Array, _).

is_unique([]).
is_unique([X|Xs]) :-
    \+ member(X, Xs),
    is_unique(Xs).

pokemon_move_valid(Name, Move) :-
    pokemon(Name),
    move(Move),
    ( hidden_power(Move)
        -> has_move(Name, 'Hidden Power')
        ; has_move(Name, Move)
    ).

pokemon_all_moves_valid(Name, Moves) :-
    (\+ has_index(Moves, 0) ; nth0(0, Moves, M0), pokemon_move_valid(Name, M0)),
    (\+ has_index(Moves, 1) ; nth0(1, Moves, M1), pokemon_move_valid(Name, M1)),
    (\+ has_index(Moves, 2) ; nth0(2, Moves, M2), pokemon_move_valid(Name, M2)),
    (\+ has_index(Moves, 3) ; nth0(3, Moves, M3), pokemon_move_valid(Name, M3)).

pokemon_hidden_power_valid(Pokemon) :-
    include(hidden_power, Pokemon.moves, HiddenPowerMoves),
    length(HiddenPowerMoves, Count),
    Count =< 1,
    (HiddenPowerMoves = [HiddenPowerMove]
        -> hidden_power_type_from_ivs(Pokemon.ivs, Type),
            format(atom(BracketedType), "Hidden Power [~w]", [Type]),
            format(atom(PlainType), "Hidden Power ~w", [Type]),
            ( HiddenPowerMove == BracketedType
                ; HiddenPowerMove == PlainType
            )
        ; true
    ).

pokemon_nickname_valid(Pokemon) :-
    ( get_dict(nickname, Pokemon, Nickname)
    -> atom_length(Nickname, Len),
       Len =< 10
    ; true
    ).

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
    pokemon_evs_valid(Pokemon.evs),
    pokemon_hidden_power_valid(Pokemon),
    pokemon_nickname_valid(Pokemon).

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
    maplist(print_pokemon, PlayerTeam),
    maplist(print_pokemon, OpponentTeam),
    is_team_valid(PlayerTeam),
    is_team_valid(OpponentTeam).

:- initialization(main, main).