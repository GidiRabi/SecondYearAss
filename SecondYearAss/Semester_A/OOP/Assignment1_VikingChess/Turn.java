import java.util.ArrayList;
import java.util.List;

/**
 * A turn in the game.<br>
 * A turn is defined by a piece, a starting position, an ending position, and a list of eaten pawns.
 */
public class Turn {
    /**
     * the piece that was moved in the turn
     */
    private final ConcretePiece piece;
    /**
     * the starting position of the piece
     */
    private final Position from;
    /**
     * the ending position of the piece
     */
    private final Position to;
    /**
     * the list of eaten pawns in the turn (max 4)
     */
    private final List<Pawn> eatenPawn;

    public Turn(ConcretePiece piece, Position from, Position to) {
        this.piece = piece;
        this.from = from;
        this.to = to;
        this.eatenPawn = new ArrayList<>(4); // max 4 pawns can be eaten in a turn
    }

    public ConcretePiece getPiece() {
        return piece;
    }

    public Position getFrom() {
        return from;
    }

    public Position getTo() {
        return to;
    }

    public List<Pawn> getEatenPawn() {
        return eatenPawn;
    }

    public void addEatenPawn(Pawn pawn) {
        this.eatenPawn.add(pawn);
    }
}
