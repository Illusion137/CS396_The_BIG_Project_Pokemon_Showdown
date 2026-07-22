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
        alias: pokemon,
        language: "en"
    };
    const smogon_response = await rozfetch<RawPokemonData>("https://www.smogon.com/dex/_rpc/dump-pokemon", {cache_opts, headers, method: "POST", body: JSON.stringify(payload)});
    if("error" in smogon_response) return smogon_response;
    const pokemon_data = await smogon_response.json();
    if("error" in pokemon_data) return pokemon_data;
    return pokemon_data;
}

async function populate_extracted_pokemon_data(){
    for(const pokemon of pokemon_list) {
        const path = `./data/${pokemon}.json`;
        if((await fs().get_info(path)).exists) continue;
        log_info(`fetching ${pokemon}`);
        await wait(milliseconds_of({seconds: 1}));
        const extracted_data = await extract_smogon_pokemon_data(pokemon);
        if("error" in extracted_data) continue;
        await fs().write_file_as_string(path, JSON.stringify(extracted_data), {encoding: "utf8"});
    }
}

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

async function generate_pokemon_gen_cpp(){
    const pokemon_to_map = (pokemon: typeof pokemon_dump['pokemon'][0]) =>
        `    {"${pokemon.name}", BasePokemon("${pokemon.name}", {${type_string_to_cpp_type(pokemon.types[0])}, ${type_string_to_cpp_type(pokemon.types[1])}}, {${pokemon.hp}, ${pokemon.atk}, ${pokemon.def}, ${pokemon.spa}, ${pokemon.spd}, ${pokemon.spe}}, ${pokemon.weight})}`;

    const pokemon_entries = pokemon_dump.pokemon
        .filter(pokemon => pokemon.isNonstandard === "Standard")
        .map(pokemon_to_map)
        .join(",\n");

    const template =
`#include "../Pokemon.h"

std::unordered_map<std::string, BasePokemon> name_to_pokemon_map = {
${pokemon_entries}
};
`;
    await fs().write_file_as_string("../cpp_player/src/gen/PokemonGen.cpp", template, {encoding: "utf8"});
}

async function main(){
    await load_native_fs();
    await populate_extracted_pokemon_data();
    await generate_pokemon_gen_cpp();
}

main().catch(catch_log);