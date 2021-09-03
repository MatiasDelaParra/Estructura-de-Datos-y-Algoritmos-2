#include <string>
#include <iostream>
using namespace std;



// De nuevo el mismo problema de la mochila pero con Backtracking

bool sePuedeMeterEnLaMochila(int objetoAEvaluar, int *pesos, int *lineas, int capacidadTam, int capacidadLineas)
{
    return (pesos[objetoAEvaluar] <= capacidadTam) && (lineas[objetoAEvaluar] <= capacidadLineas);
}
void colocarEnLaMochila(int objetoAEvaluar, int &capacidadTam, int &capacidadLineas, int &valorActual, bool *solucion, int *pesos, int *lineas, int *valores)
{
    capacidadTam = capacidadTam - pesos[objetoAEvaluar];
    capacidadLineas = capacidadLineas - lineas[objetoAEvaluar];
    valorActual = valorActual + valores[objetoAEvaluar];
    solucion[objetoAEvaluar] = true;
}
void sacarloDeLaMochila(int objetoAEvaluar, int &capacidadTam, int &capacidadLineas, int &valorActual, bool *solucion, int *pesos, int *lineas, int *valores)
{
    capacidadTam = capacidadTam + pesos[objetoAEvaluar];
    capacidadLineas = capacidadLineas + lineas[objetoAEvaluar];
    valorActual = valorActual - valores[objetoAEvaluar];
    solucion[objetoAEvaluar] = false;
}

bool esMejorSolucion(int actual, int mejor)
{
    return actual > mejor;
}

void clonar(bool *origen, bool *destino, int N)
{
    for (int i = 0; i < N; i++)
    {
        destino[i] = origen[i];
    }
}

void mochila(int objetoAEvaluar, int N, int capacidadTam, int capacidadLineas, int valorActual, int *pesos, int *valores, int *lineas, bool *&solucion, int &mejorValor, bool *&mejorSolucion)
{
    if (objetoAEvaluar < N)
    {
        if (sePuedeMeterEnLaMochila(objetoAEvaluar, pesos, lineas, capacidadTam, capacidadLineas))
        {
            colocarEnLaMochila(objetoAEvaluar, capacidadTam, capacidadLineas, valorActual, solucion, pesos, lineas, valores);
            if (esMejorSolucion(valorActual, mejorValor))
            {
                clonar(solucion, mejorSolucion, N);

                mejorValor = valorActual;
            }
            // trato de usar el objeto
            mochila(objetoAEvaluar + 1, N, capacidadTam, capacidadLineas, valorActual, pesos, valores, lineas, solucion, mejorValor, mejorSolucion);
            sacarloDeLaMochila(objetoAEvaluar, capacidadTam, capacidadLineas, valorActual, solucion, pesos, lineas, valores);
            // no usando el objeto
            mochila(objetoAEvaluar + 1, N, capacidadTam, capacidadLineas, valorActual, pesos, valores, lineas, solucion, mejorValor, mejorSolucion);
        }
        else
        {
            // intentar con el resto
            mochila(objetoAEvaluar + 1, N, capacidadTam, capacidadLineas, valorActual, pesos, valores, lineas, solucion, mejorValor, mejorSolucion);
        }
    }
}

int main()
{
    int N;
    int S;
    int L;
    cin >> N >> S >> L;
    int *tamanio = new int[N + 1];
    int *lineas = new int[N + 1];
    int *valores = new int[N + 1];

    for (int i = 0; i < N; i++)
    {
        int t, l, v;
        cin >> t >> l >> v;
        tamanio[i] = t;
        lineas[i] = l;
        valores[i] = v;
    }

    bool *playground = new bool[N]();
    bool *solucion = new bool[N]();

    int valorMochila = 0;
    mochila(0, N, S, L, 0, tamanio, valores, lineas, playground, valorMochila, solucion);

    cout << valorMochila << endl;
}