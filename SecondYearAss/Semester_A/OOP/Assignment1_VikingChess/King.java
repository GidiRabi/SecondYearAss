public class King extends ConcretePiece {
    private static final String type = "♔";

    public King(Player owner, int index, Position initialPosition) {
        super(owner, index, initialPosition);
    }

    @Override
    public String getType() {
        return type;
    }


    @Override
    public String getName() {
        return "K" + getPieceNum();
    }

    /**
     * The king can't eat, so always return 0.
     *
     * @return 0
     */
    @Override
    public int getNumberOfEats() {
        return 0;
    }

    /**
     * The king can't eat, so do nothing.
     */
    @Override
    public void setNumberOfEats(int ignored) {
        // do nothing
    }
}
