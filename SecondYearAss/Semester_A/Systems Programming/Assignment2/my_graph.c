#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "my_mat.h"

int main(void)
{

    int mat[N][N];
    char c;
    int start, end;
    int need_update = TRUE;

    do
    {
        if (scanf("%c", &c) != 1)
        {
            printf("Error: invalid input\n");
            return 1;
        }

        if (c == 'A')
        {

            // printf("get arrays data\n");
            // get the matrix data
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (scanf("%d", &mat[i][j]) != 1)
                    {
                        printf("Error: invalid input\n");
                        return 1;
                    }
                }
                // set need_update to TRUE, so that the next time isPathExists or printShortestPath is called, the floydWarshall function will be called
                need_update = TRUE;
            }
        }
        else if (c == 'B')
        {
            // printf("check if there is a path from i to j\n");
            // check if there is a path from i to j
            scanf("%d %d", &start, &end);
            if (isPathExists(mat, start, end, need_update) == TRUE)
            {
                printf("True\n");

                // printf("There is a path from %d to %d\n", start, end);
            }
            else
            {
                printf("False\n");
                // printf("There is no path from %d to %d\n", start, end);
            }
            need_update = FALSE;
        }
        else if (c == 'C')
        {
            // printf("print the shortest path from i to j\n");
            // print the shortest path from i to j
            scanf("%d %d", &start, &end);
            printShortestPath(mat, start, end, need_update);
            need_update = FALSE;
        }
    } while (c != 'D' && c != EOF);
    return 0;
}