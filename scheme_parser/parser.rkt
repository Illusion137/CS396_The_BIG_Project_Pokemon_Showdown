#lang racket
(require racket/string)
(require json)

; Format from https://github.com/smogon/pokemon-showdown/blob/master/sim/TEAMS.md ; specifically the human readable Export format
#|
Example: =======================================
Articuno @ Leftovers  
Ability: Pressure  
EVs: 252 HP / 252 SpA / 4 SpD  
Modest Nature  
IVs: 30 SpA / 30 SpD  
- Ice Beam  
- Hurricane  
- Substitute  
- Roost  

Which can be broken down into: ================
[pokemon-name] [@ [pokemon-item]]?
Ability: [ability]
[EVs: [[0-252] [unique-stat]][].join(' / ')]?
[nature] Nature
[IVs: [[0-31] [unique-stat]][].join(' / ')]?
[- [unique-move-name-0]]
[- [unique-move-name-1]]?
[- [unique-move-name-2]]?
[- [unique-move-name-3]]?

output: =======================================
{
    "name": "Articuno",
    "item": "Leftovers",
    "ability": "Pressure",
    "evs": {"hp": 252, "atk": 0, "def": 0, "spa": 252, "spd": 4, "spe": 0}, ; Note that all unstated EVs are set to 0 by default
    "nature": "Modest",
    "ivs": {"hp": 31, "atk": 31, "def": 31, "spa": 30, "spd": 30, "spe": 31}, ; Note that all unstated IVs are set to 31 by default
    "moves": ["Ice Beam", "Hurricane", "Substitute", "Roost"]
}
|#

