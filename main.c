#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define WRONG_INPUT_STR "Error: Wrong input!\n"
#define WRONG_INPUT 100
#define EOI (-1)  // end of input
#define EOL (-2)  // end of line

#define INITIAL_MATRICES_NUM 4
#define INITIAL_MATRICES_SIZE 10

typedef struct {
    int rows_num;
    int columns_num;
    int **values;
} matrix;

int get_input(matrix**, char**, int*, int*, int*);
int get_input_alt(char**, matrix**, int*);
int input_matrix(matrix*);
int input_matrix_alt(matrix**);
int input_int(int*);
int input_operation(char*);
int input_operations_alt(matrix**, char**, matrix**, int*);
int check_dimensions(matrix*, const char*, int);

matrix* calculate(matrix*, char*, int);
int multiply(matrix***, matrix***);
int add(matrix***, matrix***);
int subtract(matrix***, matrix***);

void* mem_allocation(size_t);
void* mem_reallocation(void*, size_t);
void* mem_callocation(int, size_t);

void print_matrix(matrix*);
void print_matrix_alt(matrix*);
void copy_matrix(matrix*, matrix*);
void free_matrix();
void free_all(char*, matrix*, int);


int main(int argc, char *argv[])
{
    int opt;
    int alt_input = 0, alt_output = 0;
    while((opt = getopt(argc, argv, "io")) != -1) {
        switch (opt) {
            case 'i':
                alt_input = 1;
                break;
            case 'o':
                alt_output = 1;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    int mat_size = INITIAL_MATRICES_NUM;
    int mat_num = 0, ops_num = 0;
    matrix *matrices = mem_allocation(mat_size * sizeof(matrix));
    char *operations = mem_allocation((mat_size - 1) * sizeof(char));
    int input_res;
    printf("Enter matrices and operations:\n");
    if (alt_input) {
        ops_num = INITIAL_MATRICES_NUM - 1;
        input_res = get_input_alt(&operations, &matrices, &ops_num);
    }
    else
        input_res = get_input(&matrices, &operations, &mat_size, &mat_num, &ops_num);
    if (input_res == WRONG_INPUT) {
        free_all(operations, matrices, ops_num);
        fprintf(stderr, WRONG_INPUT_STR);
        return WRONG_INPUT;
    }
    if (check_dimensions(matrices, operations, mat_num) == WRONG_INPUT) {
        free_all(operations, matrices, ops_num);
        fprintf(stderr, WRONG_INPUT_STR);
        return WRONG_INPUT;
    }
    matrix *result = calculate(matrices, operations, ops_num);
    if (alt_output)
        print_matrix_alt(result);
    else
        print_matrix(result);

    return 0;
}

int get_input(matrix **mats, char **ops, int *mat_size, int *mat_num, int *ops_num) {
    int ops_size = *mat_size - 1;
    int input_result = input_matrix((*mats) + *mat_num);

    if (input_result == WRONG_INPUT)
        return WRONG_INPUT;
    ++(*mat_num);
    while(input_result != EOF) {
        if (*ops_num == ops_size) {
            ops_size *= 2;
            *ops = mem_reallocation(*ops, ops_size * sizeof(char));
        }
        input_result = input_operation(*ops + *ops_num);
        if (input_result == WRONG_INPUT)
            return WRONG_INPUT;
        if (input_result != EOF)  {
            ++(*ops_num);
            if (*mat_num == *mat_size) {
                *mat_size *= 2;
                *mats = mem_reallocation(*mats, *mat_size * sizeof(matrix));
            }
            input_result = input_matrix((*mats) + *mat_num);
            if (input_result == WRONG_INPUT)
                return WRONG_INPUT;
            ++(*mat_num);
        }
    }
    return 0;
}

int get_input_alt(char** ops, matrix** ordered_mats, int* ops_num) {
    matrix **mats = mem_allocation(26 * sizeof(matrix*));
    for (int i = 0; i < 26; ++i)
        mats[i] = NULL;
    int input_res = input_matrix_alt(mats);
    while (input_res == 0)
        input_res = input_matrix_alt(mats);
    if (input_res == WRONG_INPUT)
        return WRONG_INPUT;
    if (input_operations_alt(mats, ops, ordered_mats, ops_num) == WRONG_INPUT)
        return WRONG_INPUT;
    for (int i = 0; i < 26; ++i) {
        if (mats[i] != NULL) {
            free_matrix(mats[i]);
            free(mats[i]);
        }
    }
    free(mats);
    return 0;
}

int input_matrix(matrix *dest_matrix)  {
    int rows_num, columns_num;
    if (scanf("%d %d", &rows_num, &columns_num) != 2)
        return WRONG_INPUT;
    dest_matrix->columns_num = columns_num;
    dest_matrix->rows_num = rows_num;
    dest_matrix->values = mem_allocation(rows_num * sizeof(int32_t*));
    for (int i = 0; i < rows_num; ++i)
        (dest_matrix->values)[i] = mem_allocation(columns_num * sizeof(int32_t));

    for (int i = 0; i < rows_num; ++i) {
        for (int j = 0; j < columns_num; ++j) {
            if(scanf("%d", (dest_matrix->values)[i] + j) != 1) {
                free_matrix(dest_matrix);
                return WRONG_INPUT;
            }
        }
    }
    return 0;
}

int input_matrix_alt(matrix **mats)  {
    int c = getchar();
    if (c == '\n')
        return EOI;  // end of matrix input
    int index = c - 'A', input_res;
    if (mats[index] != NULL)
        return WRONG_INPUT;  // matrix with this name already exists
    mats[index] = mem_allocation(1 * sizeof(matrix));
    getchar();
    getchar();
    int columns = INITIAL_MATRICES_SIZE, rows = INITIAL_MATRICES_SIZE;
    int **values = mem_allocation(INITIAL_MATRICES_SIZE * sizeof(int32_t*));
    for (int i = 0; i < INITIAL_MATRICES_SIZE; ++i) {
        values[i] = mem_callocation(INITIAL_MATRICES_SIZE, sizeof(int32_t));
    }
    int i = 0, j = 0;
    input_res = input_int(values[i] + j);
    while(input_res != EOI) {
        if (input_res == WRONG_INPUT)
            return WRONG_INPUT;

        if (input_res == EOL) {
            if (j + 1 != columns) {
                if (i == 0) {
                    columns = j + 1;
                    for (int t = 0; t < rows; ++t)
                        values[t] = mem_reallocation(values[t],
                                                     columns * sizeof(int32_t));
                }
                else
                    // matrix with different number of numbers in rows
                    return WRONG_INPUT;
            }
            ++i;
            j = 0;
            if (i == rows) {
                int old_size = rows;
                rows *= 2;
                values = mem_reallocation(values,rows * sizeof(int32_t*));
                for (int t = old_size; t < rows; ++t)
                    values[t] = mem_callocation(columns, sizeof(int32_t));
            }
        } else {
            ++j;
            if (j == columns) {
                if (i > 0)
                    // matrix with different number of numbers in rows
                    return WRONG_INPUT;
                int old_size = columns;
                columns *= 2;
                for (int t = 0; t < rows; ++t) {
                    values[t] = mem_reallocation(values[t],
                                                 columns * sizeof(int32_t));
                    for (int k = old_size; k < columns; ++k)
                        values[t][k] = 0;
                }
            }
        }
        input_res = input_int(values[i] + j);
    }
    if (i + 1 != rows) {
        for (int t = i + 1; t < rows; ++t)
            free(values[t]);
        rows = i + 1;
        values = mem_reallocation(values,rows * sizeof(int32_t*));
    }
    if (j != columns - 1) {
        columns = j + 1;
        for (int t = 0; t < rows; ++t)
            values[t] = mem_reallocation(values[t],columns * sizeof(int32_t));
    }
    mats[index]->columns_num = columns;
    mats[index]->rows_num = rows;
    mats[index]->values = values;
    getchar();
    return 0;
}

int input_int(int *a) {
    int num_sign = 1, c;
    c = getchar();
    if (c == '-') {
        num_sign = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        *a = *a * 10 + (c - '0');
        c = getchar();
    }
    *a *= num_sign;
    if (c == ']')
        return EOI;
    if (c == ';') {
        getchar();
        return EOL;
    }
    if (c != ' ')
        return WRONG_INPUT;
    return 0;
}

int input_operation(char *dest_operation) {
    int input_res = scanf(" %c", dest_operation);
    if (input_res == EOF)
        return EOF;
    else if (input_res != 1 ||
             !(*dest_operation == '-' ||
               *dest_operation == '+' ||
               *dest_operation == '*')) {
        return WRONG_INPUT;
    }
    return 0;
}

int input_operations_alt(matrix** mats,
                     char** ops,
                     matrix** ordered_mats,
                     int* ops_num) {

    int c = getchar(), i = 0;
    while(c != '\n') {
        if (i % 2 != 0) {
            if (c == '*' || c == '+' || c == '-') {
                if (i / 2 == *ops_num) {
                    *ops_num *= 2;
                    *ops = mem_reallocation(*ops,*ops_num * sizeof(char));
                    *ordered_mats = mem_reallocation(*ordered_mats,
                                                     (*ops_num + 1) * sizeof(matrix));
                }
                (*ops)[i / 2] = (char)c;
            }
            else
                return WRONG_INPUT;
        }
        else {
            if (c >= 'A' && c <= 'Z' && mats[c - 'A'] != NULL)
                copy_matrix(mats[c - 'A'], (*ordered_mats) + (i / 2));
            else
                return WRONG_INPUT;
        }
        c = getchar();
        ++i;
    }
    if (i % 2 == 0)
        return WRONG_INPUT;

    if (*ops_num != i / 2) {
        *ops_num = i / 2;
        *ops = mem_reallocation(*ops,*ops_num * sizeof(char));
        *ordered_mats = mem_reallocation(*ordered_mats,
                                         (*ops_num + 1) * sizeof(matrix));
    }
    return 0;
}

int check_dimensions(matrix *mats, const char *ops, int mat_num) {
    int height[mat_num], width[mat_num];
    int *height_adr[mat_num], *width_adr[mat_num];
    for (int i = 0; i < mat_num; ++i) {
        height_adr[i] = height + i;
        width_adr[i] = width + i;
        height[i] = mats[i].rows_num;
        width[i] = mats[i].columns_num;
    }

    for (int i = 0; i < mat_num - 1; ++i) {
        if (ops[i] == '*') {
            if (*(width_adr[i]) != *(height_adr[i + 1]))
                return WRONG_INPUT;
            *(width_adr[i]) = *(width_adr[i + 1]);
            width_adr[i + 1] = width_adr[i];
            height_adr[i + 1] = height_adr[i];
        }
    }
    for (int i = 0; i < mat_num - 1; ++i) {
        if (ops[i] != '*') {
            if (*(width_adr[i]) != *(width_adr[i + 1]) ||
                *(height_adr[i]) != *(height_adr[i + 1])) {
                return WRONG_INPUT;
            }
        }
    }
    return 0;
}

matrix * calculate(matrix *matrices, char *operations, int ops_num) {
    matrix *matrices_adr[ops_num + 1];
    for (int i = 0; i < ops_num + 1; ++i)
        matrices_adr[i] = matrices + i;

    matrix **ops_result[ops_num + 1];
    for (int i = 0; i < ops_num + 1; ++i)
        ops_result[i] = matrices_adr + i;

    for (int i = 0; i < ops_num; ++i) {
        if (operations[i] == '*')
            multiply(ops_result + i, ops_result + i + 1);
    }
    for (int i = 0; i < ops_num; ++i) {
        if (operations[i] == '+')
            add(ops_result + i, ops_result + i + 1);
        else if (operations[i] == '-')
            subtract(ops_result + i, ops_result + i + 1);
    }
    matrix *result = mem_allocation(sizeof(matrix));
    copy_matrix(matrices_adr[0], result);
    free_matrix(matrices_adr[0]);
    free(operations);
    free(matrices);
    return result;
}

int multiply(matrix ***m1, matrix ***m2) {
    int new_columns_num = (**m2)->columns_num, new_rows_num = (**m1)->rows_num;
    int **new_values = mem_allocation(new_rows_num * sizeof(int32_t*));
    for (int i = 0; i < new_rows_num; ++i)
        new_values[i] = mem_callocation(new_columns_num, sizeof(int32_t));

    for (int i = 0; i < new_rows_num; ++i) {
        for (int j = 0; j < new_columns_num; ++j) {
            for (int z = 0; z < (**m2)->rows_num; ++z)
                new_values[i][j] += (**m1)->values[i][z] * (**m2)->values[z][j];
        }
    }

    free_matrix(**m1);
    free_matrix(**m2);
    (**m1)->rows_num = new_rows_num;
    (**m1)->columns_num = new_columns_num;
    (**m1)->values = new_values;
    **m2 = **m1;
    *m2 = *m1;
    return 0;
}

int add(matrix ***m1, matrix ***m2) {
    for (int i = 0; i < (**m1)->rows_num; ++i) {
        for (int j = 0; j < (**m1)->columns_num; ++j)
            (**m1)->values[i][j] += (**m2)->values[i][j];
    }
    free_matrix(**m2);
    **m2 = **m1;
    *m2 = *m1;
    return 0;
}

int subtract(matrix ***m1, matrix ***m2) {
    for (int i = 0; i < (**m1)->rows_num; ++i) {
        for (int j = 0; j < (**m1)->columns_num; ++j)
            (**m1)->values[i][j] -= (**m2)->values[i][j];
    }
    free_matrix(**m2);
    **m2 = **m1;
    *m2 = *m1;
    return 0;
}

void * mem_allocation(size_t size) {
    void *temp = malloc(size);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }
    return temp;
}

void * mem_reallocation(void* old, size_t size) {
    void *temp = realloc(old, size);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }
    return temp;
}

