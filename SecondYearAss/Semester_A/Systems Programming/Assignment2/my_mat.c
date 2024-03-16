#include <stdio.h>
#include <math.h>
#include "my_mat.h"

int dp[N][N];
int next[N][N];
int need_update = TRUE;

void setup(int mat[N][N])
{
    // init dp and add the length to next
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (i == j)
            {
                dp[i][j] = 0;
                next[i][j] = -1;
                continue;
            }
            dp[i][j] = mat[i][j];
            if (mat[i][j] != 0) // Update next matrix only if there is a path
                next[i][j] = j;
            else
                next[i][j] = -1; // No path initially
        }
    }
}

void floydWarshall(int mat[N][N])
{
    setup(mat);

    for (int k = 0; k < N; ++k)
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (i == j)
                    continue;

                if (dp[i][k] == 0 || dp[k][j] == 0)
                {
                    // there is no path from i to k or from k to j
                    continue;
                }

                // if there is no path from i to j or if the path from i to k and k to j is shorter than the current path
                if (dp[i][j] == 0 || dp[i][k] + dp[k][j] < dp[i][j])
                {
                    dp[i][j] = dp[i][k] + dp[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // no need to handle negative cycles
    //    return dp;
}

int isPathExists(int mat[N][N], int start, int end, int need_update)
{
    if (need_update == TRUE)
    {
        floydWarshall(mat);
    }
    return dp[start][end] != 0 ? TRUE : FALSE;
}

void printShortestPath(int mat[N][N], int start, int end, int need_update)
{
    if (need_update == TRUE)
    {
        floydWarshall(mat);
    }

    // not path
    if (isPathExists(mat, start, end, FALSE) == FALSE || (start == end && mat[start][end] == 0))
    {
        puts("-1");
        return;
    }

    printf("%d\n", dp[start][end]);

    // int at = start;
    // printf("%d", at); // print the start node

    // while (at != end)
    // {
    //     at = next[at][end];
    //     printf(" -> %d", at);
    // }

    // printf("\n");
}