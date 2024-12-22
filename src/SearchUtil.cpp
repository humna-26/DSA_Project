#include "SearchUtil.h"

using namespace std;

// pv util variables
int pvLength[128];
int pvTable[128][128];
int followPV;
int scorePV;

// 3-fold rep
uint64_t gameHistoryTable[17700];
int repetitionIndex;

/*

    FUNCTIONS

*/

bool stopSearch = false;

void resetSearch() {
    stopSearch = false;
    // Reset other search-related variables as needed
    followPV = 0;
    scorePV = 0;
    memset(killerMoves, 0, sizeof(killerMoves));
    memset(historyMoves, 0, sizeof(historyMoves));
    memset(pvTable, 0, sizeof(pvTable));
    memset(pvLength, 0, sizeof(pvLength));
}

void perft_test(Board *board, int depth, int *count)
{
    // Base condition
    if(depth == 0){
        // Count the leaf node and return
        (*count)++;
        return;
    }
    // Save the board state
    Board boardCopy = Board();

    // Generate Moves into Move list
    board->generateMoves();

    // Loop over all the legal moves
    for(int i = 0; i < (*board).moveList.count; i++){
        // Save the board state
        memcpy(&boardCopy, board, sizeof(*board));

        // Apply the i'th move to the board
        // Need the makeMove function for this to be done properly

        if(!(*board).makeMove((*board).moveList.moves[i]))
            continue;

        // Call function with updated board and further depth
        perft_test(board, depth - 1, count);

        // Restore the board state
        memcpy(board, &boardCopy, sizeof(*board));
    }
}

