# Smogon Scraper
Extracts all the pokemon data from `https://www.smogon.com/dex/bw/pokemon/`.\
Uses internal library [`lib-origin`](https://github.com/Illusion137/lib-origin/tree/dev) for caching requests.
Not bundled since it's a big library and the `smogon_scraper` isn't the main focus of the project.

## Installing lib-origin
cd ./ts_smogon_scraper
git clone https://github.com/Illusion137/lib-origin/tree/dev

## Usage
Make sure packages are installed first.
```bash
yarn
```
Then ->
```bash
yarn build
```