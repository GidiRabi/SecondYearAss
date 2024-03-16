public class Pawn extends ConcretePiece {
    private static final String PLAYER_ONE_TYPE = "♙";
    private static final String PLAYER_TWO_TYPE = "♟";

    private int numberOfEatenPawns;

    public Pawn(Player owner, int pieceNum, Position initialPosition) {
        super(owner, pieceNum, initialPosition);
        this.numberOfEatenPawns = 0;
    }


    @Override
    public String getType() {
        return getOwner().isPlayerOne() ? PLAYER_ONE_TYPE : PLAYER_TWO_TYPE;
    }

    /**
     * Increment the number of eaten pawns by 1.
     */
    public void eat() {
        this.numberOfEatenPawns++;
    }

    @Override
    public int getNumberOfEats() {
        return numberOfEatenPawns;
    }

    @Override
    public void setNumberOfEats(int i) {
        this.numberOfEatenPawns = i;
    }

    @Override
    public String getName() {
        String prefix = super.getOwner().isPlayerOne() ? "D" : "A";
        return prefix + super.getPieceNum();
    }
}
