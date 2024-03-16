
public class ConcretePlayer implements Player {
    private final boolean isPlayerOne;
    private int wins;

    public ConcretePlayer(boolean isPlayerOne) {
        this.isPlayerOne = isPlayerOne;
        this.wins = 0;
    }

    @Override
    public boolean isPlayerOne() {
        return isPlayerOne;
    }

    @Override
    public int getWins() {
        return wins;
    }

    public void addWin() {
        this.wins++;
    }
}
