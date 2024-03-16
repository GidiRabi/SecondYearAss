#include "my_mat.h"
#include <stdio.h>

#define ITEM_NAME_MAX_LEN 20
#define MAX_KG 20
#define NUM_OF_ITEMS 5
/**
 * return the max of two numbers
 * @param a the first number
 * @param b the second number
 */
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/**
 * Solve the integer knapsack problem
 * @param weights the weights of the items
 * @param values the values of the items
 * @param selected_bool the selected items in boolean values - 1 for selected, 0 for not selected
 * @return the maximum value
 *
 */
int knapSack(int weights[], int values[], int selected_bool[])
{
    // Initialize the selected items array to false
    for (int i = 0; i < NUM_OF_ITEMS; i++)
    {
        selected_bool[i] = FALSE;
    }

    // Create a dp array
    int dp[NUM_OF_ITEMS + 1][MAX_KG + 1];

    // i is the number of items, B is the maximum weight
    int i, B;

    // Initialize the dp array
    for (i = 0; i <= NUM_OF_ITEMS; i++)
    {
        for (B = 0; B <= MAX_KG; B++)
        {
            // Base case - if there are no items or the maximum weight is 0, the value is 0
            if (i == 0 || B == 0)
            {
                dp[i][B] = 0;
            }
            else if (weights[i - 1] <= B) // If the weight of the item is less than the maximum weight - we can choose to take it or not
            {
                // Choose the maximum value between taking the item and not taking the item
                dp[i][B] = max(values[i - 1] + dp[i - 1][B - weights[i - 1]], dp[i - 1][B]);
            }
            else // If the weight of the item is more than the maximum weight - we can't take it
            {
                dp[i][B] = dp[i - 1][B];
            }
        }
    }

    // Find the selected items - we start from the last item and go back
    i = NUM_OF_ITEMS;
    B = MAX_KG;
    while (i > 0 && B > 0) // If we haven't reached the first item and the maximum weight is not 0
    {
        if (dp[i][B] != dp[i - 1][B]) // If the value is different from the value of the previous item - it means we took the item
        {
            selected_bool[i - 1] = TRUE; // Mark the item as selected
            B -= weights[i - 1];         // Subtract the weight of the item from the maximum weight
        }
        i--;
    }
    // the maximum value is in the last cell of the dp array
    return dp[NUM_OF_ITEMS][MAX_KG];
}

int main(void)
{
    char itemNames[NUM_OF_ITEMS][ITEM_NAME_MAX_LEN + 1];
    int values[NUM_OF_ITEMS];
    int weights[NUM_OF_ITEMS];
    int selected_bool[NUM_OF_ITEMS];

    for (int i = 0; i < NUM_OF_ITEMS; i++)
    {
        if (scanf("%s %d %d", itemNames[i], &values[i], &weights[i]) != 3)
        {
            printf("Invalid input\n");
            return 1;
        }
    }

    int max_value = knapSack(weights, values, selected_bool);
    printf("Maximum profit: %d\n", max_value);
    printf("Selected items: ");

    char *last = NULL;
    for (int i = 0; i < NUM_OF_ITEMS; i++)
    {

        if (selected_bool[i])
        {
            if (last != NULL)
            {
                printf("%s ", last);
            }
            last = itemNames[i];
        }
    }
    printf("%s", last);
    return 0;
}