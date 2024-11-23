No longer contains a makefile, as I can't make one that will work for everyone with CMake.
So please compile these yourself. CMakeLists.txt is still provided.
Will create ChessProject.exe, which you can run.

Please keep clutter out of the repo. Keep the structure according to the main branch.
You will need to use a .gitignore file for this.
Preferably should containt a src folder with all the source code, a compiled exe, a CMakeLists.txt and a readme.md.

Current progress
-FEN parser
-Attack maps for rooks, bishops and queens
-Attack maps for pawns, knights and kings

To be pushed
-Zobrist Key initialization
-Visualization along with utility info

Next Steps
-Zobrist Hash from FEN
-Attacking Bitboards from FEN (done)
-Move Gen
-Making Moves
-Print Move list
-Print Move
-Perft
