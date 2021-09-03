#include <string>
#include <iostream>
using namespace std;

/* Se le pide a un alumno entregar un proyecto en el cual el alumno debera elegir hacer los ejercicios que le permitar conseguir la mayor cantidad de puntos de
puntos teniendo en cuenta que no debe superar una cierta cantidad de MB de tamanio en archivos y no puede superar una cierta cantidad de lineas. Esto es resolver
el problema de la mochila. Se pide resolverlo con Programacion Dinamica. 
*/

int max(int a, int b)
{
    return a < b ? b : a;
}

void mochila01Recorrido(int capacidadTam, int cantidadObjetos, int capacidadLineas, int *pesosTam, int *pesosLinea, int *valores)
{

    // Creo el cubo
    int ***m = new int **[cantidadObjetos + 1];
    for (int i = 0; i < cantidadObjetos + 1; i++)
    {
        m[i] = new int *[capacidadTam + 1]();
        for (int j = 0; j < capacidadTam + 1; j++)
        {
            m[i][j] = new int[capacidadLineas + 1]();
        }
    }

    for (int objetoHasta = 1; objetoHasta <= cantidadObjetos; objetoHasta++)
        for (int capacidadTamHasta = 1; capacidadTamHasta <= capacidadTam; capacidadTamHasta++)
            for (int capacidadLineasHasta = 1; capacidadLineasHasta <= capacidadLineas; capacidadLineasHasta++)
            {
               
                if (pesosTam[objetoHasta] > capacidadTamHasta || pesosLinea[objetoHasta] > capacidadLineasHasta)
                {
                    m[objetoHasta][capacidadTamHasta][capacidadLineasHasta] = m[objetoHasta - 1][capacidadTamHasta][capacidadLineasHasta];
                }
                else
                {
                    
                    int valorDeUsarElObjeto = valores[objetoHasta] + m[objetoHasta - 1][capacidadTamHasta - pesosTam[objetoHasta]][capacidadLineasHasta - pesosLinea[objetoHasta]];
                    int valorDeNoUsarElObjeto = objetoHasta == 0 ? 0 : m[objetoHasta - 1][capacidadTamHasta][capacidadLineasHasta];
                     if(pesosTam[objetoHasta]<= 50 && pesosLinea[objetoHasta] <= 1000)
                    m[objetoHasta][capacidadTamHasta][capacidadLineasHasta] = max(valorDeUsarElObjeto, valorDeNoUsarElObjeto);
                    else
                    m[objetoHasta][capacidadTamHasta][capacidadLineasHasta] = valorDeNoUsarElObjeto;// Si el archivo no cumple con las condiciones entonces se que no debo usarlo
                }
         
            }

    cout << m[cantidadObjetos - 1][capacidadTam][capacidadLineas] << endl;
}

int main()
{
    int N;
    int S;
    int L;
    cin >> N >> S >> L;
    int *tamanio = new int[N + 1];
    int *lineasCodigo = new int[N + 1];
    int *valores = new int[N + 1];

    for (int i = 1; i <= N; i++)
    {
        int t, l, v;
        cin >> t >> l >> v;
        tamanio[i] = t;
        lineasCodigo[i] = l;
        valores[i] = v;
    }

    mochila01Recorrido(S, N, L, tamanio, lineasCodigo, valores);
}