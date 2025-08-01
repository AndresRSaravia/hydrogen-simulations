# Monte Carlo Simulations on Hydrogen

### Description

This project is a Master's degree thesis in Applied Mathematics in which we simulate the adsorption of hydrogen atoms over nickel plates / surfaces with the goal of study clean energy storage alternatives.

There are three main folders:
1. `generators` has Python scripts that creates `.in` and `.sh` files to be used in XCrySDen (visualization) and Quantum Espresso (calculating the values). Also, the folder has a tables generator for LaTeX (`generator_table.py`) and a script that checks for all possible hydrogen configurations in a 3x3 grid (`possibilities_checker.py`).
2. `inshoutdata` resides all the files that `generators` creates (not included in the repository), energy values for a hydrogen molecule and the necessary pseudopotentials files (`.upf`) for hydrogen and nickel.
3. `monte carlo` has all the C files. `main.c` (with its `Makefile`) is the main file that access all of the others which are grouped in two different folders:
    - `aux_files`: auxiliary files for constants, distributions for the Monte Carlo simulations and matrix operations
    - `mc_files`: files detailing the different Monte Carlo methods, specifically canonical and grand canonical

    `out_files` folder has all the `.json` files that `main.c` generates, `viewers` has Python files that visualize the results and statistics of the simulations, and `png_files` has all the `.png` files that the viewers generate.

### License

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg