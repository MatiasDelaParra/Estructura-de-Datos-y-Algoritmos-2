#include <climits>
#include <string>
#include <iostream>
using namespace std;

// Dado un grafo no dirigido y no ponderado, determinar si el mismo puede colorearse con exactamente 2 colores de forma tal que no haya 2 vertices consecutivos con el mismo color(basicamente determinar si el grafo es bipartito)


// uso un NodoLista generico
template <class T>
struct NodoLista
{
    T dato;
    NodoLista *sig;
    NodoLista(T &unDato) : dato(unDato), sig(0){};
    NodoLista(T &unDato, NodoLista *unSig) : dato(unDato), sig(unSig){};
};

struct Arista
{
    int origen;
    int destino;
    bool colorNodo;
    int peso; // puede que sea 1, en el caso de que no sea ponderado.
    Arista(int unOrigen, int unDestino, int unPeso = 1, bool unColor = true) : origen(unOrigen), destino(unDestino), peso(unPeso), colorNodo(unColor) {}
};

// para trabajar mas comodo defino mi Lista de Aristas como un puntero a un NodoLista de Arista
typedef NodoLista<Arista> *ListaAristas;

class GrafoListaAdyImp
{
private:
    ListaAristas *listaAdy; // represetnacion del grafo con listas
    int V;                  // cantidad de vertices
    int A;                  // cantidad de arsitas

    //  int * grados;

    bool esDirigido;  // indica si el grafo es dirigido
    bool esPonderado; // indica si el grafo es ponderado

public:
    // Crea un grafo con V vertices (del 1 al V)
    // O(V)
    GrafoListaAdyImp(int cantidadDeVertices, bool esDirigido = true, bool esPonderado = true)
    {
        this->V = cantidadDeVertices;
        this->A = 0;
        this->esDirigido = esDirigido;
        this->esPonderado = esPonderado;

        listaAdy = new ListaAristas[V + 1];
        for (int i = 1; i <= V; i++)
        {
            listaAdy[i] = NULL; // inicializa todas las listas como vacias
        }
    }

    // O(1)
    void aniadirArista(int v, int w, int peso = 1)
    {
        int pesoArista = this->esPonderado ? peso : 1; // en el caso de ser ponderado se toma en cuenta el parametro
        Arista a1(v, w, pesoArista, true);
        listaAdy[v] = new NodoLista<Arista>(a1, listaAdy[v]); // se agrega al ppio de la lista de los adyacentes al veritce v
        this->A++;
        if (!esDirigido) // en caso de no ser dirigido podemos duplicar la arista hacia el otro sentido w->v
        {
            Arista a2(w, v, pesoArista, false);
            listaAdy[w] = new NodoLista<Arista>(a2, listaAdy[w]);
        }
    }

    void cargarLista(int E)
    {
        for (int i = 1; i <= E; i++)
        {
            int vi, wi;
            cin >> vi;
            cin >> wi;
            this->aniadirArista(vi, wi, 1);
        }
    }

    // O(V) pc
    // si bien esta funcion podria ser O(1) si retornamos la lista original, es peligroso si la manipulan => corrompiendo el grafo
    ListaAristas adyacentesA(int origen)
    {
        // copio la lista
        ListaAristas listaRetorno = NULL;
        ListaAristas listaAuxiliar = listaAdy[origen];

        while (listaAuxiliar != NULL)
        {
            Arista aristaAuxiliar = listaAuxiliar->dato;
            listaRetorno = new NodoLista<Arista>(aristaAuxiliar, listaRetorno);
            listaAuxiliar = listaAuxiliar->sig;
        }

        return listaRetorno;
    }
};

void DFS_rec(GrafoListaAdyImp *g, int origen, bool *visitados, int *colores, bool &res)
{
    visitados[origen] = true;
    if (colores[origen] == 0) // Si no esta coloreado
        colores[origen] = 1;
    ListaAristas adyacentesAOrigen = g->adyacentesA(origen);
    while (adyacentesAOrigen != NULL)
    {
        int w = adyacentesAOrigen->dato.destino;

        if (!visitados[w])
        {
            if (colores[origen] == 1) // Los nodos "vecinos" deben ser de distinto color
                colores[w] = 2;
            else
                colores[w] = 1;
            DFS_rec(g, w, visitados, colores, res);
        }
        if (colores[origen] == colores[w])
        {
            res = false;
        }

        adyacentesAOrigen = adyacentesAOrigen->sig;
    }
}

int main()
{
    int V;
    int E;
    cin >> V;
    cin >> E;

    int *colores = new int[V]();
    bool *visitados = new bool[V]();

    GrafoListaAdyImp *g = new GrafoListaAdyImp(V, false, false);

    g->cargarLista(E);

    bool res = true;

    for (int i = 1; i <= V; i++)
    {
        if (!visitados[i])
        {
            DFS_rec(g, i, visitados, colores, res);
        }
    }
    if (res)
        cout << 1 << endl;
    else
        cout << 0 << endl;
}
