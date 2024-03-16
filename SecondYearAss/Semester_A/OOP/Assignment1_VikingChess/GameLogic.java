import java.util.*;

public class GameLogic implements PlayableLogic {

    public static final int BOARD_SIZE = 11;

    /**
     * The pieces of the game. <br>
     * from index 0 to 12 - defender pieces. (king is at index 6). <br>
     * from index 13 to 36 - attacker pawns.
     */
    private final ConcretePiece[] gamePieces;
    /**
     * The position of the king. <br>
     * Helps us to check if the game is finished.
     */
    private Position kingPosition;
    /**
     * The board of the game. <br>
     * The board is a 2D array of ConcretePiece. <br>
     */
    private final ConcretePiece[][] board;

    /**
     * The turn history of the game. <br>
     * It will be used to undo moves.
     */
    private final Stack<Turn> turnsHistory;

    /**
     * The tracker of each cell in the board. <br>
     * Its tracks the pieces that were in each cell. <br>
     * The key is the position of the cell, and the value is a map of the pieces that were in the cell. <br>
     * the map key is the piece, and the value is the number of times that the piece was in the cell. <br>
     */
    private final Map<Position, Map<ConcretePiece, Integer>> boardStepTracer;

    /**
     * The first player of the game.
     */
    private final ConcretePlayer firstPlayer;

    /**
     * The second player of the game.
     */
    private final ConcretePlayer secondPlayer;

    /**
     * true if it's the second player turn, false otherwise
     */
    private boolean secondPlayerTurn;

    /**
     * true if the game is finished, false otherwise
     */
    private boolean isGameFinished;

    /**
     * indicates how many pawns the second player has left
     * if the second player has 0 pawns left, the first player won. <br>
     * <p>
     * (the first player cant have 0 pawns left because the king cant be eaten)
     */
    private int playerTwoRemainingPawns;

    public GameLogic() {
        // initialize the board, the tracker, the turns history, the pieces, the players, and the king position
        board = new ConcretePiece[BOARD_SIZE][BOARD_SIZE];
        boardStepTracer = new HashMap<>();
        turnsHistory = new Stack<>();
        gamePieces = new ConcretePiece[37]; // 12 defender pawns + 1 king + 24 attacker pawns
        isGameFinished = false;
        playerTwoRemainingPawns = 24;

        // create the players
        firstPlayer = new ConcretePlayer(true);
        secondPlayer = new ConcretePlayer(false);
        secondPlayerTurn = true;


        // create the pieces and place them on the board
        firstPlayerPieces();
        secondPlayerPieces();
    }


    /**
     * helper method to put a pawn on the board. <br>
     * will be used in the constructor. <br>
     * will update the board, the tracker, and the pieces moves list.
     *
     * @param pieceNum        the number of the piece
     * @param initialPosition the initial position of the piece
     * @param player          the player that the piece belongs to
     */
    private void putPawnOnBoard(int pieceNum, Position initialPosition, Player player) {
        Pawn newPawn = new Pawn(player, pieceNum, initialPosition);
        board[initialPosition.row()][initialPosition.col()] = newPawn;
        if (player.isPlayerOne()) {
            gamePieces[pieceNum - 1] = newPawn;
        } else {
            gamePieces[pieceNum + 12] = newPawn;
        }
        boardStepTracer.put(initialPosition, new HashMap<>(Map.of(newPawn, 1)));
    }

    /**
     * helper method to initialize the pieces of the first player. <br>
     * will be used in the constructor. <br>
     * will update the board, the tracker, and the pieces moves list.
     */
    private void firstPlayerPieces() {
        // create the pieces for player one:

        // create the king
        kingPosition = new Position(5, 5);
        King king = new King(firstPlayer, 7, kingPosition);
        gamePieces[6] = king;
        boardStepTracer.put(kingPosition, new HashMap<>(Map.of(king, 1)));
        board[5][5] = king;

        // create the defender pawns
        // row 3
        putPawnOnBoard(1, new Position(5, 3), firstPlayer);
        // row 4
        putPawnOnBoard(2, new Position(4, 4), firstPlayer);
        putPawnOnBoard(3, new Position(5, 4), firstPlayer);
        putPawnOnBoard(4, new Position(6, 4), firstPlayer);

        // row 5
        putPawnOnBoard(5, new Position(3, 5), firstPlayer);
        putPawnOnBoard(6, new Position(4, 5), firstPlayer);
        putPawnOnBoard(8, new Position(6, 5), firstPlayer);
        putPawnOnBoard(9, new Position(7, 5), firstPlayer);

        // row 6
        putPawnOnBoard(10, new Position(4, 6), firstPlayer);
        putPawnOnBoard(11, new Position(5, 6), firstPlayer);
        putPawnOnBoard(12, new Position(6, 6), firstPlayer);

        // row 7
        putPawnOnBoard(13, new Position(5, 7), firstPlayer);
    }