// Function to handle perft. prints number of leaf nodes and the time taken to generate
void perft(Board *board, int depth){

    int countn = 0;
    int countp = 0;
    Board copy;

    cout << "Starting Perft" << endl << endl;

    auto start = chrono::high_resolution_clock::now();


    (*board).generateMoves();

    for(int i = 0; i < board->moveList.count; i++){
        memcpy(&copy, board, sizeof(Board));

        if(!board->makeMove(board->moveList.moves[i]))
            continue;

        perft_test(board, depth - 1, &countn);


        memcpy(board, &copy, sizeof(Board));
        cout << printMove(board->moveList.moves[i]) << ": " << countn - countp << endl;
        countp = countn;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << endl << "Perft at depth " << depth << " : " << countn << endl;
    cout << "Time taken: " << duration.count() << " ms; " << ((double)countn / duration.count()) / 1000 << " Mn/s" << endl;
}

// Negamax search with alpha-beta pruning
// depth is in plies
int negamax(Board board, int alpha, int beta, int depth, int ply, int *move, int *nodes){
    
    if (((*nodes) & 2047) == 0) {  // Check every 2048 nodes
        if (timeManager.shouldStop()) {
            stopSearch = true;
        }
    }
    
    // declare score
    int score;

    // default init to alpha flag
    int entryFlag = entryAlpha;

    // check for rep
    if(ply > 0 && repetitionDetection(board.zobristHash))
        return 0;

    // init pv length
    pvLength[ply] = ply;

    // check if move is stored, and return if yes
    if((ply > 0) && !((score = tt.search(board.zobristHash, alpha, beta, depth, ply)) == 2100000000) && !((beta - alpha) > 0))
        return score;

    // Base condition
    if(depth == 0){
        // Start quiescense search at depth 0
        return quiescenseSearch(board, alpha, beta, ply,nodes);
    }

    // drop search if depth goes beyond what pv arrs can handle
    if(ply >= 128)
        return evaluatePosition(board);

    (*nodes)++;

    // declare variable to store board copy in
    Board boardCopy;

    int currAlpha = alpha;
    int bestMove;

    // Variables for checkmate/stalemate detection
    bool isChecked = isSquareAttacked(1-board.sideToMove, board, getLSBIndex(board.pieceBitboards[board.sideToMove][king]));
    int legalMovesCount = 0;

    // search extension for if king is in check
    if(isChecked) depth++;

    // null move pruning
    // condition set to true for now. later make it so null move pruning is not done if we are in zugzwang
    // or just simply let it be if feeling lazy and because zugzwangs are so rare
    // beside zugzwang, might also miss some mate in x's that the opponent might have
    if(true){
        if(depth >= 3 && isChecked == 0 && ply > 0){
            // save board
            memcpy(&boardCopy, &board, sizeof(Board));

            // switch side. basically give the opponents 2 moves to make. also reset enpassant bc 2 moves for same side
            // also update zobrist hash
            board.sideToMove = 1 - board.sideToMove;
            board.zobristHash ^= sideToMoveKey;
            if(board.enpassantSquare != -1) board.zobristHash ^= enpassantKeys[board.enpassantSquare % 8];
            board.enpassantSquare = -1;

            // increment repetition index and store zobrist hash
            repetitionIndex++;
            gameHistoryTable[repetitionIndex] = board.zobristHash;

            // search with reduced depth and smaller alpha-beta window
            score = -negamax(board, -beta, -beta + 1, depth - 3, ply + 1, move, nodes);

            // decrement repetition index
            repetitionIndex--;

            // restore board
            memcpy(&board, &boardCopy, sizeof(Board));

            if (stopSearch) return 0;

            // check for beta prune
            if(score >= beta){
                return beta;
            }
        }
    }

    // generate moves
    board.generateMoves();
    // sort moves according to score
    sortMoves(board, ply);

    // pv move ordering
    if(followPV){
        followPV = 0;
        for(int i = 0; i < board.moveList.count; i++){
            if(pvTable[0][ply] == board.moveList.moves[i]){
                // Enable if current move is part of pv
                scorePV = 1;
                followPV = 1;
            }
        }
    }

    // variable to keep track of number of moves searched. (used in lmr)
    int movesSearched = 0;

    // Loop over all moves
    for(int i = 0; i < board.moveList.count; i++){

        // Save current board config
        memcpy(&boardCopy, &board, sizeof(board));

        // make the move, and check whether illegal
        if (!board.makeMove(board.moveList.moves[i]))
            continue;

        // Increment legal moves
        legalMovesCount++;

        // search deeper. PVS + LMR applied
        // normal full search at first
        if(movesSearched == 0){
            // increment repetition index and store zobrist hash
            repetitionIndex++;
            gameHistoryTable[repetitionIndex] = board.zobristHash;

            score = -negamax(board, -beta, -alpha, depth - 1, ply + 1, move, nodes);

            repetitionIndex--;
        }
        // Apply LMR + PVS here
        else {
            // condition for reducing depth (LMR)
            if(movesSearched >= fullDepthMoves && depth >= reductionLimit && isChecked == 0 && get_move_capture(board.moveList.moves[i]) == 0 && get_move_promoted(board.moveList.moves[i]) == 0){
                // increment repetition index and store zobrist hash
                repetitionIndex++;
                gameHistoryTable[repetitionIndex] = board.zobristHash;

                // search with reduced depth
                score = -negamax(board, -alpha - 1, -alpha, depth - 2, ply + 1, move, nodes);

                repetitionIndex--;
            }
            else {
                // ensure that full depth search is done
                score = alpha + 1;
            }

            // PVS here
            if(score > alpha){
                // increment repetition index and store zobrist hash
                repetitionIndex++;
                gameHistoryTable[repetitionIndex] = board.zobristHash;

                // Assume current node to be the best in the whole tree at the current ply level
                // pv moves are most likely to be the best one, if we close the range between alpha and beta on the score
                // of the current pv move, it will prune alot more of the tree if it is actually the best move.
                score = -negamax(board, -alpha - 1, -alpha, depth - 1, ply + 1, move, nodes);

                repetitionIndex--;

                // this will however, not work everytime. move from the last pv isn't necessarily the best move for the current search depth.
                // in this case we will check if the score is outside  the bounds of alpha and beta, which means a move better than the last pv move was found.
                // Then we simply have to run a normal search again, which makes the time spent on the last pv search wasted.
                // but the time saved from doing this pv search is greater than the time wasted on it, so it makes our search faster overall in most cases.
                if(score > alpha && score < beta){
                    // increment repetition index and store zobrist hash
                    repetitionIndex++;
                    gameHistoryTable[repetitionIndex] = board.zobristHash;

                    score = -negamax(board, -beta, -alpha, depth - 1, ply + 1, move, nodes);

                    repetitionIndex--;
                }
            }
        }

        // Load back the current config
        memcpy(&board, &boardCopy, sizeof(board));

        if (stopSearch) return 0;

        // increment number of moves searched
        movesSearched++;

        // Found a better move
        if(score > alpha){
            
            // update entry flag to pv
            entryFlag = entryExact;

            // store history move
            if(get_move_capture(board.moveList.moves[i]) == 0)
                historyMoves[get_move_colour(board.moveList.moves[i]) > 0][get_move_piece(board.moveList.moves[i])][get_move_target(board.moveList.moves[i])] += depth;

            // update alpha with the better one
            alpha = score;

            // add to pv table
            pvTable[ply][ply] = board.moveList.moves[i];

            // copy from deeper pv into this one
            for(int nply = ply + 1; nply < pvLength[ply+1]; nply++){
                pvTable[ply][nply] = pvTable[ply+1][nply];
            }

            // adjust pv length
            pvLength[ply] = pvLength[ply+1];

            // if we are searching at root move, update the best move
            if(ply == 0){
                bestMove = board.moveList.moves[i];
            }

            // prune for beta fail
            if(score >= beta){

                // store tt entry as beta
                tt.store(beta, depth, board.zobristHash, entryBeta, ply);

                if(get_move_capture(board.moveList.moves[i]) > 0)
                    return beta;
                // add killer
                killerMoves[1][ply] = killerMoves[0][ply];
                killerMoves[0][ply] = board.moveList.moves[i];

                return beta;
            }
        }
    }

    // If no more moves to be made
    if(legalMovesCount == 0){
        // If king is checked, i.e checkmate
        if(isChecked){
            // return (almost) worst possible score + ply count
            // ply count ensures it will go for the quickest mate
            return -mateScore + ply;
        }
        // else for stalemate
        else{
            // return 0 because draw
            return 0;
        }
    }

    // if alpha has changed, better move was found.
    if(currAlpha != alpha){
        *move = bestMove;
    }

    // write entry with alpha
    tt.store(alpha, depth, board.zobristHash, entryFlag, ply);

    // return the eval of the best move that is reachable
    return alpha;
}

// negamax modified search to search into moves that are only captures. avoids the horizon effect which is caused by fixed search depth
int quiescenseSearch(Board board, int alpha, int beta, int ply, int *nodes){

    if (((*nodes) & 2047) == 0) {  // Check every 2048 nodes
        if (timeManager.shouldStop()) {
            stopSearch = true;
        }
    }

    int eval = evaluatePosition(board);

    // Too deep for the pv table to handle, just return eval
    if(ply >= 128)
        return eval;

    (*nodes)++;

    // beta prune
    if(eval >= beta){
        return beta;
    }

    // found better move
    if(eval > alpha){
        alpha = eval;
    }

    board.generateMoves();
    sortMoves(board, ply);
    Board cpy;

    // loop over all moves
    for(int i = 0; i < board.moveList.count; i++){
        // save board
        memcpy(&cpy, &board, sizeof(Board));


        // skip move if illegal and is not a capture
        if (!get_move_capture(board.moveList.moves[i]) || !board.makeMove(board.moveList.moves[i])){
            continue;
        }

        // increment repetition index and store zobrist hash
        repetitionIndex++;
        gameHistoryTable[repetitionIndex] = board.zobristHash;

        // search further if move is a capture
        int score = -quiescenseSearch(board, -beta, -alpha, ply + 1,nodes);

        repetitionIndex--;

        // reset board
        memcpy(&board, &cpy, sizeof(Board));

        if (stopSearch) return 0;
        
        // found better move
        if(score > alpha){
            alpha = score;

            // beta prune
            if(score >= beta)
                return beta;
        }
    }

    return alpha;
}

// findBestMove now uses time management
std::pair<int, int> findBestMove(Board board, int depth) {
    resetSearch();
    timeManager.start();

    
    int nodes = 0;
    int move = 0;
    int score = 0;
    int bestMove = 0;
    int bestScore = -infinity;

    // pre declaration of alpha and beta for aspiration window
    int alpha = -infinity;
    int beta = infinity;

    // Iterative deepening
    for(int currentDepth = 1; currentDepth <= depth; currentDepth++) {
        if (stopSearch) {
            return {bestMove, bestScore};
        }

        followPV = 1;
        
        score = negamax(board, alpha, beta, currentDepth, 0, &move, &nodes);

        // if score falls outside of the window, do a full search again at same depth
        // time gained is more than wasted here (hopefully)

        if(score <= alpha || score >= beta){
            alpha = -infinity;
            beta = infinity;
            currentDepth--;
            continue;
        }

        if(stopSearch) {
            return {bestMove, bestScore};
        }

        bestMove = move;
        bestScore = score;

        alpha = score - aspirationWindow;
        beta = score + aspirationWindow;

        // UCI info output
        cout << "info"
            << " depth " << currentDepth
            << " score " << formatScore((board.sideToMove == white ? score : -score))
            << " nodes " << nodes
            << " time " << timeManager.getElapsedTime()
            << " nps " << (nodes * 1000LL / (timeManager.getElapsedTime() + 1))
            << " pv ";
        
        for(int j = 0; j < pvLength[0]; j++) {
            std::cout << printMove(pvTable[0][j]) << " ";
        }
        std::cout << std::endl;
    }

    return {bestMove, bestScore};
}

std::string formatScore(int score) {
    if (score > mateLower) {
        int movesToMate = (mateScore - score + 1) / 2;
        return "mate " + std::to_string(movesToMate);
    }
    else if (score < -mateLower) {
        int movesToMate = (mateScore + score) / 2;
        return "mate -" + std::to_string(movesToMate);
    }
    return "cp " + std::to_string(score);
}