void * mem_callocation(int num, size_t size) {
    void *temp = calloc(num, size);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }
    return temp;
}

void print_matrix(matrix* m) {
    printf("%d %d\n", m->rows_num, m->columns_num);
    for (int i = 0; i < m->rows_num; ++i) {
        printf("%d", m->values[i][0]);
        for (int j = 1; j < m->columns_num; ++j)
            printf(" %d", m->values[i][j]);
        printf("\n");
    }
}

void print_matrix_alt(matrix* m) {
    printf("[");
    for (int i = 0; i < m->rows_num; ++i) {
        for (int j = 0; j < m->columns_num; ++j) {
            if (i == 0 && j == 0)
                printf("%d", m->values[i][j]);
            else
                printf(" %d", m->values[i][j]);
        }
        if (i != m->rows_num - 1)
            printf(";");
        else
            printf("]\n");
    }
}

void copy_matrix(matrix *from, matrix *to) {
    int rows_num = from->rows_num;
    int columns_num = from->columns_num;
    int **values;
    values = mem_allocation(rows_num * sizeof(int32_t*));
    for (int j = 0; j < rows_num; ++j) {
        values[j] = mem_callocation(columns_num, sizeof(int32_t));
        for (int z = 0; z < columns_num; ++z)
            values[j][z] = from->values[j][z];
    }
    to->rows_num = rows_num;
    to->columns_num = columns_num;
    to->values = values;
}

void free_matrix(matrix* m) {
    for (int i = 0; i < m->rows_num; ++i)
        free(m->values[i]);
    free(m->values);
}

void free_all(char *ops, matrix *ordered_mats, int ops_num) {
    if (ops != NULL)
        free(ops);
    if (ordered_mats != NULL) {
        for (int i = 0; i <= ops_num; ++i)
            free_matrix(ordered_mats + i);
        free(ordered_mats);
    }
}
