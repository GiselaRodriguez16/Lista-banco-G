#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct persona {
    int id;
    char nombre[50];
    char estado;   // 'P' = preferente, 'N' = normal
    int tramite;   // 1=Depositar, 2=Retirar, 3=Credito
    struct persona *sig;
    struct persona *ant;
} Persona;


Persona *cab = NULL, *cola = NULL;


Persona* crearPersona(int id, char* nombre, char estado, int tramite);
Persona* copiarPersona(Persona *original);
void agregarFila(Persona **cab, Persona **cola, Persona *nueva);
void atenderFila(Persona **cab, Persona **cola);
void mostrarFila(Persona *cab);
void liberarFila(Persona **cab, Persona **cola);
void copiarPorEstado(Persona *origen,Persona **cabPref, Persona **colaPref,Persona **cabNorm, Persona **colaNorm);
void mostrarPorTramite(Persona *cab, int tramite);

int main() {
    int opcion, tramite, id;
    char nombre[50], estado;

    do {
        printf("\n=== MENU BANCO ===\n");
        printf("1. Agregar persona a la fila\n");
        printf("2. Atender persona\n");
        printf("3. Mostrar fila completa\n");
        printf("4. Mostrar por tramite (preferentes o normales)\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                printf("ID: ");
                scanf("%d", &id);
                getchar();

                printf("Nombre: ");
                scanf(" %[^\n]s", nombre);

                printf("Estado (P=Preferente, N=Normal): ");
                scanf(" %c", &estado);

                printf("Tramite (1=Depositar, 2=Retirar, 3=Credito): ");
                scanf("%d", &tramite);

                if (tramite >= 1 && tramite <= 3 && (estado == 'P' || estado == 'N')) {
                    Persona* nueva = crearPersona(id, nombre, estado, tramite);
                    agregarFila(&cab, &cola, nueva);
                } else {
                    printf("Datos invalidos.\n");
                }
                break;

            case 2:
                atenderFila(&cab, &cola);
                break;

            case 3:
                mostrarFila(cab);
                break;

            case 4: {

                Persona *cabPref = NULL, *colaPref = NULL;
                Persona *cabNorm = NULL, *colaNorm = NULL;

                copiarPorEstado(cab, &cabPref, &colaPref, &cabNorm, &colaNorm);

                printf("Mostrar fila: 1 = Preferentes, 2 = Normales: ");
                int tipoFila;
                scanf("%d", &tipoFila);

                printf("Tramite (1=Depositar, 2=Retirar, 3=Credito): ");
                scanf("%d", &tramite);

                if (tipoFila == 1)
                    mostrarPorTramite(cabPref, tramite);
                else if (tipoFila == 2)
                    mostrarPorTramite(cabNorm, tramite);
                else
                    printf("Fila invalida.\n");


                liberarFila(&cabPref, &colaPref);
                liberarFila(&cabNorm, &colaNorm);
                break;
            }

            case 5:
                liberarFila(&cab, &cola);
                printf("Saliendo...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 5);

    return 0;
}



Persona* crearPersona(int id, char* nombre, char estado, int tramite) {
    Persona* nueva = (Persona*)malloc(sizeof(Persona));
    if (!nueva) {
        printf("Error: no se pudo asignar memoria\n");
        exit(1);
    }
    nueva->id = id;
    strcpy(nueva->nombre, nombre);
    nueva->estado = estado;
    nueva->tramite = tramite;
    nueva->sig = nueva->ant = NULL;
    return nueva;
}

Persona* copiarPersona(Persona *original) {
    return crearPersona(original->id, original->nombre, original->estado, original->tramite);
}

void agregarFila(Persona **cab, Persona **cola, Persona *nueva) {
    if (*cab == NULL) {
        *cab = *cola = nueva;
    } else {
        if (nueva->estado == 'P') {
            nueva->sig = *cab;
            (*cab)->ant = nueva;
            *cab = nueva;
        } else {
            (*cola)->sig = nueva;
            nueva->ant = *cola;
            *cola = nueva;
        }
    }
}

void atenderFila(Persona **cab, Persona **cola) {
    if (*cab == NULL) {
        printf("La fila esta vacia.\n");
        return;
    }

    Persona *atendido = *cab;
    printf("Atendiendo: ID %d - %s - tramite %d\n", atendido->id, atendido->nombre, atendido->tramite);

    *cab = atendido->sig;
    if (*cab)
        (*cab)->ant = NULL;
    else
        *cola = NULL;

    free(atendido);
}

void mostrarFila(Persona *cab) {
    if (!cab) {
        printf("Fila vacia.\n");
        return;
    }

    Persona *tmp = cab;
    while (tmp) {
        printf("[%d %s %c %d] -> ", tmp->id, tmp->nombre, tmp->estado, tmp->tramite);
        tmp = tmp->sig;
    }
    printf("NULL\n");
}

void liberarFila(Persona **cab, Persona **cola) {
    Persona *tmp = *cab;
    while (tmp) {
        Persona *aux = tmp;
        tmp = tmp->sig;
        free(aux);
    }
    *cab = *cola = NULL;
}

void copiarPorEstado(Persona *origen,Persona **cabPref, Persona **colaPref,Persona **cabNorm, Persona **colaNorm) {
    Persona *actual = origen;
    while (actual != NULL) {
        Persona *copia = copiarPersona(actual);
        if (copia->estado == 'P')
            agregarFila(cabPref, colaPref, copia);
        else
            agregarFila(cabNorm, colaNorm, copia);

        actual = actual->sig;
    }
}

void mostrarPorTramite(Persona *cab, int tramite) {
    char *nombresTramite[] = {"Depositar", "Retirar", "Credito"};
    int encontrado = 0;
    printf("Personas con tramite %s:\n", nombresTramite[tramite - 1]);

    Persona *tmp = cab;
    while (tmp != NULL) {
        if (tmp->tramite == tramite) {
            printf("ID: %d, Nombre: %s, Estado: %c\n", tmp->id, tmp->nombre, tmp->estado);
            encontrado = 1;
        }
        tmp = tmp->sig;
    }

    if (!encontrado)
        printf("No hay personas con ese tramite.\n");
}
