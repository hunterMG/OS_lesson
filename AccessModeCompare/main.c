#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 888
int main()
{
    int a[N][N];
    int i=0,j=0, tmp;
    clock_t start, end;
    start  = clock();
    for(i=0; i<N; i++){
        for(j=0;j<N;j++){
            tmp = a[i][j];
        }
    }
    end = clock();

    printf("access by row: %d ms\n", end-start);

    start  = clock();
    for (i=0; i<N; i++){
        for(j=0; j<N;j++){
            tmp = a[j][i];
        }
    }
    end = clock();

    printf("access by col: %d ms\n", end-start);

    return 0;
}
