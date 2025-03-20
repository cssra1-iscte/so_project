#include <stdio.h>
int main() {
    int a;
    float b;
    printf("Escreva um número inteiro: ");
    scanf("%d", &a);
    printf("Escreva um número com vírgula flutuante: ");
    scanf("%f", &b);
    printf("%d + %f = %f\n", a, b, a + b );
}