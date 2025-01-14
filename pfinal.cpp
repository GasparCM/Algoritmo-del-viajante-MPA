#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

using namespace std;

clock_t tini, tfin;
double tiempo;
int **camino;
int opcion, costevoptimo, n, k, rec, costeMinimo;
int *v, *voptimo;
int listado;

//Esta funcion inicializa la matriz de costes, en vez de hacerlo en el main para asi evitar errores al probar con distintos ficheros txt
int **inicializarArray(int n)
{

    int **a = new int *[n + 1];

    for (int i = 0; i <= n; i++)
    {
        a[i] = new int[n + 1];
    }
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            a[i][j] = 99999;
        }
    }
    return a;
}

//esta funcion imprime la matriz
void imprimirArray(int **a, int n)
{
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            cout << a[i][j] << "   ";
        }
        cout << endl;
    }
}
//con esta funcino calculamos el minimo 
int calcularMinimo(int **camino, int n)
{

    int a = 9999;
    int aux = 0;
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++){
            aux = camino[i][j];
            a = min(aux,a);
        }
    }
    return a;
}

//Con esta funcion obtenemos la matriz de costes, y la montamos, desues de cojer sus datos de un fichero txt que debe encontrarse en la misma carpeta
//que el proyecto
void obtenerLista()
{

    ifstream fDatos;
    char linea[200], *tok;
    char nombre[100];
    int origen, destino, tiempo;

    cout << "Introduce el nombre del fichero: ";
    cin >> nombre;

    // Abrir fichero
    fDatos.open(nombre);

    // Aqui lee la primera linea, el n
    fDatos.getline(linea, 200);
    tok = strtok(linea, " \t\r\n");
    n = atoi(tok);

    // Numero de clientes = tamaño del array
    camino = inicializarArray(n);

    bool seguir = false;
    while (!seguir)
    {
        fDatos.getline(linea, 200);
        if (!strcmp(linea, ""))
        {
            seguir = true;
        }
        else
        {

            tok = strtok(linea, " \t\r\n");
            origen = atoi(tok);

            tok = strtok(NULL, " \t\r\n");
            destino = atoi(tok);

            tok = strtok(NULL, " \t\r\n");
            tiempo = atoi(tok);

            camino[origen][destino] = tiempo;
        }
    }
    fDatos.close();
    costeMinimo = calcularMinimo(camino, n);
    imprimirArray(camino, n);
}

//Esta funcion es con la que calculamos el coste del viaje
int calcularcoste(int **camino, int *v, int n)
{

    int a = 0;

    for (int i = 0; i < n; i++)
    {
        a = a + camino[v[i]][v[i + 1]];
    }
    a = a + camino[v[n]][v[0]];

    return a;
}
//Esta funcion es la que hace que se cumpla el ciclo hamiltoniano, es la que se encarga de que no se recorra una misma ciudad dos veces,
//para ello compara todas las posiciones del vector, devolviendo true en caso de que se repitan y sin que sea igual j y i;
bool ciclos(int *v, int k)
{
    cout<<"k: "<<k<<endl;
    for (int i = 0; i <=k; i++)
    {
        cout<<v[i]<<" ";
    }
    cout<<endl;
    cout<<"entra en ciclos y comprueba "<<endl;
    bool a = false;

    for (int i = 1; i <= k; i++)
    {
        for (int j = 2; j <= k; j++)
        {
            if (v[i] == v[j] && i != j)
            {
                cout<<"son iguales "<<v[i]<<" y "<<v[j]<<endl;
                a = true;
            }
        }
    }
    return a;
}
//Esta funcion es la que usamos para cuando se marca 0 en la opcion 2, cuando queremos imprimir todas las soluciones, mostrando el numero 
//de recorrido que es(1,2,3,4....), que posiciones recorre con sus costes y el coste total de esa posible solucion.
void imprimirInfo(int *v, int costev, int k, int rec)
{

    cout << "\nRecorrido " << rec << endl;
    for (int ii = 0; ii <= n; ii++)
    {
        if (ii == 0)
        {
            cout << "SUPER  " << endl;
        }
        else
        {
            cout << v[ii] << " ";

            cout << camino[v[ii-1]][v[ii]] << endl;
        }
        if (ii == n)
        {
            cout << "SUPER " << camino[v[n]][v[0]] << endl;
        }
        
    }
    cout << "Coste: " << costev << endl;
}
//Esta funcion nos imprime por pantalla la solucion al problema, despues de haber sido calculada por el algoritmo pertinente
void imprimirSolucion(int *voptimo, int costevoptimo, int n)
{

    cout << "\n\nvoptimo: [";
    for (int ii = 0; ii <= n; ii++)
    {
        cout << voptimo[ii] << " ";
    }
    cout << "0]. ";
    cout << "El costo optimo es de : " << costevoptimo << endl;
}
//ALGORITMO 1, VUELTA ATRAS
void viajante(int **camino, int *v, int *voptimo, int &costevoptimo, int k, int n, bool listado)
{
    int costev;
    v[k] = 0;

    while (v[k] != n)
    {
        v[k] = v[k] + 1;

        if (camino[v[k - 1]][v[k]] != 99999 && ciclos(v, k) == false)
        {
            if (k == n)
            {
                costev = calcularcoste(camino, v, n);
                if (listado == false)
                {
                    imprimirInfo(v, costev, k, rec);
                    rec++;
                }
                if (costev < costevoptimo)
                { 
                    for (int i = 0; i <= k; i++)
                    {
                        voptimo[i] = v[i];
                    }
                    costevoptimo = costev;
                }
            }
            else
            {
                viajante(camino, v, voptimo, costevoptimo, k + 1, n, listado); // volvemos a llamar a la funcion recursivamente con el vector que tenemos actualmente i con la k +1
            }
        }
    }
}

