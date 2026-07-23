import rozfetch, { type RoZFetchRequestInit } from "@common/rozfetch";
import type { PromiseResult } from "@common/types";
import { catch_log } from "@common/utils/error_util";
import { wait } from "@common/utils/timed_util";
import { milliseconds_of } from "@common/utils/util";
import { fs, load_native_fs } from "@roze/native/fs/fs";
import { log_info } from '@common/log';
import type { RawPokemonData } from "types";
import pokemon_list from 'data/pokemon.json';
import pokemon_dump from 'data/basics_dump.json';

const cache_opts: RoZFetchRequestInit['cache_opts'] = {
    cache_mode: 'file',
    cache_ms: milliseconds_of({years: 1}),
    cache_ms_fail: 0,
    cache_on: "request"
}
const headers = {
    "accept": "*/*",
    "accept-language": "en-US,en;q=0.9",
    "cache-control": "no-cache",
    "content-type": "application/json",
    "pragma": "no-cache",
    "priority": "u=1, i",
    "sec-ch-ua": "\"Not;A=Brand\";v=\"8\", \"Chromium\";v=\"150\", \"Google Chrome\";v=\"150\"",
    "sec-ch-ua-mobile": "?0",
    "sec-ch-ua-platform": "\"Windows\"",
    "sec-fetch-dest": "empty",
    "sec-fetch-mode": "cors",
    "sec-fetch-site": "same-origin",
    "Referer": "https://www.smogon.com/dex/bw/pokemon/alomomola/",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/150.0.0.0 Safari/537.36"
}

async function extract_smogon_pokemon_data(pokemon: string): PromiseResult<RawPokemonData> {
    const payload = {
        gen: "bw",
        alias: pokemon.replaceAll("'", ''),
        language: "en"
    };
    const smogon_response = await rozfetch<RawPokemonData>("https://www.smogon.com/dex/_rpc/dump-pokemon", {cache_opts, headers, method: "POST", body: JSON.stringify(payload)});
    if("error" in smogon_response) return smogon_response;
    const pokemon_data = await smogon_response.json();
    if(pokemon_data === null || "error" in pokemon_data) return pokemon_data;
    return pokemon_data;
}

async function populate_extracted_pokemon_data(){
    for(const pokemon of pokemon_list) {
        const path = `./data/${pokemon}.json`;
        if((await fs().get_info(path)).exists) continue;
        log_info(`fetching ${pokemon}`);
        await wait(milliseconds_of({seconds: 1}));
        const extracted_data = await extract_smogon_pokemon_data(pokemon);
        if(extracted_data === null || "error" in extracted_data) continue;
        await fs().write_file_as_string(path, JSON.stringify(extracted_data), {encoding: "utf8"});
    }
}

const AUTO_GENERATED_HEADER_CPP = "// AUTO GENERATED DO NOT MODIFY";
const AUTO_GENERATED_HEADER_PROLOG = "% AUTO GENERATED DO NOT MODIFY";

function type_string_to_cpp_type(type_string: string|undefined){
    switch(type_string){
        case "Bug": return "Type::BUG";
        case "Dark": return "Type::DARK";
        case "Dragon": return "Type::DRAGON";
        case "Electric": return "Type::ELECTRIC";
        case "Fighting": return "Type::FIGHTING";
        case "Fire": return "Type::FIRE";
        case "Flying": return "Type::FLYING";
        case "Ghost": return "Type::GHOST";
        case "Grass": return "Type::GRASS";
        case "Ground": return "Type::GROUND";
        case "Ice": return "Type::ICE";
        case "Normal": return "Type::NORMAL";
        case "Poison": return "Type::POISON";
        case "Psychic": return "Type::PSYCHIC";
        case "Rock": return "Type::ROCK";
        case "Steel": return "Type::STEEL";
        case "Water": return "Type::WATER";
        default: return "Type::NONE";
    }
}