    /**
     * helper method to initialize the pieces of the second player. <br>
     * will be used in the constructor. <br>
     * will update the board, the tracker, and the pieces moves list.
     */
    private void secondPlayerPieces() {
        //  row 0
        putPawnOnBoard(1, new Position(3, 0), secondPlayer);
        putPawnOnBoard(2, new Position(4, 0), secondPlayer);
        putPawnOnBoard(3, new Position(5, 0), secondPlayer);
        putPawnOnBoard(4, new Position(6, 0), secondPlayer);
        putPawnOnBoard(5, new Position(7, 0), secondPlayer);

        // row 1
        putPawnOnBoard(6, new Position(5, 1), secondPlayer);

        // row 3
        putPawnOnBoard(7, new Position(0, 3), secondPlayer);
        putPawnOnBoard(8, new Position(10, 3), secondPlayer);

        // row 4
        putPawnOnBoard(9, new Position(0, 4), secondPlayer);
        putPawnOnBoard(10, new Position(10, 4), secondPlayer);

        // row 5
        putPawnOnBoard(11, new Position(0, 5), secondPlayer);
        putPawnOnBoard(12, new Position(1, 5), secondPlayer);
        putPawnOnBoard(13, new Position(9, 5), secondPlayer);
        putPawnOnBoard(14, new Position(10, 5), secondPlayer);

        // row 6
        putPawnOnBoard(15, new Position(0, 6), secondPlayer);
        putPawnOnBoard(16, new Position(10, 6), secondPlayer);

        // row 7
        putPawnOnBoard(17, new Position(0, 7), secondPlayer);
        putPawnOnBoard(18, new Position(10, 7), secondPlayer);

        // row 9
        putPawnOnBoard(19, new Position(5, 9), secondPlayer);

        // row 10
        putPawnOnBoard(20, new Position(3, 10), secondPlayer);
        putPawnOnBoard(21, new Position(4, 10), secondPlayer);
        putPawnOnBoard(22, new Position(5, 10), secondPlayer);
        putPawnOnBoard(23, new Position(6, 10), secondPlayer);
        putPawnOnBoard(24, new Position(7, 10), secondPlayer);
    }