(define (fail message) 
    (display "[ERROR]: ")
    (displayln message)
    (exit 1)
)
(define (fail-on-false-line line message)
    (when (boolean? line) (fail message))
)
(define (strip-prefix str prefix)
    (if (string-prefix? str prefix)
        (substring str (string-length prefix))
        str)
)
(define (strip-suffix str suffix)
    (if (string-suffix? str suffix)
        (substring str 0 (- (string-length str) (string-length suffix)))
        str)
)
(define (in-range str-num min max)
    (if (>= (string->number str-num) min) 
        (if (<= (string->number str-num) max) 
            #t 
            #f)
        #f)
)

(define (is-stat str)
    (define stat-list (list "hp" "atk" "def" "spa" "spd" "spe"))
    (if (member str stat-list) #t #f)
)

(define (empty-stat-map default-value)
    (define stat-map (make-hash))
    (hash-set! stat-map 'hp default-value)
    (hash-set! stat-map 'atk default-value)
    (hash-set! stat-map 'def default-value)
    (hash-set! stat-map 'spa default-value)
    (hash-set! stat-map 'spd default-value)
    (hash-set! stat-map 'spe default-value)
    stat-map
)

(define (maybe-parse-else-false fail-condition function)
    (if fail-condition #f (function))
)

(define (parse-name-item-line line data)
    (fail-on-false-line line "unable to parse name-item line")
    (define split (map string-trim (string-split line " @ ")))
    (hash-set! data 'name (first split))
    (when (= (length split) 2)
        (hash-set! data 'item (second split)))
)
(define (parse-ability-line line data) 
    (fail-on-false-line line "unable to parse ability line")
    (when (not (string-prefix? line "Ability: ")) 
        (fail "ability line missing prefix"))
    (hash-set! data 'ability (strip-prefix line "Ability: "))
)
(define (maybe-parse-evs-line line data)
    (define ev-stat-map (empty-stat-map 0))
    (hash-set! data 'evs ev-stat-map)
    (maybe-parse-else-false (not (string-prefix? line "EVs: "))
        (lambda ()
            (define ev-stat-map (empty-stat-map 0))
            (for-each (lambda (ev-stat-str) (
                    begin
                    (define ev-stat-split (string-split ev-stat-str " "))
                    (if (is-stat (second ev-stat-split))
                        (if (in-range (first ev-stat-split) 0 252)
                            (hash-set! ev-stat-map (string->symbol (second ev-stat-split)) (string->number (first ev-stat-split)))
                            (fail "ev stat is not in range 0-252")
                            ) (fail "ev stat is not a valid stat"))
                ))
                (string-split (string-downcase (strip-prefix line "EVs: ")) " / "))
            (hash-set! data 'evs ev-stat-map)
            #t
    ))
)
(define (parse-nature-line line data) 
    (fail-on-false-line line "unable to parse nature line")
    (when (not (string-suffix? line " Nature")) 
        (fail "nature line missing suffix"))
    (hash-set! data 'nature (strip-suffix line " Nature"))
)
(define (maybe-parse-ivs-line line data)
    (define iv-stat-map (empty-stat-map 31))
    (hash-set! data 'ivs iv-stat-map)
    (maybe-parse-else-false (not (string-prefix? line "IVs: "))
        (lambda ()
            (define iv-stat-map (empty-stat-map 31))
            (for-each (lambda (iv-stat-str) (
                    begin
                    (define iv-stat-split (string-split iv-stat-str " "))
                    (if (is-stat (second iv-stat-split))
                        (if (in-range (first iv-stat-split) 0 31)
                            (hash-set! iv-stat-map (string->symbol (second iv-stat-split)) (string->number (first iv-stat-split)))
                            (fail "iv stat is not in range 0-31")
                            ) (fail "iv stat is not a valid stat"))
                ))
                (string-split (string-downcase (strip-prefix line "IVs: ")) " / "))
            (hash-set! data 'ivs iv-stat-map)
            #t
    ))
)
(define (parse-move-line line data-moves) 
    (fail-on-false-line line "unable to parse move line")
    (hash-set! data-moves 'moves (append (hash-ref data-moves 'moves '()) (list (strip-prefix line "- "))))
)
(define (maybe-parse-move-line line data-moves)
    (maybe-parse-else-false (not (string-prefix? line "- "))
        (lambda ()
            (parse-move-line line data-moves)
            #t
        )
    )
)

(define (parse-pokemon all-pokemon-data lines index-start count)
    (set! count (add1 count))
    (when (= count 7) (fail "there can be a max of 6 pokemon per team"))

    (define pokemon-data (make-hash))
    (define lines-size (vector-length lines))
    (define index index-start)

    (define (get-line i) 
        (if (< i lines-size) (vector-ref lines i) #f))

    (parse-name-item-line (get-line index) pokemon-data)
    (set! index (add1 index))
    (parse-ability-line (get-line index) pokemon-data)
    (set! index (add1 index))

    (when (maybe-parse-evs-line (get-line index) pokemon-data) (set! index (add1 index)))
    (parse-nature-line (get-line index) pokemon-data)
    (set! index (add1 index))
    (when (maybe-parse-ivs-line (get-line index) pokemon-data) (set! index (add1 index)))

    (define data-moves (make-hash))
    (hash-set! data-moves 'moves '())
    (parse-move-line (get-line index) data-moves)
    (set! index (add1 index))
    (when (maybe-parse-move-line (get-line index) data-moves)
        (begin 
            (set! index (add1 index))
            (when (maybe-parse-move-line (get-line index) data-moves)
                (begin 
                    (set! index (add1 index))
                    (when (maybe-parse-move-line (get-line index) data-moves)
                        (set! index (add1 index)))
                ))
        ))
    (hash-set! pokemon-data 'moves (hash-ref data-moves 'moves))

    (set-box! all-pokemon-data (append (unbox all-pokemon-data) (list pokemon-data)))
    (when (not (boolean? (get-line index))) (parse-pokemon all-pokemon-data lines index count))
)

(define (parse-file file-path)
    (define all-pokemon-data (box '()))
    (define content (file->string file-path))
    (define lines (list->vector (filter non-empty-string? (map string-trim (string-split content "\n")))))

    (parse-pokemon all-pokemon-data lines 0 0)
    (unbox all-pokemon-data)
)

(define (output-team-json data output-path) 
    (with-output-to-file output-path
        (lambda () (write-json data))
        #:exists 'replace)
)

(define argv (current-command-line-arguments))
(displayln argv)
(if (= (vector-length argv) 2)
    (begin
        (output-team-json (parse-file (vector-ref argv 0)) ".tmp/player.json")
        (output-team-json (parse-file (vector-ref argv 1)) ".tmp/opponent.json")
    )
    (fail "argv length not valid (Expected 2)"))