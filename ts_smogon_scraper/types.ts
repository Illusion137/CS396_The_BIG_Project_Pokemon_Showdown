export interface RawPokemonData {
    languages: string[]
    learnset: string[]
    strategies: Strategy[]
    formeStrategies: any[]
}

export interface Strategy {
    format: string
    outdated: any
    overview: string
    comments: string
    movesets: Moveset[]
    credits: Credits
}

export interface Moveset {
    name: string
    pokemon: string
    shiny: boolean
    gender: string
    levels: any[]
    description: string
    abilities: string[]
    items: string[]
    teratypes: any[]
    moveslots: Moveslot[][]
    evconfigs: Evconfig[]
    ivconfigs: Ivconfig[]
    natures: string[]
}

export interface Moveslot {
    move: string
    type?: string
}

export interface Evconfig {
    hp: number
    atk: number
    def: number
    spa: number
    spd: number
    spe: number
}

export interface Ivconfig {
    hp: number
    atk: number
    def: number
    spa: number
    spd: number
    spe: number
}

export interface Credits {
    writtenBy: WrittenBy[]
    teams: Team[]
}

export interface WrittenBy {
    user_id: number
    username: string
}

export interface Team {
    name: string
    members: Member[]
}

export interface Member {
    user_id: number
    username: string
}