    /**
     * Try to move a piece from position a to position b.
     * if the move is valid, update the board, the tracker, the turns history, the piece moves list, and the turn owner.
     *
     * @param a The starting position of the piece.
     * @param b The destination position for the piece.
     * @return true if the move was successful, false otherwise
     */
    @Override
    public boolean move(Position a, Position b) {
        // check if a and b are in the board
        checkIfPositionIsInBoard(a);
        checkIfPositionIsInBoard(b);

        ConcretePiece piece = board[a.row()][a.col()];
        Player currentPlayer = isSecondPlayerTurn() ? secondPlayer : firstPlayer;
        // check if there is a piece at position a
        if (piece == null)
            return false; // there is no piece at position a

        // check if the piece at position a belongs to the player who is playing
        if (piece.getOwner() != currentPlayer)
            return false; // the piece at position a does not belong to the player who is playing

        // if he tries to move to the same position
        if (a.equals(b))
            return false;


        // check if the move is valid
        // check if he tries moves vertical or horizontal
        if (!(a.col() == b.col() || a.row() == b.row()))
            return false; // he tries to move diagonal

        //Only the King is allowed into the corner squares
        if (
                ((b.col() == 0 && b.row() == 0) || (b.col() == 0 && b.row() == 10) || (b.col() == 10 && b.row() == 0) || (b.col() == 10 && b.row() == 10)) // if he tries to move into a corner square
                        && !(piece instanceof King) // and the piece is not the king
        )
            return false; // he tries to move a piece that is not the king into a corner square

        // horizontal move
        // check if there is a piece in the way if he tries to move right
        for (int i = a.row() + 1; i <= b.row(); i++) {
            if (board[i][a.col()] != null)
                return false; // there is a piece in the way, so the move is invalid
        }
        // check if there is a piece in the way if he tries to move left
        for (int i = a.row() - 1; i >= b.row(); i--) {
            if (board[i][a.col()] != null)
                return false; // there is a piece in the way, so the move is invalid
        }

        // vertical move
        // check if there is a piece in the way if he tries to move down
        for (int i = a.col() + 1; i <= b.col(); i++) {
            if (board[a.row()][i] != null)
                return false; // there is a piece in the way, so the move is invalid
        }
        // check if there is a piece in the way if he tries to move up
        for (int i = a.col() - 1; i >= b.col(); i--) {
            if (board[a.row()][i] != null)
                return false; // there is a piece in the way, so the move is invalid
        }


        // update the board
        board[a.row()][a.col()] = null;
        board[b.row()][b.col()] = piece;

        // update the tracker - add the piece to the tracker - track how many times the piece was in the cell
        if (boardStepTracer.containsKey(b)) { // if the position was already in the tracker
            // get the map of the pieces that were in the cell
            Map<ConcretePiece, Integer> pieceToNumOfStep = boardStepTracer.get(b);
            // check if the piece was already in the cell
            if (pieceToNumOfStep.containsKey(piece)) {
                pieceToNumOfStep.put(piece, pieceToNumOfStep.get(piece) + 1); // add 1 to the number of times that the piece was in the cell
            } else { // if the piece was not in the cell, add the piece to the map, and set the number of times that the piece was in the cell to 1
                pieceToNumOfStep.put(piece, 1);
            }
        } else { // this is the first time that a piece is in the cell
            Map<ConcretePiece, Integer> pieceToNumOfStep = new HashMap<>();
            // add the piece to the set of pieces that were in the cell, and set the number of times that the piece was in the cell to 1
            pieceToNumOfStep.put(piece, 1);
            boardStepTracer.put(b, pieceToNumOfStep);
        }

        // update the turns history
        turnsHistory.push(new Turn(piece, a, b));

        // update the piece moves list
        piece.addMove(b);


        // update the king position
        if (piece instanceof King)
            kingPosition = b;

        // check if the piece can eat
        checkEat(b);

        updateGameStats();
        // check if the game is finished
        // if the game is finished, print the game stats
        if (isGameFinished)
            printGameStats(!secondPlayerTurn); // print the game stats for the winner, the winner is the player who is playing now

        // update the turn owner
        secondPlayerTurn = !secondPlayerTurn;
        return true; // the move was successful
    }


