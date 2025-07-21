# Monte Carlo Simulations on Hydrogen
This project is a Master's degree thesis in Applied Mathematics in which we simulate the adsorption of hydrogen atoms over nickel plates / surfaces with the goal of study clean energy storage alternatives.

There are three main folders:
1. `generators` has Python scripts that creates `.in` and `.sh` files to be used in XCrySDen (visualization) and Quantum Espresso (calculating the values). Also, the folder has a tables generator for LaTeX (`generator_table.py`) and a script that checks for all possible hydrogen configurations in a 3x3 grid (`possibilities_checker.py`).
2. `inshoutdata` resides all the files that `generators` creates (not included in the repository), energy values for a hydrogen molecule and the necessary pseudopotentials files (`.upf`) for hydrogen and nickel.
3. `montecarlo` has all the C files. `main.c` (with its `Makefile`) is the main file that access all of the others which are grouped in two different folders:
    - `aux_files`: auxiliary files for constants, distributions for the Monte Carlo simulations and matrix operations
    - `mc_files`: files detailing the different Monte Carlo methods, specifically canonical and grand canonical

    `out_files` folder has all the `.json` files that main.c generates, whereas the `viewers` one has Python files that visualize the results and statistics of the simulations.
