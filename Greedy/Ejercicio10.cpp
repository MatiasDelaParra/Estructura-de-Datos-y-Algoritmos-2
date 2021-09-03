#include <string>
#include <iostream>

using namespace std;


// Dados N vuelos, se quiere saber cuantas terminales de aviones se necesitan en un aeropuerto sabiendo los horarios de cada vuelo y teniendo en cuenta que no puede haber mas de un avion a la vez en cada terminal. 
// Se requiere hacer el problema en O(NlogN). La estrategia a utilizar sera Greedy y usare HeapSort para ordenar los vuelos por horario. 

class Heap
{
private:
    int *elementos;
    int largo;
    int ultimoLibre;

    // navegar hacia la izquierda
    int izq(int posNodo)
    {
        return posNodo * 2;
    }

    // navegar hacia la derecha
    int der(int posNodo)
    {
        return posNodo * 2 + 1;
    }

    // navego hacia mi padre
    int padre(int posNodo)
    {
        return posNodo / 2;
    }
    void intercambiar(int x, int y)
    {
        int aux = elementos[x];
        elementos[x] = elementos[y];
        elementos[y] = aux;
    }

    void flotar(int nodo)
    {
        if (nodo != 1)
        {
            int nodoPadre = padre(nodo);
            if (elementos[nodo] < elementos[nodoPadre])
            {
                intercambiar(nodo, nodoPadre);
                flotar(nodoPadre);
            }
        }
    }

    void hundir(int nodo)
    {
        // si tiene hijos (al menos 1)
        if (izq(nodo) < ultimoLibre)
        {
            int i = izq(nodo);
            int d = der(nodo);
            int hijoMenor = i;

            if (d < ultimoLibre && elementos[d] < elementos[i])
            {
                hijoMenor = d;
            }

            if (elementos[hijoMenor] < elementos[nodo])
            {
                intercambiar(hijoMenor, nodo);
                hundir(hijoMenor);
            }
        }
    }

public:
    Heap(int tamanio)
    {
        elementos = new int[tamanio + 1];
        largo = tamanio;
        ultimoLibre = 1;
    }

    void insertar(int nuevoElemento)
    {
        if (!estaLleno())
        {
            elementos[ultimoLibre] = nuevoElemento;
            flotar(ultimoLibre);
            ultimoLibre++;
        }
    }

    int obtenerMinimo()
    {
        if (!esVacio())
        {
            return elementos[1];
        }
        return -1;
    }

    void borrarMinimo()
    {
        if (!esVacio())
        {
            elementos[1] = elementos[ultimoLibre - 1];
            ultimoLibre--;
            hundir(1);
        }
    }

    bool esVacio()
    {
        return ultimoLibre == 1;
    }

    bool estaLleno()
    {
        return ultimoLibre > largo;
    }
};

int encontrarPuerto(int *salidas, int *llegadas, int n)
{

    int puertos = 0;
    int res = 0;
    int i = 0;
    int j = 0;

    while (i < n && j < n)
    {
        if (llegadas[i] < salidas[j])
        {
            puertos++;
            i++;
        }

        else if (llegadas[i] >= salidas[j])
        {
            puertos--;
            j++;
        }

        if (puertos > res)
            res = puertos;
    }

    return res;
}

// Creo dos heaps, uno para los horarios de llegada y otro para los de salida. Luego almaceno los horarios ordenados en arrays
void heapSort(int N, Heap *a, Heap *d)
{

    //Para obtener O(NlogN) uso heapsort
    for (int i = 0; i < N; i++)
    {
        int llegada;
        int salida;
        cin >> llegada >> salida;
        a->insertar(llegada);
        d->insertar(salida);
    }
    int minA = 0;
    int minD = 0;

    int i = 0;
    int *salidas = new int[N];
    int *llegadas = new int[N];

    while (!a->esVacio() && !d->esVacio())
    {
        minA = a->obtenerMinimo();
        minD = d->obtenerMinimo();
        a->borrarMinimo();
        d->borrarMinimo();

        salidas[i] = minD;
        llegadas[i] = minA;
        i++;
    }
    cout << encontrarPuerto(salidas, llegadas, N) << endl;
}

int main()
{

    int N;
    cin >> N;
    Heap *a = new Heap(N);
    Heap *d = new Heap(N);

    heapSort(N, a, d);
}