    /**
     * check if the piece at the given position can eat. <br>
     * if it can, eat the enemy pawn and update the board, and the piece data.
     *
     * @param position the position of the piece that we want to check if it can eat
     */
    private void checkEat(Position position) {
        // only pawns can eat, so if the piece at the given position is not a pawn, return
        if (!(board[position.row()][position.col()] instanceof Pawn currPawn))
            return;
        /*
        we add a pawn from the same player to the board corners, it's help us to don't have to handle the edge cases
        that the corner acts as the second captured pawn
         */
        //add a pawn from the same player to the board corners (the index is 0 because we don't care about the index)
        board[0][0] = new Pawn(currPawn.getOwner(), 0, null);
        board[0][10] = new Pawn(currPawn.getOwner(), 0, null);
        board[10][0] = new Pawn(currPawn.getOwner(), 0, null);
        board[10][10] = new Pawn(currPawn.getOwner(), 0, null);

        // if there is an enemy pawn up, and it's not the king (the king can't be eaten)
        if (isThereEnemyPawnDir(position, 0, -1) && !kingPosition.equals(new Position(position.row() - 1, position.col()))) {
            if (position.row() - 1 == 0) { // if the pawn is in the first row
                // check if the enemy pawn is the second captured pawn
                if (board[position.row() - 1][position.col()].getOwner() == secondPlayer) {
                    // the second player lost a pawn
                    playerTwoRemainingPawns--;
                }
                // eat the enemy pawn
                currPawn.eat();
                // update the turn history
                turnsHistory.peek().addEatenPawn((Pawn) board[position.row() - 1][position.col()]);
                // remove the enemy pawn from the board
                board[position.row() - 1][position.col()] = null;

            } else { // we need another pawn from the same player above the enemy pawn, so check if there is a pawn from the same player above the enemy pawn
                if (board[position.row() - 2][position.col()] instanceof Pawn && board[position.row() - 2][position.col()].getOwner() == currPawn.getOwner()) {
                    // check if the enemy pawn is the second captured pawn
                    if (board[position.row() - 1][position.col()].getOwner() == secondPlayer) {
                        // the second player lost a pawn
                        playerTwoRemainingPawns--;
                    }
                    // eat the enemy pawn
                    currPawn.eat();
                    // update the turn history
                    turnsHistory.peek().addEatenPawn((Pawn) board[position.row() - 1][position.col()]);
                    // remove the enemy pawn from the board
                    board[position.row() - 1][position.col()] = null;
                }
            }
        }
        // if there is an enemy pawn down
        if (isThereEnemyPawnDir(position, 0, 1) && !kingPosition.equals(new Position(position.row() + 1, position.col()))) {
            if (position.row() + 1 == 10) { // if the pawn is in the last row
                // check if the enemy pawn is the second captured pawn
                if (board[position.row() + 1][position.col()].getOwner() == secondPlayer) {
                    // the second player lost a pawn
                    playerTwoRemainingPawns--;
                }
                // eat the enemy pawn
                currPawn.eat();
                // update the turn history
                turnsHistory.peek().addEatenPawn((Pawn) board[position.row() + 1][position.col()]);
                // remove the enemy pawn from the board
                board[position.row() + 1][position.col()] = null;
            } else { // check if there is a pawn from the same player below the enemy pawn
                if (board[position.row() + 2][position.col()] instanceof Pawn && board[position.row() + 2][position.col()].getOwner() == currPawn.getOwner()) {
                    // check if the enemy pawn is the second captured pawn
                    if (board[position.row() + 1][position.col()].getOwner() == secondPlayer) {
                        // the second player lost a pawn
                        playerTwoRemainingPawns--;
                    }
                    // eat the enemy pawn
                    currPawn.eat();
                    // update the turn history
                    turnsHistory.peek().addEatenPawn((Pawn) board[position.row() + 1][position.col()]);
                    // remove the enemy pawn from the board
                    board[position.row() + 1][position.col()] = null;
                }
            }
        }

        // if there is an enemy pawn left
        if (isThereEnemyPawnDir(position, -1, 0) && !kingPosition.equals(new Position(position.row(), position.col() - 1))) {
            if (position.col() - 1 == 0) { // if the pawn is in the first column
                if (board[position.row()][position.col() - 1].getOwner() == secondPlayer) {
                    // the second player lost a pawn
                    playerTwoRemainingPawns--;
                }
                // eat the enemy pawn
                currPawn.eat();
                // update the turn history
                turnsHistory.peek().addEatenPawn((Pawn) board[position.row()][position.col() - 1]);
                // remove the enemy pawn from the board
                board[position.row()][position.col() - 1] = null;
            } else { // check if there is a pawn from the same player left to the enemy pawn
                if (board[position.row()][position.col() - 2] instanceof Pawn && board[position.row()][position.col() - 2].getOwner() == currPawn.getOwner()) {
                    if (board[position.row()][position.col() - 1].getOwner() == secondPlayer) {
                        // the second player lost a pawn
                        playerTwoRemainingPawns--;
                    }
                    // eat the enemy pawn
                    currPawn.eat();
                    // update the turn history
                    turnsHistory.peek().addEatenPawn((Pawn) board[position.row()][position.col() - 1]);
                    // remove the enemy pawn from the board
                    board[position.row()][position.col() - 1] = null;
                }
            }
        }
        if (isThereEnemyPawnDir(position, 1, 0) && !kingPosition.equals(new Position(position.row(), position.col() + 1))) {
            if (position.col() + 1 == 10) { // if the pawn is in the last column
                if (board[position.row()][position.col() + 1].getOwner() == secondPlayer) {
                    // the second player lost a pawn
                    playerTwoRemainingPawns--;
                }
                // eat the enemy pawn
                currPawn.eat();
                // update the turn history
                turnsHistory.peek().addEatenPawn((Pawn) board[position.row()][position.col() + 1]);
                // remove the enemy pawn from the board
                board[position.row()][position.col() + 1] = null;
            } else { // check if there is a pawn from the same player right to the enemy pawn
                if (board[position.row()][position.col() + 2] instanceof Pawn && board[position.row()][position.col() + 2].getOwner() == currPawn.getOwner()) {
                    if (board[position.row()][position.col() + 1].getOwner() == secondPlayer) {
                        // the second player lost a pawn
                        playerTwoRemainingPawns--;
                    }
                    // eat the enemy pawn
                    currPawn.eat();
                    // update the turn history
                    turnsHistory.peek().addEatenPawn((Pawn) board[position.row()][position.col() + 1]);
                    // remove the enemy pawn from the board
                    board[position.row()][position.col() + 1] = null;
                }
            }
        }

        // remove the added pawn from the board corners
        board[0][0] = null;
        board[0][10] = null;
        board[10][0] = null;
        board[10][10] = null;
    }


