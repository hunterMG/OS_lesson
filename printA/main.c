#include <stdio.h>
#include <stdlib.h>

int row,col;
int a[99][99];

int main()
{
    int row,col;
    int i=0, j=0;
    int  ch = 65;// 'A'
    scanf("%d%d", &row, &col);
    for(i = 0; i < row; i++) {
        for(j = i; j < col - i; j++)
            a[i][j] = ch++;

        for(j = i + 1; j < row - i; j++)
            a[j][col - i - 1] = ch++;

        for(j = col - i - 2; j >= i; j--)
            a[row - i - 1][j] = ch++;

        for(j = row - i - 2; j > i; j--)
            a[j][i] = ch++;
    }

    //output
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            printf("%c ", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}
