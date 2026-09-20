/*
* 1. Longoria Flores Cesar
* 2. Molina Alvarado Alvaro Moises
* 3. Pantoja Araiza Ismael
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <omp.h>
#include <string>

using namespace std;

void imprimirNombres() {
    cout << "\n=======================================================\n";
    cout << "Equipo:\n";
    cout << "1. Longoria Flores Cesar\n";
    cout << "2. Molina Alvarado Alvaro Moises\n";
    cout << "3. Pantoja Araiza Ismael\n";
    cout << "=======================================================\n\n";
}

class GeneradorArreglo {
private:
    int* arreglo;
    long long size;
    int rank;
    int num_procs;
    string nombreEquipo;

public:
    GeneradorArreglo(long long s, int r, int p, string equipo)
        : size(s), rank(r), num_procs(p), nombreEquipo(equipo) {
        arreglo = new int[size];
    }

    ~GeneradorArreglo() {
        delete[] arreglo;
    }

    void llenarYProcesar() {
        srand(time(NULL) + rank);

        long long completados = 0;
        int siguiente_porcentaje = 10;

        #pragma omp parallel for
        for (long long i = 0; i < size; i++) {
            arreglo[i] = rand() % 1000;

            long long local_c;
            #pragma omp atomic capture
            {
                completados++;
                local_c = completados;
            }

            long long target = (size * siguiente_porcentaje) / 100;

            if (local_c >= target) {
                #pragma omp critical
                {
                    if (siguiente_porcentaje <= 100 && local_c >= (size * siguiente_porcentaje) / 100) {
                        cout << siguiente_porcentaje << "% - Proceso/Nodo: " << rank
                             << " - Equipo: " << nombreEquipo
                             << " - Hilo OpenMP: " << omp_get_thread_num() << "\n";
                        siguiente_porcentaje += 10;
                    }
                }
            }
        }
    }

    void imprimirArreglo() {
        if (size <= 50) {
            cout << "\n[Proceso " << rank << " - " << nombreEquipo << "] Contenido del arreglo:\n";
            for (long long i = 0; i < size; i++) {
                cout << arreglo[i] << " ";
            }
            cout << "\n\n";
        }
    }
};

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_procs);

    if (rank == 0) {
        imprimirNombres();
    }

    string equipo = "Cesar, Moy e Ismael";
    long long tamano_arreglo = 0;

    if (argc > 1) {
        tamano_arreglo = atoll(argv[1]);
    } else {
        srand(time(NULL) + rank);+
        tamano_arreglo = 20 + (rand() % 31);
    }

    if (rank == 0) {
        cout << "=== Iniciando procesamiento con " << size_procs << " procesos MPI ===" << endl;
        cout << "Tamano del arreglo dinamico por proceso: " << tamano_arreglo << " elementos\n\n";
    }

    MPI_Barrier(MPI_COMM_WORLD);

    GeneradorArreglo generador(tamano_arreglo, rank, size_procs, equipo);

    generador.llenarYProcesar();

    MPI_Barrier(MPI_COMM_WORLD);

    generador.imprimirArreglo();

    if (rank == 0) {
        cout << "Ejecucion finalizada correctamente.\n";
        imprimirNombres();
    }

    MPI_Finalize();
    return 0;
}