    /**
     * Get the piece at a given position.
     *
     * @param position The position to get the piece from.
     * @return The piece at the given position, or null if there is no piece at the given position.
     */
    @Override
    public Piece getPieceAtPosition(Position position) {
        checkIfPositionIsInBoard(position);
        return board[position.row()][position.col()];
    }

    /**
     * @return the first player
     */
    @Override
    public Player getFirstPlayer() {
        return firstPlayer;
    }

    /**
     * @return the second player
     */
    @Override
    public Player getSecondPlayer() {
        return secondPlayer;
    }


    private void updateGameStats() {
        // check if the king is in one of the corners
        if (board[0][0] instanceof King || board[0][10] instanceof King || board[10][0] instanceof King || board[10][10] instanceof King) {
            firstPlayer.addWin();
            this.isGameFinished = true;
            return;
        }

        // check if all the pawns of the second player were eaten
        if (playerTwoRemainingPawns == 0) {
            firstPlayer.addWin();
            this.isGameFinished = true;
            return;
        }

        // check if the king is surrounded by pawns
        // if the king is in the edge of the board, check if he is surrounded by 3 pawns
        if (kingPosition.col() == 0 && // if the king is in the first column
                isThereEnemyPawnDir(kingPosition, 0, -1) &&// up
                isThereEnemyPawnDir(kingPosition, 1, 0) && // left
                isThereEnemyPawnDir(kingPosition, 0, 1) // down
        ) {
            secondPlayer.addWin(); // the second player won
            this.isGameFinished = true;
        } else if (kingPosition.col() == 10 && // if the king is in the last column
                isThereEnemyPawnDir(kingPosition, 0, -1) &&// up
                isThereEnemyPawnDir(kingPosition, -1, 0) && // right
                isThereEnemyPawnDir(kingPosition, 0, 1) // down
        ) {
            secondPlayer.addWin(); // the second player won
            this.isGameFinished = true;
        } else if (kingPosition.row() == 0 && // if the king is in the first row
                isThereEnemyPawnDir(kingPosition, 1, 0) &&// left
                isThereEnemyPawnDir(kingPosition, 0, 1) && // down
                isThereEnemyPawnDir(kingPosition, -1, 0) // right
        ) {
            secondPlayer.addWin(); // the second player won

            this.isGameFinished = true;
        } else if (kingPosition.row() == 10 && // if the king is in the last row
                isThereEnemyPawnDir(kingPosition, 1, 0) &&// left
                isThereEnemyPawnDir(kingPosition, 0, -1) && // up
                isThereEnemyPawnDir(kingPosition, -1, 0) // right
        ) {
            secondPlayer.addWin(); // the second player won
            this.isGameFinished = true;
        } else if ( // if the king is not in the edge of the board, check if he is surrounded by 4 pawns
                isThereEnemyPawnDir(kingPosition, 1, 0) &&// left
                        isThereEnemyPawnDir(kingPosition, 0, -1) && // up
                        isThereEnemyPawnDir(kingPosition, -1, 0) && // right
                        isThereEnemyPawnDir(kingPosition, 0, 1) // down
        ) {
            secondPlayer.addWin(); // the second player won
            this.isGameFinished = true;
        }
        // no one won yet - do nothing
    }