function move_category_string_to_cpp_category(category_string: string){
    switch(category_string){
        case "Non-Damaging": return "MoveCategory::NON_DAMAGING";
        case "Physical": return "MoveCategory::PHYSICAL";
        case "Special": return "MoveCategory::SPECIAL";
        default: throw new Error(`unknown move category: ${category_string}`);
    }
}

function move_target_string_to_cpp_target(target_string: string){
    switch(target_string){
        case "Self": return "MoveTarget::SELF";
        case "AdjacentAllyOrSelf": return "MoveTarget::ADJACENT_ALLY_OR_SELF";
        case "Any": return "MoveTarget::ANY";
        case "Normal": return "MoveTarget::NORMAL";
        case "AllyTeam": return "MoveTarget::ALLY_TEAM";
        case "AllAdjacentFoes": return "MoveTarget::ALL_ADJACENT_FOES";
        case "All": return "MoveTarget::ALL";
        case "AdjacentAlly": return "MoveTarget::ADJACENT_ALLY";
        case "AllySide": return "MoveTarget::ALLY_SIDE";
        case "AdjacentFoe": return "MoveTarget::ADJACENT_FOE";
        case "FoeSide": return "MoveTarget::FOE_SIDE";
        case "RandomNormal": return "MoveTarget::RANDOM_NORMAL";
        case "AllAdjacent": return "MoveTarget::ALL_ADJACENT";
        case "Scripted": return "MoveTarget::SCRIPTED";
        default: throw new Error(`unknown move target: ${target_string}`);
    }
}

function move_flag_string_to_cpp_flag(flag_string: string){
    switch(flag_string){
        case "Bypasses Substitute": return "MoveFlags::BYPASSES_SUBSTITUTE";
        case "Charge": return "MoveFlags::CHARGE";
        case "Contact": return "MoveFlags::CONTACT";
        case "Defrost": return "MoveFlags::DEFROST";
        case "Distance": return "MoveFlags::DISTANCE";
        case "Fail Copycat": return "MoveFlags::FAIL_COPYCAT";
        case "Fail Encore": return "MoveFlags::FAIL_ENCORE";
        case "Fail Me First": return "MoveFlags::FAIL_ME_FIRST";
        case "Fail Mimic": return "MoveFlags::FAIL_MIMIC";
        case "Fail Sketch": return "MoveFlags::FAIL_SKETCH";
        case "Future Move": return "MoveFlags::FUTURE_MOVE";
        case "Gravity": return "MoveFlags::GRAVITY";
        case "Heal": return "MoveFlags::HEAL";
        case "Metronome": return "MoveFlags::METRONOME";
        case "Mirror": return "MoveFlags::MIRROR";
        case "Must Pressure": return "MoveFlags::MUST_PRESSURE";
        case "No Assist": return "MoveFlags::NO_ASSIST";
        case "No Sleep Talk": return "MoveFlags::NO_SLEEP_TALK";
        case "Pledge Combo": return "MoveFlags::PLEDGE_COMBO";
        case "Protect": return "MoveFlags::PROTECT";
        case "Punch": return "MoveFlags::PUNCH";
        case "Recharge": return "MoveFlags::RECHARGE";
        case "Reflectable": return "MoveFlags::REFLECTABLE";
        case "Snatch": return "MoveFlags::SNATCH";
        case "Sound": return "MoveFlags::SOUND";
        default: throw new Error(`unknown move flag: ${flag_string}`);
    }
}

