/*******************************************************
 * File: Lab2_MustafaJawish.c
 * Author: Mustafa Jawish
 * Course: CST8234 302 – C Programming
 * Purpose: Sudoku Solver.
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Function Prototypes */
int** createGrid(int size);
void inputSudokuGrid(int** grid, int size);
void printSudokuGrid(int** grid, int size);
bool isValid(int** grid, int size, int row, int col, int num, bool printErrors);
bool validateSudokuGrid(int** grid, int size);
bool solveSudoku(int** grid, int size);
bool backtrack(int** grid, int size);
void freeMemory(int** grid, int size);

/** Main Function */
int main() {
    int size;
    int** grid;
    int choice;

    printf("Enter the size of Sudoku grid (e.g., 9 for 9x9): ");
    scanf("%d", &size);

    /** Validate grid size */
    if (size % 3 != 0 || size < 9 || size > 16) {
        printf("Error: Invalid size. Please enter a valid Sudoku grid size (e.g., 9 or 16).\n");
        return 1;
    }

    /** Create the grid dynamically */
    grid = createGrid(size);
    if (grid == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    /** Menu System */
    while (1) {
        printf("\nMenu:\n");
        printf("1. Input Sudoku Puzzle\n");
        printf("2. Solve Sudoku Puzzle\n");
        printf("3. Display Sudoku Grid\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputSudokuGrid(grid, size);
                if (!validateSudokuGrid(grid, size)) {
                    printf("Error: The Sudoku puzzle is invalid.\n");
                }
                break;
            case 2:
                if (validateSudokuGrid(grid, size)) {
                    if (solveSudoku(grid, size)) {
                        printf("Sudoku puzzle solved successfully!\n");
                        printSudokuGrid(grid, size);
                    } else {
                        printf("No solution exists for this Sudoku puzzle.\n");
                    }
                } else {
                    printf("Error: The Sudoku puzzle is invalid and cannot be solved.\n");
                }
                break;
            case 3:
                printSudokuGrid(grid, size);
                break;
            case 4:
                freeMemory(grid, size);
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/** Function to dynamically allocate memory for the Sudoku grid */
int** createGrid(int size) {
    int** grid;
    int i, j; /** Declare variables at the top */

    grid = (int**)malloc(size * sizeof(int*));
    if (grid == NULL) return NULL;

    for (i = 0; i < size; i++) {
        grid[i] = (int*)malloc(size * sizeof(int));
        if (grid[i] == NULL) {
            /** Free previously allocated memory in case of failure */
            for (j = 0; j < i; j++) {
                free(grid[j]);
            }
            free(grid);
            return NULL;
        }
    }
    return grid;
}

/** Function to input the Sudoku grid from the user */
void inputSudokuGrid(int** grid, int size) {
    int i, count, input;
    char buffer[256]; /** Declare variables at the top */
    char* ptr;

    printf("Enter the Sudoku puzzle row by row, with 0 representing an empty cell:\n");
    for (i = 0; i < size; i++) {
        while (1) {
            count = 0;
            ptr = buffer; /** Initialize ptr here */

            /** Read the entire line */
            fgets(buffer, sizeof(buffer), stdin);

            while (*ptr != '\0') {
                /** Skip spaces */
                while (*ptr == ' ' || *ptr == '\t') {
                    ptr++;
                }

                /** Stop if end of line */
                if (*ptr == '\n' || *ptr == '\0') {
                    break;
                }

                /** Convert to integer */
                if (sscanf(ptr, "%d", &input) == 1) {
                    if (input < 0 || input > size) {
                        printf("Error: Invalid input. Please enter a number between 0 and %d.\n", size);
                        count = -1;
                        break;
                    }
                    grid[i][count] = input;
                    count++;
                }

                /** Move to the next number */
                while (*ptr != ' ' && *ptr != '\t' && *ptr != '\0') {
                    ptr++;
                }
            }

            /** Handle errors and re-entry */
            if (count == size) break; /** Valid input */
            if (count > size) {
                printf("Error: Row %d contains more numbers than the specified grid size (%dx%d).\n", i + 1, size, size);
            } else if (count != -1) {      
             }
        }
    }
}

/** Function to print the Sudoku grid */
void printSudokuGrid(int** grid, int size) {
    int i, j;

    printf("\nSudoku Grid:\n");
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            printf("%1d ", grid[i][j]); 
        }
        printf("\n");
    }
}

/** Function to check if a number can be placed in a specific cell */
bool isValid(int** grid, int size, int row, int col, int num, bool printErrors) {
    int i, j;
    int subgridSize = size / 3;
    int startRow = row - row % subgridSize;
    int startCol = col - col % subgridSize;

    /** Check row and column */
    for (i = 0; i < size; i++) {
        if (grid[row][i] == num) {
            if (printErrors) printf("Error: Duplicate value %d in row %d.\n", num, row + 1);
            return false;
        }
        if (grid[i][col] == num) {
            if (printErrors) printf("Error: Duplicate value %d in column %d.\n", num, col + 1);
            return false;
        }
    }

    /** Check subgrid */
    for (i = 0; i < subgridSize; i++) {
        for (j = 0; j < subgridSize; j++) {
            if (grid[startRow + i][startCol + j] == num) {
                if (printErrors) printf("Error: Duplicate value %d in subgrid (starting at row %d, col %d).\n", num, startRow + 1, startCol + 1);
                return false;
            }
        }
    }
    return true;
}

/** Function to validate the entire Sudoku grid */
bool validateSudokuGrid(int** grid, int size) {
    int i, j, num;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            num = grid[i][j];
            if (num != 0) {
                /** Temporarily set the cell to 0 to avoid self-comparison */
                grid[i][j] = 0;
                if (!isValid(grid, size, i, j, num, true)) {
                    grid[i][j] = num; /** Restore the original value */
                    return false;
                }
                grid[i][j] = num; /** Restore the original value */
            }
        }
    }
    return true;
}

/** Function to solve the Sudoku puzzle using backtracking */
bool solveSudoku(int** grid, int size) {
    return backtrack(grid, size);
}

/** Recursive backtracking function */
bool backtrack(int** grid, int size) {
    int row, col, num;

    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (grid[row][col] == 0) {
                for (num = 1; num <= size; num++) {
                    if (isValid(grid, size, row, col, num, false)) {
                        grid[row][col] = num;
                        if (backtrack(grid, size)) return true;
                        grid[row][col] = 0; /** Backtrack */
                    }
                }
                return false; /** No valid number found */
            }
        }
    }
    return true; /** Puzzle solved */
}

/** Function to free dynamically allocated memory */
void freeMemory(int** grid, int size) {
    int i;

    for (i = 0; i < size; i++) {
        free(grid[i]);
    }
    free(grid);
}
