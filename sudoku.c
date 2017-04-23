#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define true 1
#define false 0
void printGrid(int grid[9][9]) {
    int i, j;
    printf("|-------|-------|-------|\n");
    for (i = 0; i < 9; i++) {
        printf("| ");
        for (j = 0; j < 9; j++) {
            printf("%d ", grid[i][j]);
            if ((j + 1) % 3 == 0 && j > 0) {
                printf("| ");
            }
        }
        printf("\n");
        if ((i + 1) % 3 == 0) {
            printf("|-------|-------|-------|\n");
        }
    }
}


int inCol(int num, int col, int grid[9][9]) {
    int i;
    int total = 0;
    for (i = 0; i < 9; i++) {
        if (grid[i][col] == num) {
            total++;
        }
    }
    return total;
}


int inRow(int num, int row, int grid[9][9]) {
    int i;
    int total = 0;
    for (i = 0; i < 9; i++) {
        if (grid[row][i] == num) {
            total++;
        }
    }
    return total;
}

int checkBoxRow(int num, int row, int col, int grid[9][9]) {
    return (grid[row][col] == num) + (grid[row][col + 1] == num) + (grid[row][col + 2] == num);
}

int checkBoxCol(int num, int row, int col, int grid[9][9]) {
    return (grid[row][col] == num) + (grid[row + 1][col] == num) + (grid[row + 2][col] == num);
}

int inBox(int num, int row, int col, int grid[9][9]) {
    int cRow = (row / 3) * 3;
    int cCol = (col / 3) * 3;
    int sum =
        checkBoxRow(num, cRow,     cCol, grid) +
        checkBoxRow(num, cRow + 1, cCol, grid) +
        checkBoxRow(num, cRow + 2, cCol, grid) +
        checkBoxCol(num, cRow,     cCol, grid) +
        checkBoxCol(num, cRow, cCol + 1, grid) +
        checkBoxCol(num, cRow, cCol + 2, grid);
    return sum / 2;

}


int goodRow(int row, int grid[9][9]) {
    int num;
    for (num = 1; num < 10; num++) {
        if (inRow(num, row, grid) != 1) {
            return false;
        }
    }
    return true;
}


int goodCol(int col, int grid[9][9]) {
    int num;
    for (num = 1; num < 10; num++) {
        if (inCol(num, col, grid) != 1) {
            return false;
        }
    }
    return true;
}


int goodRows(int grid[9][9]) {
    int row;
    for (row = 0; row < 9; row++) {
        if (!goodRow(row, grid)) {
            return false;
        }
    }
    return true;
}


int goodCols(int grid[9][9]) {
    int col;
    for (col = 0; col < 9; col++) {
        if (!goodCol(col, grid)) {
            return false;
        }
    }
    return true;
}

int goodBox(int row, int col, int grid[9][9]) {
    // Must be provided the boxes upper left indexes
    int num;
    for (num = 1; num < 10; num++) {
        if (inBox(num, row, col, grid) != 1) {
            return false;
        }
    }
    return true;
}


int goodBoxes(int grid[9][9]) {
    int n1 = 1;
    int n2;
    int row, col;
    for (row = 0; row < 9; row = 3*n1) {
        n2 = 1;
        for (col = 0; col < 9; col = 3*n2) {
            if (!goodBox(row, col, grid)) {
                return false;
            }
            n2++;
        }
        printf("\n");
        n1++;
    }
    return true;
}



int isSolved(int grid[9][9]) {
    int row, col;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                return false;
            }
        }
    }
    return goodRows(grid) && goodCols(grid) && goodBoxes(grid);
}


int recurseSolve(int row, int col, int orig[9][9], int grid[9][9]) {
    // Can't change cell
    /*printGrid(grid);*/
    if (row == 9) {
        return isSolved(grid);
    }
    if (orig[row][col] != 0) {
        if (col == 8) {
            return recurseSolve(row + 1, 0, orig, grid);
        }
        else {
            return recurseSolve(row, col + 1, orig, grid);
        }
    }
    // Can Change Cell
    else {
        int num;
        for (num = 1; num < 10; num++) {
            if (!inRow(num, row, grid) && !inCol(num, col, grid) && !inBox(num, row, col, grid)) {
                grid[row][col] = num;
                if (col == 8) {
                    if (recurseSolve(row + 1, 0, orig, grid)) {
                        return true;
                    }
                }
                else {
                    if (recurseSolve(row, col + 1, orig, grid)) {
                        return true;
                    }
                }
            }
        }
        grid[row][col] = 0;
        return false;
    }
    

}

