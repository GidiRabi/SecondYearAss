#define N 10

#define TRUE 1
#define FALSE 0

void floydWarshall();
int isPathExists(int mat[N][N], int start, int end, int need_update);
void printShortestPath(int mat[N][N], int start, int end, int need_update);
