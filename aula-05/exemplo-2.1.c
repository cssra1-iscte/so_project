#include <stdio.h>
int main () {
	int a = 10, b = 3;
	float r;
	int v[4] = {2, 6, -9, 78};
	r = (float)a / b;
	printf("A divisão de %5d por %05d é %05.2f\n", a, b, r );
	int x = (b > 2);
	printf("valor de x : %d\n", x);
	// mostrar a 3ª posição do array v[] --> v[2]
	printf("valor de v[2] é %d\n", v[2]);
}
