#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int **mx1,**mx2,**mx3;
int rown1,colm1,rown2,colm2;

void * cElement(void * u){
            int index = (int)u; //get the number of the elements in the matrix as {5,6,8,9} will be their numbers {0,1,2,3}

            int row = index/ colm2; //get number of rows by dividing the index of element by number of columns in the output matrix
            int column = index % colm2; //get number of columns by getting the remainder from the number of columns
            int sum = 0;

            for(int k = 0;k<rown2;k++){ //to calculate the element loop to number of common rows and columns
                sum = sum + (mx1[row][k] * mx2[k][column]);
            }

            mx3[row][column] = sum;
}

void * cRow(void * u){
            int index = (int)u;
            int row = index; //number of row is the index of thread
            for(int j = 0;j<colm2;j++){     //calculate row loop to number of columns in the output matrix
                int sum = 0;
                for(int k = 0;k<rown2;k++){  //calculate element loop to number of common rows and columns
                    sum = sum + (mx1[row][k] * mx2[k][j]);
                }
            mx3[row][j] = sum;
            }
}

int readfromFile(){
    FILE *fptr = fopen("input.txt","r");// to reset the file everytime open the program
     if (fptr == NULL){
        printf("Could not open input file\n");
        return 0;
    }
    else{
    fscanf(fptr,"%d",&rown1);
    fscanf(fptr,"%d",&colm1);

    //printf("%d %d\n",rown1,colm1);
    mx1 = (int **)malloc(rown1 * sizeof(int *));
            for (int i = 0; i < rown1; i++) {
                    mx1[i] = (int *)malloc(colm1 * sizeof(int));
             }
    for (int i = 0; i < rown1; i++) {
      for (int j = 0; j < colm1; j++){
            fscanf(fptr,"%d",&mx1[i][j]);
       }
     }

    fscanf(fptr,"%d",&rown2);
    fscanf(fptr,"%d",&colm2);

    mx2 = (int **)malloc(rown2 * sizeof(int *));
            for (int i = 0; i < rown2; i++) {
                    mx2[i] = (int *)malloc(colm2 * sizeof(int));
             }
    for (int i = 0; i < rown2; i++) {
      for (int j = 0; j < colm2; j++){
            fscanf(fptr,"%d",&mx2[i][j]);
       }
     }

     mx3 = (int **)malloc(rown1 * sizeof(int *));
            for (int i = 0; i < rown1; i++) {
                    mx3[i] = (int *)malloc(colm2 * sizeof(int));
             }

    fclose(fptr);
    return 1;
    }
}

//write the element method in file
void writeinFileE(double time_taken){
    FILE *fptr = fopen("output.txt","a");// to reset the file everytime open the program
    fprintf(fptr,"Element Method\n");
    for (int i = 0; i < rown1; i++) {
      for (int j = 0; j < colm2; j++){
        fprintf(fptr,"%d ", mx3[i][j]);
        }
      fprintf(fptr,"\n");
    }
    fprintf(fptr,"END1\t [%lf]\n",time_taken);
    fclose(fptr);
}

//write the row method in file
void writeinFileR(double time_taken){
    FILE *fptr = fopen("output.txt","a");// to reset the file everytime open the program
    fprintf(fptr,"Row Method\n");
    for (int i = 0; i < rown1; i++) {
      for (int j = 0; j < colm2; j++){
        fprintf(fptr,"%d ", mx3[i][j]);
        }
      fprintf(fptr,"\n");
    }
    fprintf(fptr,"END1\t [%lf]\n",time_taken);
    fclose(fptr);
}

/****print output array****/
void print_Array(int **arr){

    for (int i = 0; i < rown1; i++) {
      for (int j = 0; j < colm2; j++){
        printf("%d   |   ", arr[i][j]);
        }
      printf("\n");
    }
}

/****print input1 array****/
void print_Array1(int **arr){

    for (int i = 0; i < rown1; i++) {
      for (int j = 0; j < colm1; j++){
        printf("%d   |   ", arr[i][j]);
        }
      printf("\n");
    }
}

/****print input2 array****/
void print_Array2(int **arr){

    for (int i = 0; i < rown2; i++) {
      for (int j = 0; j < colm2; j++){
        printf("%d   |   ", arr[i][j]);
        }
      printf("\n");
    }
}

int main()
{
    FILE *fptr = fopen("output.txt","w");// to reset the file everytime open the program
    fprintf(fptr,"");
    fclose(fptr);

    /**reading from file**/
    int check = readfromFile();
    if(check == 0){
        return 0;
    }


    if(colm1 != rown2){
        printf("Error in matrix dimension can't multiplicate \n");
        return 0;
    }

    printf("*********FIRST MATRIX*********\n");
    print_Array1(mx1);
    printf("\n");

    printf("*********SECOND MATRIX*********\n");
    print_Array2(mx2);
    printf("\n");

    /**declaring threads arrays**/
    pthread_t ethreads[rown1 * colm2]; //threads array for elements
    pthread_t rthreads[rown1];  //threads array for row

    clock_t t;
    t = clock();
    for(int i = 0;i<rown1 * colm2;i++){
         pthread_create(&ethreads[i],NULL,&cElement,i);
    }

    /***Suspends the main until all the threads terminates and join with it***/
    for (int i = 0; i < rown1 * colm2; i++){
        pthread_join(ethreads[i], NULL);
    }

    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    writeinFileE(time_taken);

    printf("time taken to calculate Element Method: %lf\n\n",time_taken);

    t = clock();
     for(int i = 0;i<rown1;i++){
         pthread_create(&rthreads[i],NULL,&cRow,i);
    }

    /***Suspends the main until all the threads terminates and join with it***/
    for (int i = 0; i < rown1; i++){
        pthread_join(rthreads[i], NULL);
    }

    t = clock() - t;
    time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    writeinFileR(time_taken);

    printf("time taken to calculate Row Method: %lf\n\n",time_taken);


    printf("*********OUTPUT MATRIX*********\n");
    print_Array(mx3);
    printf("\n");


    return 0;
}
