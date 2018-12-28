//Kevin Yang   ICS3U   Ms. Cullum   4/13/18
//This program adds and multiples two matrices

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

char get_choice(void);
void add(int *** matrix, int * numRows, int * numColumns);
void multiply(int *** matrix, int * numRows, int * numColumns);

int main(void) {

    FILE * fptr = NULL;

    //since don't know dimensions of matrices, must use dynamically-allocated arrays with malloc
    //each element of matrix is a double pointer; it is intended to point to an array of pointers in which each element corresponds to the row #; each pointer in that array points to an array storing the actual values (columns)
    int  ** matrix[2]; //still need to allocate memory for an array of pointers (the size of which depends on the # of rows), and the corresponding arrays of integers (the size of which depends on the # of columns)

    char choice = 0;
    int  numRows[2] = {0}; //the number of rows of each matrix
    int numColumns[2] = {0};  //the number of columns of each matrix
    int   i, j, k;

    if((fptr = fopen("MatrixIn.txt", "r")) == NULL) {
        printf("Invalid file");
        exit(EXIT_FAILURE);
    }

    //matrix[0] is matrix 1, matrix[1] is matrix 2; loop to populate each matrix
    for (i = 0; i < 2; i++) {
        printf("Matrix %d: \n", i+1);

        //scan dimensions of each matrix
        fscanf(fptr, "%d %d", &numRows[i], &numColumns[i]);

        //allocate memory for an array of pointers (each corresponds to row #)
        matrix[i] = (int **)malloc(numRows[i]*sizeof(int *)); //matrix[i] now points to (stores the address of) the first element of an array of pointers
                                                              //malloc returns the address of the first byte of a block

        //for every row, allocate memory for an array of integers that store the actual values
        for (j = 0; j < numRows[i]; j++) {
            matrix[i][j] = (int *)malloc(numColumns[i]*sizeof(int)); //matrix[i][j] now points to the first element of an integer array which stores the actual values of the matrix
            printf("Row %d: ", j+1);

            //Now, the variable "matrix" points to the first element of an array of double pointers.
            //Each double pointer in that array points to the first element of an array of pointers.
            //Each pointer in that array points to the first element of an array of integers (actual values of matrix).

            for (k = 0; k < numColumns[i]; k++) {   //scan and print the elements of each matrix
                fscanf(fptr, "%d", &matrix[i][j][k]);
                printf("%d ", matrix[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    choice = get_choice(); //get user input to add or multiply

    switch(choice) {
        case 'a':
            add(matrix, numRows, numColumns); //pass address of arrays
            break;
        case 'b':
            multiply(matrix, numRows, numColumns); //pass address of arrays
            break;
        case 'q':
            exit(0);
        default:
            puts("Program error\n");
        }

    fclose(fptr);

    //free memory
    for(i=0; i<2; i++) {
        for(j=0; j<numRows[i]; j++) {
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
}


//user decides which operation to perform; returns their choice
char get_choice(void) {
    char choice;
    puts("\nEnter the letter of your choice:\n");
    puts("a. Addition \t b. Multiplication \t q. Quit");
    fflush(stdin);
    choice = getchar();
    while(choice < 'a' || choice > 'c' && choice != 'q') {
        puts("Invalid choice. Please enter a, b, or q: ");
        fflush(stdin);
        choice = getchar();
    }
    return choice;
}
//addition
void add(int *** matrix, int * numRows, int * numColumns)
{
    //validate size of matrices
    if (numRows[0] != numRows[1] || numColumns[0] != numColumns[1]) {
        puts("\nCan only add matrices with the same dimensions.\n");
        exit(0);
    }

    int matrix_sum[*numRows][*numColumns]; //matrix sum has the same dimensions as both matrix 1 and 2
    int i, j;

    for(i=0; i<numRows[0]; i++) {
        for(j=0; j<numColumns[0]; j++) {
            matrix_sum[i][j] = matrix[0][i][j] + matrix[1][i][j];  //add matrix 1 and 2
        }
    }

    //print matrix sum
    puts("\nMatrix sum:\n");
    for(i=0; i<numRows[0]; i++) {
        printf("Row %d: ", i+1);
        for(j=0; j<numColumns[0]; j++) {
            printf("%d ", matrix_sum[i][j]);
        }
        printf("\n");
    }
}


//multiplication
void multiply(int *** matrix, int * numRows, int * numColumns)
{
    //validate size of matrices
    if (numColumns[0] != numRows[1]) {
        puts("\nIn order to multiply, matrix 1 must have the same number of columns as matrix 2 has rows.\n");
        exit(0);
    }

    //matrix product has a dimension of (# of rows of matrix 1)*(# of columns of matrix 2)
    int matrix_prod[numRows[0]][numColumns[1]];
    int i, j, k;

    //initialize matrix product
     for(i=0; i<numRows[0]; i++) {
        for(j=0; j<numColumns[1]; j++)
            matrix_prod[i][j] = 0;
     }
    //multiply
    for(i=0; i < numRows[0]; i++) { //row i of matrix 1 is multiplied by every column of matrix 2; row # of matrix 1 changes the slowest
        for(j=0; j<numColumns[1]; j++) {    //column # of matrix 2 changes faster than row # of matrix 1

            //column # of matrix 1 and row # of matrix 2 change the fastest
            for(k=0; k < numRows[1]; k++) {  //k = column # of matrix 1 and row # of matrix 2
                matrix_prod[i][j] += (matrix[0][i][k] * matrix[1][k][j]);  //column # of matrix 1 changes simultaneously with row # of matrix 2
                                                                           //column # of matrix_prod changes simultaneously with column # of matrix 2
            }
        }
    }

    //print matrix product
    puts("\nMatrix product:\n");
    for(i=0; i<numRows[0]; i++) {
        printf("Row %d: ", i+1);
        for(j=0; j<numColumns[1]; j++) {
            printf("%d ", matrix_prod[i][j]);
        }
        printf("\n");
    }
}