    /**
     * check if the game is finished
     *
     * @return true if the game is finished, false otherwise
     */
    @Override
    public boolean isGameFinished() {
        return isGameFinished;
    }

    /**
     * helper method for check if there is an enemy pawn in the given direction.
     *
     * @param position the position of the piece that we want to check if there is an enemy pawn in the given direction
     * @param dx       the x direction. 1 for right, -1 for left, 0 for same column
     * @param dy       the y direction. 1 for down, -1 for up, 0 for same row
     * @return true if there is an enemy pawn in the given direction, false otherwise
     */
    private boolean isThereEnemyPawnDir(Position position, int dx, int dy) {
        if (position.col() + dx < 0 || position.col() + dx > 10 || position.row() + dy < 0 || position.row() + dy > 10)
            return false;
        return board[position.row() + dy][position.col() + dx] instanceof Pawn &&
                board[position.row() + dy][position.col() + dx].getOwner() != board[position.row()][position.col()].getOwner();
    }

    /**
     * check if it is the second player's turn
     *
     * @return true if it is the second player's turn, false otherwise
     */
    @Override
    public boolean isSecondPlayerTurn() {
        return secondPlayerTurn;
    }

    /**
     * reset the game to the initial state.
     * including the board and the data of the players
     */
    @Override
    public void reset() {
        // set the player turn to the second player
        secondPlayerTurn = true;

        // reset the board
        for (Piece[] row : board)
            Arrays.fill(row, null);

        // put the pieces on the board
        firstPlayerPieces();
        secondPlayerPieces();

        // clear the turns history
        turnsHistory.clear();
        // clear the tracker
        boardStepTracer.clear();
    }

    /**
     * undo the last move, restore the state of the game to the state before the last move, and the turn's owner, and update all the data in all the objects.
     */
    @Override
    public void undoLastMove() {

        // check if there is a move to undo
        if (turnsHistory.isEmpty())
            return;

        // get the last turn
        Turn lastTurn = turnsHistory.pop();
        // get the piece of the last turn
        ConcretePiece piece = lastTurn.getPiece();
        // return the piece to the last position
        board[lastTurn.getFrom().row()][lastTurn.getFrom().col()] = piece;
        board[lastTurn.getTo().row()][lastTurn.getTo().col()] = null;

        // update the piece moves list
        piece.getMoves().remove(piece.getMoves().size() - 1);

        //update the piece number of eats
        piece.setNumberOfEats(piece.getNumberOfEats() - lastTurn.getEatenPawn().size());

        // return the eaten pawns to the board
        for (Pawn pawn : lastTurn.getEatenPawn()) {
            // update the number of pawns that the second player has left
            if (pawn.getOwner() == secondPlayer) {
                playerTwoRemainingPawns++;
            }
            // get the last position that the pawn was in
            Position pawnLastPosition = pawn.getMoves().get(pawn.getMoves().size() - 1);
            board[pawnLastPosition.row()][pawnLastPosition.col()] = pawn;
        }

        // update the tracker - remove the piece from the set of pieces that were in the cell
        Map<ConcretePiece, Integer> pieceToNumOfStep = boardStepTracer.get(lastTurn.getTo());

        if (pieceToNumOfStep.get(piece) == 1) { // if the piece was in the cell only one time
            pieceToNumOfStep.remove(piece); // remove the piece from the map of pieces that were in the cell
            if (pieceToNumOfStep.isEmpty()) // if the map is empty, remove it from the tracker
                boardStepTracer.remove(lastTurn.getTo()); // remove the map from the tracker -> there is no pieces that were in the cell
        } else { // if the piece was in the cell more than one time
            pieceToNumOfStep.put(piece, pieceToNumOfStep.get(piece) - 1); // remove 1 from the number of times that the piece was in the cell
        }

        // update the king position
        if (piece instanceof King)
            kingPosition = lastTurn.getFrom();


        // update the turn owner
        secondPlayerTurn = !secondPlayerTurn;
    }

