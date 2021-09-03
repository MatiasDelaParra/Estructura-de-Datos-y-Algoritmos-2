#define TABLA_HASH_ABIERTA
#define TABLA_HASH_ABIERTA
#include <string>
#include <iostream>
using namespace std;

// Dado un arreglo, se quiere saber cuantas palabras estan repetidas exactamente dos veces. Para resolverlo en O(N) promedio uso una tabla de hash.






struct NodoLista
{
    string clave;
    int valor;
    NodoLista *sig;
    NodoLista(string unaClave, int unValor) : clave(unaClave), valor(unValor), sig(0){};
    NodoLista(string unaClave, int unValor, NodoLista *unSig) : clave(unaClave), valor(unValor), sig(unSig){};
};

class TablaHashAbierta_Agenda
{
private:
    typedef NodoLista *Lista;
    Lista *arrList;
    int tamanio;
    int cantidadDeElementos;

    float factorDeCarga()
    {
        return (float)this->cantidadDeElementos / this->tamanio;
    }

    bool insertarRecusrivo(string unaClave, int unValor, NodoLista *&ptr)
    {
        if (ptr == NULL)
        {
            ptr = new NodoLista(unaClave, unValor);
            return false;
        }
        else
        {
            if (ptr->clave == unaClave)
            {
                ptr->valor = unValor;
                return true;
            }
            else
            {
                return insertarRecusrivo(unaClave, unValor, ptr->sig);
            }
        }
    }

    int cambiarValor(NodoLista *&ptr)
    {
        ptr->valor = ptr->valor + 1;
        return ptr->valor;
    }

    int recuperarRecursivo(string unaClave, NodoLista *&ptr)
    {
        if (ptr == NULL)
        {
            return -1;
        }
        else
        {
            if (ptr->clave == unaClave)
            {
                return ptr->valor;
            }
            else
            {
                return recuperarRecursivo(unaClave, ptr->sig);
            }
        }
    }

    NodoLista *existeRecursivo(string unaClave, NodoLista *&ptr)
    {
        if (ptr == NULL)
        {
            return NULL;
        }
        else
        {
            if (ptr->clave == unaClave)
            {
                return ptr;
            }
            else
            {
                return existeRecursivo(unaClave, ptr->sig);
            }
        }
    }

    void eliminarRecursivo(string unaClave, NodoLista *&ptr)
    {
        if (ptr != NULL)
        {
            if (ptr->clave == unaClave)
            {
                NodoLista *aEliminar = ptr;
                ptr = ptr->sig;
                delete aEliminar;
            }
            else
            {
                return eliminarRecursivo(unaClave, ptr->sig);
            }
        }
    }

public:
    TablaHashAbierta_Agenda(int tamaniInicial)
    {
        this->tamanio = tamaniInicial;
        this->cantidadDeElementos = 0;
        arrList = new Lista[this->tamanio]();
    }

    int setClave(NodoLista *&ptr)
    {
        return cambiarValor(ptr);
    }

    int fnHash(string clave)
    {

        int h = 0;

        for (int i = 0; i < clave.length(); i++)
        {
            h = 31 * h + int(clave[i]);
        }
        return h;
    }

    float getFactordeCarga()
    {
        return this->factorDeCarga();
    }

    void insertar(string unaClave, int unValor)
    {
        int pos = abs(this->fnHash(unaClave)) % this->tamanio;
        if (!this->insertarRecusrivo(unaClave, unValor, arrList[pos]))
        {
            this->cantidadDeElementos++;
        }
    }

    int recuperar(string unaClave)
    {
        int pos = abs(this->fnHash(unaClave)) % this->tamanio;
        return this->recuperarRecursivo(unaClave, arrList[pos]);
    }

    NodoLista *existe(string unaClave)
    {
        int pos = abs(this->fnHash(unaClave)) % this->tamanio;
        return this->existeRecursivo(unaClave, arrList[pos]);
    }

    void eliminar(string unaClave)
    {
        int pos = abs(this->fnHash(unaClave)) % this->tamanio;
        this->eliminarRecursivo(unaClave, arrList[pos]);
    }
};

int main()
{
    int N;
    cin >> N;

    TablaHashAbierta_Agenda *hash = new TablaHashAbierta_Agenda(2 * N);

    int res = 0;

    for (int i = 0; i < N; i++)
    {
        int valor = 0;
        string palabra;
        cin >> palabra;

        NodoLista *ptr = hash->existe(palabra); 
        if (ptr != NULL) 
        {

            int clave = hash->setClave(ptr);

            if (clave == 2)
                res++;
            if (clave == 3)
                res--;
        }
        else
            hash->insertar(palabra, 1);
    }

    cout << res << endl;
}
