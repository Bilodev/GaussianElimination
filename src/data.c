#include <stdio.h>
#include <stdlib.h>


const char* file_name = "input.dat";
int rows, cols;

float** initialize_matrix()
{
    FILE* file = fopen(file_name, "r");
    if (!file) {
        perror("Errore nell'apertura del file");
        exit(1);
    }

    fscanf(file, "%d", &rows);
    fscanf(file, "%d", &cols);
    printf("rows: %d, cols: %d\n\n", rows, cols);

    float** matrix = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++)
        matrix[i] = (float*)malloc(cols * sizeof(float));

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            fscanf(file, "%f", &matrix[i][j]);


    fclose(file);

    return matrix;
}
