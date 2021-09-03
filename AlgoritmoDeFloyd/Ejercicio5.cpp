#include <climits>
#include <string>
#include <iostream>
using namespace std;



// Algoritmo de Floyd

// defino mi Infinito como el maximo inf posible
#define INF INT_MAX;

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

class GrafoMatrizImp
{
private:
    int **matriz;
    int **matrizVengo;
    int V;
    int A;
    bool esDirigido;
    bool esPonderado;

public:
    // Crea un grafo con V vertices (del 1 al V)
    // O(V^2)
    GrafoMatrizImp(int cantidadDeVertices, bool esDirigido = true, bool esPonderado = true)
    {
        int inf = INF;
        this->V = cantidadDeVertices;
        this->A = 0;
        this->esDirigido = esDirigido;
        this->esPonderado = esPonderado;

        matriz = new int *[V + 1];
        for (int i = 1; i <= V; i++)
        {
            matriz[i] = new int[V + 1];
            for (int j = 1; j <= V; j++)
            {
                matriz[i][j] = inf;
            }
        }
    }
    // Devuelve una copia de la matriz del grafo g
    int **grafoMatriz(GrafoMatrizImp *g, int V)
    {
        int **ret = new int *[V + 1];
        int inf = INF;

        for (int i = 1; i <= V; i++)
        {
            ret[i] = new int[V + 1];
            for (int j = 1; j <= V; j++)
            {
                if (g->matriz[i][j] != inf)
                    ret[i][j] = g->matriz[i][j];
                else
                    ret[i][j] = inf;
            }
        }

        return ret;
    }

    // O(1)
    void aniadirArista(int v, int w, int peso = 1)
    {
        matriz[v][w] = esPonderado ? peso : 1;
        if (!esDirigido)
        {
            matriz[w][v] = esPonderado ? peso : 1;
        }
        this->A++;
    }
    // O(V)
    ListaAristas adyacentesA(int origen)
    {
        int inf = INF;
        ListaAristas listaRetorno = NULL;
        for (int i = 1; i <= V; i++)
        {
            if (matriz[origen][i] != inf) // en el caso de que exista una arista origen->i
            {
                Arista aux(origen, i, matriz[origen][i]);
                listaRetorno = new NodoLista<Arista>(aux, listaRetorno);
            }
        }
        return listaRetorno;
    }

    //Cargo lista de adyacencia
    void cargarLista(int E)
    {
        int inf = INF;
        for (int i = 1; i <= E; i++)
        {
            int vi;
            int wi;
            int ci;
            cin >> vi;
            cin >> wi;
            cin >> ci;

            this->aniadirArista(vi, wi, ci);
        }
    }

    void icRec(int origen, int destino, int *vengo)
    {
        if (origen != destino)
        {
            icRec(origen, vengo[destino], vengo);
        }
    }

    void imprimirCamino(int origen, int destino, int *costos, int *vengo)
    {
        int inf = INF;
        if (costos[destino] == inf)
        {
            cout << -1 << endl;
        }
        else
        {
            cout << costos[destino] << endl;
            icRec(origen, destino, vengo);
        }
    }

    void initMatrizVengo(int V, GrafoMatrizImp *g)
    {
        int inf = INF;
        matrizVengo = new int *[V + 1];
        for (int i = 1; i <= V; i++)
        {
            matrizVengo[i] = new int[V + 1];
            for (int j = 1; j <= V; j++)
            {
                matrizVengo[i][j] = g->matriz[i][j] != inf ? i : -1;
            }
        }
    }

    void floyd(GrafoMatrizImp *g, int V)
    {
        int inf = INF;
        int **matriz = g->grafoMatriz(g, V);

        initMatrizVengo(V, g);

        for (int i = 1; i <= V; i++)
            matriz[i][i] = 0;

        for (int k = 1; k <= V; k++)
            for (int i = 1; i <= V; i++)
                for (int j = 1; j <= V; j++)
                {
                    if (matriz[i][k] != inf && matriz[k][j] != inf && matriz[i][j] > matriz[i][k] + matriz[k][j])
                    {
                        matriz[i][j] = matriz[i][k] + matriz[k][j];
                        matrizVengo[i][j] = k;
                    }
                }

        for (int i = 1; i <= V; i++)
        {

            for (int j = 1; j <= V; j++)
            {
                if (i != j)
                    imprimirCamino(i, j, matriz[i], matrizVengo[i]);
                if (matriz[i][j] == 0)
                    cout << -1 << endl;
            }
        }
    }
};

int main()
{
    int V;
    int E;
    cin >> V;
    cin >> E;
    GrafoMatrizImp *g = new GrafoMatrizImp(V, true, true);
    g->cargarLista(E);
    g->floyd(g, V);
}