    /**
     * @return the size of the board
     */
    @Override
    public int getBoardSize() {
        return BOARD_SIZE;
    }

    /**
     * print the game statistics according to the assignment instructions
     */
    public void printGameStats(boolean isPlayerOneWon) {
        printMovesHistory(isPlayerOneWon);
        printDivider();

        printEatsCount(isPlayerOneWon);
        printDivider();

        printDistances(isPlayerOneWon);
        printDivider();

        printCellsHistory();
        printDivider();

    }

    /**
     * print the moves history according to the assignment instructions. <br>
     * The instructions are:
     * after the game is over, print the moves history of each piece, in ascending order of the number of moves. <br>
     * the order of the pieces is: <br>
     * first the pieces of the winner, then the pieces of the loser. <br>
     * in these sides, the pieces are sorted by the number of moves, and then by there number.
     *
     * @param isPlayerOneWon true if the first player won, false otherwise
     */
    private void printMovesHistory(boolean isPlayerOneWon) {
        // split the pieces to two arrays - one for each player
        ConcretePiece[] playerOnePieces = Arrays.copyOfRange(gamePieces, 0, 13);
        ConcretePiece[] playerTwoPieces = Arrays.copyOfRange(gamePieces, 13, 37);

        // sort the pieces according to the assignment instructions
        Comparator<ConcretePiece> comp = (piece1, piece2) -> {
            // sort by the number of moves
            if (piece1.getMoves().size() != piece2.getMoves().size())
                return piece1.getMoves().size() - piece2.getMoves().size();

            // the number of moves is the same, so sort by the number of the pieces
            return piece1.getPieceNum() - piece2.getPieceNum();
        };
        // sort the pieces of each player separately
        Arrays.sort(playerOnePieces, comp);
        Arrays.sort(playerTwoPieces, comp);

        // print the sorted pieces
        // first the pieces of the winner, then the pieces of the loser
        ConcretePiece[] winnerPieces, loserPieces;
        if (isPlayerOneWon) {
            winnerPieces = playerOnePieces;
            loserPieces = playerTwoPieces;
        } else {
            winnerPieces = playerTwoPieces;
            loserPieces = playerOnePieces;
        }
        for (ConcretePiece piece : winnerPieces) {
            if (piece.getMoves() != null && piece.getMoves().size() > 1) {
                System.out.println(piece.getName() + ": " + piece.getMoves());
            }
        }
        for (ConcretePiece piece : loserPieces) {
            if (piece.getMoves() != null && piece.getMoves().size() > 1) {
                System.out.println(piece.getName() + ": " + piece.getMoves());
            }
        }
    }

    /**
     * print the eats count according to the assignment instructions. <br>
     * The instructions are:
     * print each piece sorted in descending order by the number of eaten pawns. <br>
     * if two pieces have the same number of eaten pawns, sort them by there number in ascending order. <br>
     * if the pieces have the same number of eaten pawns and the same number, sort them by the winner (first the winner, then the loser).
     *
     * @param isPlayerOneWon true if the first player won, false otherwise
     */
    private void printEatsCount(boolean isPlayerOneWon) {
        // filter the pieces that not eaten any pawn
        ConcretePiece[] filtered = Arrays.stream(gamePieces).filter(piece -> piece.getNumberOfEats() > 0).toArray(ConcretePiece[]::new);
        // sort the pieces according to the assignment instructions
        Arrays.sort(filtered, (piece1, piece2) -> {
            // sort by the number of eaten pawns in descending order
            if (piece1.getNumberOfEats() != piece2.getNumberOfEats())
                return piece2.getNumberOfEats() - piece1.getNumberOfEats();
                // the number of eaten pawns is the same, so sort by there number in ascending order
            else if (piece1.getPieceNum() != piece2.getPieceNum())
                return piece1.getPieceNum() - piece2.getPieceNum();

            // the number of eaten pawns and the number of the pieces are the same, them by the winner (first the winner, then the loser)
            if (isPlayerOneWon)
                return piece1.getOwner().isPlayerOne() ? -1 : 1;
            else // the second player won
                return piece1.getOwner().isPlayerOne() ? 1 : -1;
        });

        // print the sorted pieces
        for (ConcretePiece piece : filtered) {
            System.out.println(piece.getName() + ": " + piece.getNumberOfEats() + " kills");
        }

    }

