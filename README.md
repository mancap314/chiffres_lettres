# Chiffres Lettres Solver

Solver for the famous (ex) French TV show [Des Chiffres et des Lettres](https://en.wikipedia.org/wiki/Des_chiffres_et_des_lettres).
In short, you can play either:
* *lettres*: find the longest word given a set of letters
* *chiffres*: combine numbers with basic arithmetic operations (addition, subtraction, multiplication and division) to approach as close as possible a given target

See [blog article](https://mancap314.github.io/des-chiffres-et-des-lettres.html).

## Build
```{sh}
make chiffres_lettres
```

## Usage
Examples, solving for *lettres*:
```{sh}
./chiffres_lettres.o --lettres --values abroau
```
You can also put capital letters and/or separate them:
```{sh}
./chiffres_lettres.o --lettres --values h,KiG,m,N
```
Just don't put accentuated characters such as `ê` or `à`. Candidate words with accents are matched without accent, e.g. the word "déjà" matches the set of lettres 'a', 'd', 'e', 'j'.


Solving for *chiffres* (numbers):
```{sh}
./chiffres_lettres.o --chiffres --values 19,23,34,3,234 --target 134
```
You can separate the numbers by any non-numerical and non-space character.

## Behind the scene
For chiffres, it goes through every provided number, avoiding computing same operation multiple times, and ensuring that every operation is valid, means it does not result in a negative or rational number.

For lettres, all permutations of the provided letters are generated using the (non-recursive) [Heap's algorithm](https://en.wikipedia.org/wiki/Heap%27s_algorithm). Matches are searched through binary search against [this dictionnary](https://github.com/chrplr/openlexicon/blob/master/datasets-info/Liste-de-mots-francais-Gutenberg/README-liste-francais-Gutenberg.md) of 336k+ French words.


