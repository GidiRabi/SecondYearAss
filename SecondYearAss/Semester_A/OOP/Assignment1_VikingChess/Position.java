/**
 * A position on the board.<br>
 * A position is defined by a column and a row.<br>
 * (column, row)
 *
 * @param col the column of the position
 * @param row the row of the position
 */
public record Position(int col, int row) {
    /**
     * @param col the column of the position
     * @param row the row of the position
     */
    public Position {
    }

    /**
     * @return the column of the position
     */
    @Override
    public int col() {
        return col;
    }


    /**
     * @return the row of the position
     */
    @Override
    public int row() {
        return row;
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Position position = (Position) o;
        return col == position.col && row == position.row;
    }

    @Override
    public String toString() {
        return "(" + col + ", " + row + ")";
    }

    public int distance(Position next) {
        return Math.abs(this.col - next.col) + Math.abs(this.row - next.row);
    }
}
