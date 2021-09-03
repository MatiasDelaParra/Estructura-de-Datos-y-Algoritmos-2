#include <string>
#include <iostream>
using namespace std;

// Dado un arreglo, determinar cual elemento no se encuentra repetido. Utilizo estrategia Divide and Conquer


int estaRepetido(int *a, int medio, int fin)
{
    // Primero analizo los casos borde

    if (medio == 1) // Si es el primer elemento
        if (a[medio] != a[medio + 1])
        {
            cout << a[medio] << endl;
        }

    if (medio == fin) // Si es el ultimo elemento
        if (a[medio] != a[medio - 1])
        {
            cout << a[medio] << endl;
        }
    // Si no es caso borde
    if (medio > 1 && medio < fin)
        if (a[medio] != a[medio - 1])
            if (a[medio] != a[medio + 1])
            {
                cout << a[medio] << endl;
            }

    return 0;
}

int repetidos(int *a, int inicio, int fin, int N)
{
    if (inicio == fin)
        return inicio;

    int mitad = inicio + (fin - inicio) / 2;

    int izq = repetidos(a, inicio, mitad, N);
    int der = repetidos(a, mitad + 1, fin, N);

    return estaRepetido(a, izq, N) + estaRepetido(a, der, N);
}

int main()
{
    int N;
    cin >> N;
    int *a = new int[N + 1];
    for (int i = 1; i <= N; i++)
    {
        int ni;
        cin >> ni;
        a[i] = ni;
    }

    repetidos(a, 1, N, N);
}