//ALGORITMO 2 MEJORADO, RAMIFICACION Y PODA
void viajanteMejorado(int **camino, int *v, int *voptimo, int &costevoptimo, int k, int n)
{

    int costev;
    v[k] = 0;

    while (v[k] != n)
    {
        v[k] = v[k] + 1;

        if (camino[v[k - 1]][v[k]] != 99999 && ciclos(v, k) == false)
        {
            if (k == n)
            {
                costev = calcularcoste(camino, v, n);
                if (costev < costevoptimo)
                {
                    for (int i = 0; i <= k; i++)
                    {
                        voptimo[i] = v[i];
                    }
                    costevoptimo = costev;
                }
            }
            else
            {
                costev = calcularcoste(camino, v, k);
                int costeEstimado = costev + (n - k) * costeMinimo;
                if(costeEstimado < costevoptimo)viajanteMejorado(camino, v, voptimo, costevoptimo, k + 1, n); // volvemos a llamar a la funcion recursivamente con el vector que tenemos actualmente i con la k +1
            }
            
        }
    }
}
//Funcion que nos permite en la opcion dos marcar 0 para ver todas las soluciones o 1 para imprimir solo la solucion optima
bool ocultarListado()
{
    int ver;
    bool validado = false;
    do
    {
        cout << "Desea ver todos los recorridos (0) o solo el optimo (1) ?: ";
        cin >> ver;
        if (ver == 1 || ver == 0)
        {
            validado = true;
        }
    } while (!validado);

    if (ver == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//Inicializamos las variables con esta funcion.
void inicializarVariables(){
    rec = 1;
    k = 1;
    camino = inicializarArray(n);
    v = new int[n];
    v[0] = 0;
    voptimo = new int[n];
    costevoptimo = 99999;

}
//Funcion con la que imprimimos el menu por pantalla a lo largo de todo el programa
void imprimirmenu()
{

    bool validado = false;

    do
    {
        cout << "\n\n1.- Obtener lista de clientes\n";
        cout << "2.- Ocultar listado de recorridos en opcion 3?\n";
        cout << "3.- Listado de recorridos resaltando el mas corto\n";
        cout << "4.- Recorrido mas corto (algoritmo mejorado)\n";
        cout << "5.- Salir\n";
        cout << "Opcion: ";
        cin >> opcion;
        if (opcion >= 1 || opcion <= 5)
        {
            validado = true;
        }
        else
        {
            cout << "Introduce un valor entre 1 y 5.";
        }
    } while (!validado);

    switch ((opcion))
    {
    case 1:
        inicializarVariables();
        obtenerLista();
        imprimirmenu();
        break;

    case 2:
        listado = ocultarListado();
        imprimirmenu();
        break;

    case 3:
        tini=clock();
        viajante(camino, v, voptimo, costevoptimo, k, n, listado);
        imprimirSolucion(voptimo, costevoptimo, n);
        tfin=clock();
        tiempo = double(tfin-tini)/CLOCKS_PER_SEC*1000;
        cout<<"el tiempo es:"<<tiempo<<endl;
        imprimirmenu();
        break;

    case 4:
        tini=clock();
        viajanteMejorado(camino, v, voptimo, costevoptimo, k, n);
        imprimirSolucion(voptimo, costevoptimo, n);
        tfin=clock();
        tiempo = double(tfin-tini)/CLOCKS_PER_SEC*1000;
        cout<<"el tiempo es:"<<tiempo<<endl;
        imprimirmenu();
        break;

    case 5:
        exit(0);
        break;
    }
}


int main()
{
    imprimirmenu();
    return 0;
}
