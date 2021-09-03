#include <string>
#include <iostream>
using namespace std;

// Dado un grafo dirigido y no ponderado , retornar 1 si el mismo contiene algun ciclo y 0 en otro caso. Usare ordenacion topologica para determinar si el grafo es ciclico. 




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
    int peso; // puede que sea 1, en el caso de que no sea ponderado.
    Arista(int unOrigen, int unDestino, int unPeso = 1) : origen(unOrigen), destino(unDestino), peso(unPeso) {}
};

// para trabajar mas comodo defino mi Lista de Aristas como un puntero a un NodoLista de Arista
typedef NodoLista<Arista> *ListaAristas;


class GrafoListaAdyImp 
{
private:
    ListaAristas *listaAdy; // represetnacion del grafo con listas
    int V;                  // cantidad de vertices
    int A;                  // cantidad de arsitas


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
        Arista a1(v, w, pesoArista);
        listaAdy[v] = new NodoLista<Arista>(a1, listaAdy[v]); // se agrega al ppio de la lista de los adyacentes al veritce v
        this->A++;
        if (!esDirigido) // en caso de no ser dirigido podemos duplicar la arista hacia el otro sentido w->v
        {
            Arista a2(w, v, pesoArista);
            listaAdy[w] = new NodoLista<Arista>(a2, listaAdy[w]);
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
    // Cargar lista de adyacencia
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
};

// Devuelve el siguiente vertice con grado 0, si no hay devuelve -1
int siguienteGradoCeroNoVisitado(int *grados, bool *visitados, int V)
{
    for (int i = 1; i <= V; i++)
    {
        if (grados[i] == 0 && !visitados[i])
            return i;
    }
    return -1;
}

int *initGradoEntrada(GrafoListaAdyImp *g, int V)
{

    int *grados = new int[V + 1]();

    for (int i = 1; i <= V; i++)
    {
        ListaAristas adyacentes = g->adyacentesA(i);
        while (adyacentes != NULL)
        {
            Arista a = adyacentes->dato;
            grados[a.destino]++;
            adyacentes = adyacentes->sig;
        }
    }
    return grados;
}

bool *initVisitados(int V)
{
    bool *visitados = new bool[V + 1];
    for (int i = 1; i <= V; i++)
    {
        visitados[i] = false;
    }
    return visitados;
}

int ordenTopologico(GrafoListaAdyImp *g, int V)
{

    int *grados = initGradoEntrada(g, V);
    bool *visitados = initVisitados(V);
    int res = 0;

    for (int i = 1; i <= V; i++)
    {
        int vertice = siguienteGradoCeroNoVisitado(grados, visitados, V);
        if (vertice == -1)
        {
            return 1;
        }
        else
        {
            visitados[vertice] = true;
            ListaAristas adyacentes = g->adyacentesA(vertice);
            while (adyacentes != NULL)
            {
                Arista a = adyacentes->dato;
                grados[a.destino]--;
                adyacentes = adyacentes->sig;
            }
        }
    }
    return res;
}

int main()
{
    int V;
    int E;
    cin >> V;
    cin >> E;
    GrafoListaAdyImp *g = new GrafoListaAdyImp(V, true, false);
    int *grados = initGradoEntrada(g, V);
    bool *visitados = initVisitados(V);
    g->cargarLista(E);

    cout << ordenTopologico(g, V) << endl;
}