Group #1 (Leyzerzon, Erdenbat, Schmidt, Prielipp)

We did not use any outside resources.

How the program works:
  - It reads in the input file
  - It takes the data and runs the algorithm, which computes the moves by:
     - Assigns targets to each asset by sorting the targets on a heuristic
     - It will not assign multiple assets to get the same target
     - It then takes the list of targets for each asset and computes the instructions to make it to each target
     - It then makes sure that each asset has the same number of moves by filling in any blanks with a "just go left" instruction
  - It writes the computed moves to the output file