async function generate_move_gen_cpp(){
    const move_to_map = (move: typeof pokemon_dump['moves'][0]) =>
        `    {"${move.name}", MoveBase("${move.name}", "${move.description}", ${move_category_string_to_cpp_category(move.category)}, ${move.pp}, ${move.power}, ${move.accuracy}, ${move.priority}, ${move_target_string_to_cpp_target(move.target)}, ${type_string_to_cpp_type(move.type)}, {${move.flags.map(move_flag_string_to_cpp_flag).join(", ")}})}`;

    const move_entries = pokemon_dump.moves
        .filter(move => move.isNonstandard === "Standard")
        .map(move_to_map)
        .join(",\n");

    const template =
`${AUTO_GENERATED_HEADER_CPP}
#include "../Move.h"

std::unordered_map<std::string, MoveBase> name_to_move_map = {
${move_entries}
};
`;
    await fs().write_file_as_string("../cpp_player/src/gen/MoveBaseGen.cpp", template, {encoding: "utf8"});
}

async function generate_pokemon_gen_cpp(){
    const pokemon_to_map = (pokemon: typeof pokemon_dump['pokemon'][0]) =>
        `    {"${pokemon.name}", BasePokemon("${pokemon.name}", {${type_string_to_cpp_type(pokemon.types[0])}, ${type_string_to_cpp_type(pokemon.types[1])}}, {${pokemon.hp}, ${pokemon.atk}, ${pokemon.def}, ${pokemon.spa}, ${pokemon.spd}, ${pokemon.spe}}, ${pokemon.weight})}`;

    const pokemon_entries = pokemon_dump.pokemon
        .filter(pokemon => pokemon.isNonstandard === "Standard")
        .map(pokemon_to_map)
        .join(",\n");

    const template =
`${AUTO_GENERATED_HEADER_CPP}
#include "../Pokemon.h"

std::unordered_map<std::string, BasePokemon> name_to_pokemon_map = {
${pokemon_entries}
};
`;
    await fs().write_file_as_string("../cpp_player/src/gen/PokemonGen.cpp", template, {encoding: "utf8"});
}

function esc(str: string){
    return str.replaceAll('\'', '\\\'');
}

async function pokemon_has_move(pokemon: string) {
    let pokemon_data_name = pokemon.toLowerCase();
    if(pokemon_data_name === "mr. mime") pokemon_data_name = "mr-mime";
    if(pokemon_data_name === "mime jr.") pokemon_data_name = "mime-jr";
    const path = `./data/${pokemon_data_name}.json`;
    const pokemon_data_string = await fs().read_as_string(path, {encoding: "utf8"});
    if(typeof pokemon_data_string === "object") return ""; // good reason since some pokemon in list are bad data
    const pokemon_data: RawPokemonData = JSON.parse(pokemon_data_string); // assume no fail here
    return pokemon_data.learnset.map(move => `has_move('${esc(pokemon)}', '${esc(move)}').`).join('\n');
}

async function generate_prolog_database() {
    const template = 
`${AUTO_GENERATED_HEADER_PROLOG}
% Pokemon list
${pokemon_dump.pokemon.map(p => `pokemon('${esc(p.name)}').`).join('\n')}

% Move list
${pokemon_dump.moves.map(m => `move('${esc(m.name)}').`).join('\n')}

% Item list
${pokemon_dump.items.map(i => `item('${esc(i.name)}').`).join('\n')}

% Ability list
${pokemon_dump.abilities.map(a => `ability('${esc(a.name)}').`).join('\n')}

% Nature list
${pokemon_dump.natures.map(n => `nature('${esc(n.name)}').`).join('\n')}

% Pokemon has move
${(await Promise.all(pokemon_dump.pokemon.map(p => pokemon_has_move(p.name)))).join('\n')}

% Pokemon has ability
${pokemon_dump.pokemon.map(p => p.abilities.map(a => `has_ability('${esc(p.name)}', '${esc(a)}').`).join('\n')).join('\n')}
`;

    await fs().write_file_as_string("../prolog_validator/gen_db.pl", template, {encoding: "utf8"});
}

async function main(){
    await load_native_fs();
    await populate_extracted_pokemon_data();
    await generate_pokemon_gen_cpp();
    await generate_move_gen_cpp();

    await generate_prolog_database();
}

main().catch(catch_log);