void write_grid_solution2(double time_spent, int grid[9][9]) {
    char filename[64];
    time_t now;
    struct tm *time = gmtime(&now);
    snprintf(filename, sizeof(char)*64, "./solution_%lu.txt", (long unsigned) now);
    /*snprintf(filename, sizeof(char)*64, "./solution_%d.txt", (int) now);*/
    FILE *output = fopen(filename, "w");
    // Start Writing
    fprintf(output, "Time: %fs\n", time_spent);
    int i, j;
    fprintf(output, "|-------|-------|-------|\n");
    for (i = 0; i < 9; i++) {
        fprintf(output, "| ");
        for (j = 0; j < 9; j++) {
            fprintf(output, "%d ", grid[i][j]);
            if ((j + 1) % 3 == 0 && j > 0) {
                fprintf(output, "| ");
            }
        }
        fprintf(output, "\n");
        if ((i + 1) % 3 == 0) {
            fprintf(output, "|-------|-------|-------|\n");
        }
    }
    fclose(output);
}

void write_grid_solution(int num, double time_spent, int grid[9][9]) {
    char filename[64];
    snprintf(filename, sizeof(char)*64, "./solutions/grid%d_solution.txt", num);
    FILE *output = fopen(filename, "w");
    // Start Writing
    fprintf(output, "Time: %fs\n", time_spent);
    int i, j;
    fprintf(output, "|-------|-------|-------|\n");
    for (i = 0; i < 9; i++) {
        fprintf(output, "| ");
        for (j = 0; j < 9; j++) {
            fprintf(output, "%d ", grid[i][j]);
            if ((j + 1) % 3 == 0 && j > 0) {
                fprintf(output, "| ");
            }
        }
        fprintf(output, "\n");
        if ((i + 1) % 3 == 0) {
            fprintf(output, "|-------|-------|-------|\n");
        }
    }
    fclose(output);
}


int main(int argc, char **argv) {
    // Initial Variable
    if (argc == 2) {
        FILE *input = fopen(argv[1], "r");
        int num;
        int grid[9][9];
        int orig[9][9];
        int scanned;
        int total = 0;
        clock_t begin, end;
        // While read the grid num
        while (1 == fscanf(input, "Grid %d", &num)) {
            printf("Grid %d\n", num);
            // Read in array
            int x, y;
            char digit;
            for (y = 0; y < 9; y++) {
                for (x = 0; x < 9; x++) {
                    // Read in char
                    scanned = fscanf(input, " %c ", &digit);
                    grid[y][x] = digit - '0';
                }
            }
            // Copy Over
            memcpy(orig, grid, sizeof(int)*81);
            // Display Grid
            /*solvePuzzle(orig, grid);*/
            printGrid(grid);
            begin = clock();
            int solved = recurseSolve(0, 0, orig, grid);
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("Solved: %d\t%f\n", num, time_spent);
            write_grid_solution(num, time_spent, grid);
            printGrid(grid);
            total += 100*grid[0][0] + 10*grid[0][1] + grid[0][2];
        }
        fclose(input);
        printf("Summed Total: %d\n", total);
    }
    else {
        int i;
        int grid[9][9];
        int orig[9][9];
        char buffer[11];
        clock_t begin, end;
        int row = 0;
        printf("Input row where blanks are 0's with no spaces!\n");
        while (row < 9) {
            scanf("%s", buffer);
            for (i = 0; i < 9; i++) {
                grid[row][i] = (int) buffer[i] - '0';
            }
            row++;
        }
        memcpy(orig, grid, sizeof(int)*81);
        // Display Grid
        /*solvePuzzle(orig, grid);*/
        printGrid(grid);
        begin = clock();
        int solved = recurseSolve(0, 0, orig, grid);
        end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Solved: %fs\n", time_spent);
        write_grid_solution2(time_spent, grid);
        printGrid(grid);
    }
    return 0;
}

