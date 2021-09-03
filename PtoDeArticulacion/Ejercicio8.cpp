#include <climits>
#include <string>
#include <iostream>
using namespace std;

// Dado un grafo no dirigido y conexo, determinar si tiene puntos de articulacion(verificar si al remover cualquier vertice el grafo sigue siendo conexo)


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
    ListaAristas *listaAdy; // representacion del grafo con listas
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

    // carga los elementos del grafo al grafo
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

    void DFS_rec(GrafoListaAdyImp *g, int origen, bool *visitados)
    {
        visitados[origen] = true;

        ListaAristas adyacentesAOrigen = g->adyacentesA(origen);
        while (adyacentesAOrigen != NULL)
        {
            int w = adyacentesAOrigen->dato.destino;

            if (!visitados[w])
            {
                DFS_rec(g, w, visitados);
            }

            adyacentesAOrigen = adyacentesAOrigen->sig;
        }
    }

    bool todosEnTrue(bool *visitados, int V)
    {
        for (int i = 1; i <= V; i++)
        {
            if (!visitados[i])
            {
                return false;
            }
        }
        return true;
    }

    // Devuelve true sii el grafo es conexo
    bool esConexo(int V, int ignorado)
    {
        bool *visitados = new bool[V + 1]();
        int origen = ignorado == 1 ? 2 : 1;
        DFS_rec(this, origen, visitados);
        visitados[ignorado] = true;
        return todosEnTrue(visitados, V);
    }

    //pre: El grafo no es dirigido
    // Elimina el vertice v y todas sus aristas adyacentes del grafo
    void eliminarVertice(int v)
    {
        ListaAristas ady = this->adyacentesA(v);
        while (ady != NULL)
        {
            int w = ady->dato.destino;
            EliminarAristaNoDirigido(v, w);
            ady = ady->sig;
        }
    }

    // agrega el vertice v al grafo
    void agregarVertice(int v, ListaAristas ady)
    {
        while (ady != NULL)
        {
            int w = ady->dato.destino;
            this->aniadirArista(v, w, 1);
            ady = ady->sig;
        }
    }

    void eliminarArista(int origen, int destino)
    {
        ListaAristas ady = this->adyacentesA(origen);
        ListaAristas actual = ady;
        ListaAristas anterior = ady;

        if (actual != NULL)
        {
            if (actual->dato.destino == destino)
            {
                ady = actual->sig;

                delete (actual);
            }
            else
            {
                while (actual->dato.destino != destino)
                {
                    anterior = actual;
                    actual = actual->sig;
                }
                anterior->sig = actual->sig;
                delete (actual);
            }
        }

        listaAdy[origen] = ady;
    }

    // necesito eliminar ambos sentidos en las aristas para grafos no dirigidos
    void EliminarAristaNoDirigido(int origen, int destino)
    {
        eliminarArista(origen, destino);
        eliminarArista(destino, origen);
    }

    void PtoArticulacion(GrafoListaAdyImp *g, int V)
    {
        bool *visitados = new bool[V + 1]();
        for (int i = 1; i <= V; i++)
        {
            ListaAristas adyacentes = g->adyacentesA(i);
            g->eliminarVertice(i);
            if (!g->esConexo(V, i) && !visitados[i])
            {
                visitados[i] = true;
                cout << i << endl;
            }

            g->agregarVertice(i, adyacentes);
        }
    }
};

int main()
{
    int V;
    int E;
    cin >> V;
    cin >> E;
    GrafoListaAdyImp *g = new GrafoListaAdyImp(V, false, false);
    g->cargarLista(E);
    g->PtoArticulacion(g, V);
}