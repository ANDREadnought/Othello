ANDREadnought Liu:
-- Over the past two weeks, we have made various improvements:
---- Parity Heuristic. This was just to get our feet off the ground. We needed somewhere to start.
---- 1 ply Greedy algorith. Again, we needed something that worked. 
---- University Of Washington Heuristic. 
------ After some digging, we found a research group at UWash who had done analysis on what makes a good heuristic for Othello (see references/).
------ We followed their lead and implemented what they suggested. 
---- Minimax. The next step was to write an actual searching algorithm. Easy.
---- Alpha Beta pruning. The deeper the search, the better.
---- Timing. If we want to make proper use of our time, we were going to have to time ourselves and see how deep we can get. Using <chrono> this wasn't too difficult.
---- Iterative Deepening, with timing implemented, we could now search as deep as we could in the given time. 
---- Transposition Table. We plan on using a better search. Move Ordering first. 
------ We began with PV-move order and Hash-Moves. More cuts, better search.
---- Parallelization. Some ofter-used parts of ANDRE were easily parallelized. Why not multiply their speed?
---- Bitboard. Bitwise operations are faster than what we have, and fast is ideal.

Over the course of the project, both members(Luke Juusola and Nishad Maskara) contributed approximately equally. Nishad was primarily responsible for search algorithms and squashing bugs, while Luke was primarily responsible for the auxillary pieces that aided in optimization (scripting, transposition table, timing, heuristic...). 




Instructions:
To build:
---- make
To train against self:
---- make testminimax
---- ./testminimax
To play:
---- ./testgame <Black> <White> <Time>

To extract table files:
---- ./extract.sh
To compress table files:
---- ./compress.sh
To clear tables:
---- ./extract.sh
---- ./clear.sh
---- ./compress.sh
To transfer from algebraic notation to ANDREadnought
---- ./extract.sh
---- python3 notation_format.py 
---- <read-file> (tournamentgames.txt)
---- <write-file> (tables/openingbook.txt)
---- python3 dedup.py
---- <write-file> (tables/openingbook.txt)
---- ./compress.sh

