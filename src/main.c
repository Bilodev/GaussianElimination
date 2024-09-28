#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "data.c"

struct pivot {
    size_t i, j;
};

void print_matrix(int rows, int cols, float** matrix) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++)
            printf("%.1f\t", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

bool is_matrix_empty(const int rows, const int cols, float** matrix) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            if (matrix[i][j] != 0)
                return false;
    return true;
}

struct pivot* found_pivot(const int rows, const int cols, float** matrix) {
    struct pivot* pivot_ij = (struct pivot*)malloc(sizeof(struct pivot));
    if (!pivot_ij) return NULL;

    for (size_t c = 0; c < cols; c++) {
        for (size_t r = 0; r < rows; r++)
            if (matrix[r][c] != 0) {
                pivot_ij->i = r;
                pivot_ij->j = c;
                return pivot_ij;
            }
    }
    free(pivot_ij);
    return NULL;
}

void print_row(int row, int columns, float** matrix, int current_row) {
    int offset = rows - current_row;

    for (size_t i = 0; i < offset; i++)
        printf("0.0\t");

    for (size_t i = 0; i < columns; i++)
        printf("%.1f\t", matrix[row][i]);
    printf("\n");
}

void swap_rows(int rows, int cols, float** matrix, int r1, int r2) {
    if (r1 == r2) return;
    float* temp = matrix[r1];
    matrix[r1] = matrix[r2];
    matrix[r2] = temp;
}

void substitute_row(int row, int cols, float** matrix, float alpha) {
    for (size_t i = 0; i < cols; i++)
        matrix[row][i] += alpha * matrix[0][i];

}

int kill_below_pivot(int rows, int cols, float** matrix, struct pivot* pivot_ij) {
    float pivot = matrix[pivot_ij->i][pivot_ij->j];
    for (size_t i = pivot_ij->i + 1; i < rows; i++) {
        float b = matrix[i][pivot_ij->j];
        float alpha = -b / pivot;
        substitute_row(i, cols, matrix, alpha);
    }
    return 1;
}

void write_file(FILE* file, int row, int columns, float** matrix, int current_row) {
    int offset = rows - current_row;

    for (size_t i = 0; i < offset; i++)
        fprintf(file, "0.0 ");

    for (size_t i = 0; i < columns; i++)
        fprintf(file, "%.1f ", matrix[row][i]);
    fprintf(file, "\n");
}

int resolve_matrix(int rows, int columns, float** matrix, FILE* output) {

    if (is_matrix_empty(rows, columns, matrix)) {
        printf("\n");
        return 1;
    }

    struct pivot* pivot_ij = found_pivot(rows, columns, matrix);
    if (!pivot_ij) return 1; // Handle case where no pivot is found

    swap_rows(rows, columns, matrix, 0, pivot_ij->i); // swap riga zero e riga del pivot
    pivot_ij->i = 0;

    kill_below_pivot(rows, columns, matrix, pivot_ij);

    print_row(0, columns, matrix, rows);
    write_file(output, 0, columns, matrix, rows);

    float** new_matrix = (float**)malloc((rows - 1) * sizeof(float*));

    for (size_t i = 0; i < (rows - 1); i++)
        new_matrix[i] = (float*)malloc((columns - 1) * sizeof(float));

    for (size_t i = 1; i < rows; i++)
        for (size_t j = 1; j < columns; j++)
            new_matrix[i - 1][j - 1] = matrix[i][j];

    free(pivot_ij);
    for (size_t i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
    return resolve_matrix(rows - 1, columns - 1, new_matrix, output);
}

int main(int argc, char* argv[]) {
    float** matrix = initialize_matrix();

    print_matrix(rows, cols, matrix);
    for (size_t i = 0; i < cols; i++)
        printf("↓\t");
    printf("\n\n");

    FILE* output = fopen("output.dat", "w");

    return resolve_matrix(rows, cols, matrix, output);
}