    /**
     * print the distances according to the assignment instructions. <br>
     * The instructions are:
     * print each piece that moved at least once, sorted in descending order by the distance that the piece has moved. <br>
     * if two pieces have the same distance, sort them by their number in ascending order. <br>
     * if the pieces have the same distance and the same number, sort them by the winner (first the winner, then the loser).
     * <br>
     *
     * @param isPlayerOneWon true if the first player won, false otherwise
     */
    private void printDistances(boolean isPlayerOneWon) {
        // filter the pieces that not moved
        ConcretePiece[] filtered = Arrays.stream(gamePieces).filter(piece -> piece.getMoves().size() > 1).toArray(ConcretePiece[]::new);
        // sort the pieces according to the assignment instructions
        Arrays.sort(filtered, (piece1, piece2) -> {
            // sort by the distance that the piece has moved in descending order
            if (piece1.getDistance() != piece2.getDistance())
                return piece2.getDistance() - piece1.getDistance();
                // the distance that the piece has moved is the same, so sort by there number in ascending order
            else if (piece1.getPieceNum() != piece2.getPieceNum())
                return piece1.getPieceNum() - piece2.getPieceNum();

            // the distance that the piece has moved and the number of the pieces are the same, them by the winner (first the winner, then the loser)
            if (isPlayerOneWon)
                return piece1.getOwner().isPlayerOne() ? -1 : 1;
            else // the second player won
                return piece1.getOwner().isPlayerOne() ? 1 : -1;
        });

        // print the sorted pieces
        for (ConcretePiece piece : filtered) {
            System.out.println(piece.getName() + ": " + piece.getDistance() + " squares");
        }
    }

    /**
     * print the cells history according to the assignment instructions. <br>
     * The instructions are:
     * print each cell that had more than one piece in it, in descending order of the number of different pieces that were in the cell. <br>
     * if two cells had the same number of different pieces, sort them by the x coordinate of the cell. <br>
     * if the x coordinate of the cell is the same, sort them by the y coordinate of the cell.
     */
    private void printCellsHistory() {
        // add the positions that had more than one piece in them to a list
        List<Position> positions = new ArrayList<>();
        for (Map.Entry<Position, Map<ConcretePiece, Integer>> entry : boardStepTracer.entrySet()) {
            if (entry.getValue().size() > 1)
                positions.add(entry.getKey());
        }

        // sort the positions according to the assignment instructions
        positions.sort((position1, position2) -> {
            // sort by the number of different pieces that were in the cell in descending order
            if (boardStepTracer.get(position1).size() != boardStepTracer.get(position2).size())
                return boardStepTracer.get(position2).size() - boardStepTracer.get(position1).size();

            // the number of different pieces that were in the cell is the same, so sort by the x coordinate of the cell
            if (position1.col() != position2.col())
                return position1.col() - position2.col();

            // the x coordinate of the cell is the same, so sort by the y coordinate of the cell
            return position1.row() - position2.row();
        });

        // print the sorted positions
        for (Position position : positions) {
            System.out.println(position.toString() + boardStepTracer.get(position).size() + " pieces");
        }
    }

    /**
     * print a divider line. the divider line is 75 '*' characters.
     */
    private void printDivider() {
        System.out.println("***************************************************************************");
    }

    /**
     * check if the given position is in the board. <br>
     * if it is not, throw an exception.
     *
     * @param position the position to check
     * @throws PositionOutOfBoardException if the position is not in the board
     */
    private void checkIfPositionIsInBoard(Position position) {
        if (position.row() < 0 || position.row() > 10 || position.col() < 0 || position.col() > 10)
            throw new PositionOutOfBoardException();
    }

    private static class PositionOutOfBoardException extends IllegalArgumentException {
        public PositionOutOfBoardException() {
            super("The position is not in the board");
        }
    }
}

