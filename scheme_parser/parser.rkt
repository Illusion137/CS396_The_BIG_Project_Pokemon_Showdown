#lang racket
(require racket/string)
(require json)

; Format from https://github.com/smogon/pokemon-showdown/blob/master/sim/TEAMS.md ; specifically the human readable Export format
#| V1
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
    "nickname": "Silly",
    "name": "Articuno",
    "level": 100
    "item": "Leftovers",
    "ability": "Pressure",
    "evs": {"hp": 252, "atk": 0, "def": 0, "spa": 252, "spd": 4, "spe": 0}, ; Note that all unstated EVs are set to 0 by default
    "nature": "Modest",
    "ivs": {"hp": 31, "atk": 31, "def": 31, "spa": 30, "spd": 30, "spe": 31}, ; Note that all unstated IVs are set to 31 by default
    "moves": ["Ice Beam", "Hurricane", "Substitute", "Roost"],
    "gender": "M"
}
|#

(define last-line-context (box #f))
(define current-pokemon-num (box 0))
(define current-file-path (box #f))

(define (fail message)
    (display "[ERROR]: ")
    (define last-line-value-pair (unbox last-line-context))
    (when (unbox current-file-path) (printf "~a: " (unbox current-file-path)))
    (when (> (unbox current-pokemon-num) 0)
        (printf "(Pokemon #~a) " (unbox current-pokemon-num)))
    ; if we have ok line data try to let user know where the bad happened
    ; else if at EOF
    ; else no line context at all
    (cond
        [(and last-line-value-pair (cdr last-line-value-pair)) (printf "line ~a: \"~a\" - " (car last-line-value-pair) (cdr last-line-value-pair))]
        [last-line-value-pair (printf "at EOF after line (~a) - " (car last-line-value-pair))]
        [else (void)])
    (displayln message)
    (exit 1)
)
(define (fail-on-false-line line message)
    (when (boolean? line) (fail message))
)
(define (line-has-prefix? line prefix)
    (and (string? line) (string-prefix? line prefix)))
(define (line-has-suffix? line suffix)
    (and (string? line) (string-suffix? line suffix)))
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
    (fail-on-false-line line "expected a Pokemon name line, found EOF")
    (define split (map string-trim (string-split line " @ ")))
    (define fsplit (first split))
    (when (= (length split) 2)
        (hash-set! data 'item (second split)))
    (when (line-has-suffix? fsplit " (M)") 
        (
            begin
            (set! fsplit (strip-suffix fsplit " (M)"))
            (hash-set! data 'gender "M"))
        )
    (when (line-has-suffix? fsplit " (F)") 
        (
            begin
            (set! fsplit (strip-suffix fsplit " (F)"))
            (hash-set! data 'gender "F"))
        )
    (if (line-has-suffix? fsplit ")") 
        (
            begin
            (hash-set! data 'nickname (first (string-split fsplit " (")))
            (hash-set! data 'name (strip-suffix (second (string-split fsplit " (")) ")"))
        )
        (hash-set! data 'name fsplit)
    )
)
(define (maybe-parse-levels-line line data)
    (hash-set! data 'level 100)
    (maybe-parse-else-false (not (line-has-prefix? line "Level: "))
        (lambda ()
            (when (not (in-range (strip-prefix line "Level: ") 1 100))
                (fail "level must be between 1-100")
            )
            (hash-set! data 'level (string->number (strip-prefix line "Level: ")))
            #t
    ))
)

(define (parse-ability-line line data)
    (fail-on-false-line line "expected an \"Ability: \" line, found EOF")
    (when (not (string-prefix? line "Ability: "))
        (fail "expected a line starting with \"Ability: \""))
    (hash-set! data 'ability (strip-prefix line "Ability: "))
)
(define (maybe-parse-evs-line line data)
    (define ev-stat-map (empty-stat-map 0))
    (hash-set! data 'evs ev-stat-map)
    (maybe-parse-else-false (not (line-has-prefix? line "EVs: "))
        (lambda ()
            (define ev-stat-map (empty-stat-map 0))
            (for-each (lambda (ev-stat-str) (
                    begin
                    (define ev-stat-split (string-split ev-stat-str " "))
                    (when (not (= (length ev-stat-split) 2))
                        (fail (format "ev \"~a\" must look like \"<amount> <stat>\"" ev-stat-str)))
                    (if (is-stat (second ev-stat-split))
                        (if (in-range (first ev-stat-split) 0 252)
                            (hash-set! ev-stat-map (string->symbol (second ev-stat-split)) (string->number (first ev-stat-split)))
                            (fail (format "ev \"~a\" is not in range 0-252" ev-stat-str))
                            ) (fail (format "\"~a\" is not a valid stat name (expected hp/atk/def/spa/spd/spe)" (second ev-stat-split))))
                ))
                (string-split (string-downcase (strip-prefix line "EVs: ")) " / "))
            (hash-set! data 'evs ev-stat-map)
            #t
    ))
)
(define (parse-nature-line line data)
    (fail-on-false-line line "expected a \"<Nature> Nature\" line, found EOF")
    (when (not (string-suffix? line " Nature"))
        (fail "expected a line ending in \" Nature\""))
    (hash-set! data 'nature (strip-suffix line " Nature"))
)
(define (maybe-parse-ivs-line line data)
    (define iv-stat-map (empty-stat-map 31))
    (hash-set! data 'ivs iv-stat-map)
    (maybe-parse-else-false (not (line-has-prefix? line "IVs: "))
        (lambda ()
            (define iv-stat-map (empty-stat-map 31))
            (for-each (lambda (iv-stat-str) (
                    begin
                    (define iv-stat-split (string-split iv-stat-str " "))
                    (when (not (= (length iv-stat-split) 2))
                        (fail (format "iv \"~a\" must look like \"<amount> <stat>\"" iv-stat-str)))
                    (if (is-stat (second iv-stat-split))
                        (if (in-range (first iv-stat-split) 0 31)
                            (hash-set! iv-stat-map (string->symbol (second iv-stat-split)) (string->number (first iv-stat-split)))
                            (fail (format "iv \"~a\" is not in range 0-31" iv-stat-str))
                            ) (fail (format "\"~a\" is not a valid stat name (expected hp/atk/def/spa/spd/spe)" (second iv-stat-split))))
                ))
                (string-split (string-downcase (strip-prefix line "IVs: ")) " / "))
            (hash-set! data 'ivs iv-stat-map)
            #t
    ))
)
(define (parse-move-line line data-moves)
    (fail-on-false-line line "expected at least one \"- <move>\" line, found EOF")
    (hash-set! data-moves 'moves (append (hash-ref data-moves 'moves '()) (list (strip-prefix line "- "))))
)
(define (maybe-parse-move-line line data-moves)
    (maybe-parse-else-false (not (line-has-prefix? line "- "))
        (lambda ()
            (parse-move-line line data-moves)
            #t
        )
    )
)

(define (parse-pokemon all-pokemon-data lines line-numbers index-start count)
    (set! count (add1 count))
    (when (= count 7) (fail "there can be a max of 6 pokemon per team"))
    (set-box! current-pokemon-num count)

    (define pokemon-data (make-hash))
    (define lines-size (vector-length lines))
    (define index index-start)

    (define (get-line i)
        (if (< i lines-size)
            (begin
                (set-box! last-line-context (cons (vector-ref line-numbers i) (vector-ref lines i)))
                (vector-ref lines i))
            (begin
                (when (> lines-size 0)
                    (set-box! last-line-context (cons (vector-ref line-numbers (sub1 lines-size)) #f)))
                #f)))

    (parse-name-item-line (get-line index) pokemon-data)
    (set! index (add1 index))

    (when (maybe-parse-levels-line (get-line index) pokemon-data) (set! index (add1 index)))

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
    (when (not (boolean? (get-line index))) (parse-pokemon all-pokemon-data lines line-numbers index count))
)

(define (parse-file file-path)
    (set-box! last-line-context #f)
    (set-box! current-pokemon-num 0)
    (set-box! current-file-path file-path)
    (define all-pokemon-data (box '()))
    (define content (file->string file-path))
    (define raw-lines (map string-trim (string-split content "\n")))
    ; We ant to pair the lines to line numbers THEN clear empty lines so we can output better failed line no,
    (define numbered-lines-pairs (filter 
        (lambda (p) (non-empty-string? (cdr p)))
        (map cons (range 1 (add1 (length raw-lines))) raw-lines)))
    (when (null? numbered-lines-pairs) (fail (format "~a is empty" file-path)))
    (define lines (list->vector (map cdr numbered-lines-pairs)))
    (define line-numbers (list->vector (map car numbered-lines-pairs)))

    (parse-pokemon all-pokemon-data lines line-numbers 0 0)
    (unbox all-pokemon-data)
)

(define (output-team-json data output-path) 
    (with-output-to-file output-path
        (lambda () (write-json data))
        #:exists 'replace)
)

(define argv (current-command-line-arguments))
(if (= (vector-length argv) 2)
    (begin
        (displayln "parsing player.showdown")
        (output-team-json (parse-file (vector-ref argv 0)) ".tmp/player.json")
        (displayln "parsing opponent.showdown")
        (output-team-json (parse-file (vector-ref argv 1)) ".tmp/opponent.json")
    )
    (fail "argv length not valid (Expected 2)"))