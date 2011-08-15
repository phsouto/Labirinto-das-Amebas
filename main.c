/*
 * File:   main.c
 * Author: Pedro
 *
 * Created on April 7, 2011, 9:01 PM
 */

#include "main.h"

//------------------------------------------------------------------------------
int main(int argc, char** argv) {
    hMutex = CreateMutex(NULL, FALSE, NULL);
    AmebaData *data;
    data = (AmebaData*) malloc(sizeof (AmebaData));
    data->coord_i = entrada[0];
    data->coord_j = entrada[1];

    hLab = (HANDLE) _beginthreadex(
               NULL,
               0,
               (CAST_FUNCTION) Labirinto,
               NULL,
               0,
               (CAST_LPDWORD) & dwLab);

    Cria_Ameba(entrada[0], entrada[1]);

    while (TRUE) {
        char c = _getch();
        if (c == '\x1B') exit(0);
    };

    return (EXIT_SUCCESS);
}

//------------------------------------------------------------------------------
DWORD WINAPI Labirinto(LPVOID a) {
    int i, j;
    while (TRUE) {
        WaitForSingleObject(hMutex, INFINITE);
        system("cls");
        for (i = 0; i < 21; i++)
            for (j = 0; j < 20; j++) {
                if (matriz[i][j] == PAREDE)
                    printf("\xDB");
                if (matriz[i][j] == LIVRE)
                    printf(" ");
                if (matriz[i][j] == VISITADO)
                    printf("\xB1");
                if (matriz[i][j] == OCUPADO)
                    printf("\2");
                if (j == 19)
                    printf("\n");
            }
        Sleep(200);
        ReleaseMutex(hMutex);
    }
}

//------------------------------------------------------------------------------
DWORD WINAPI Ameba(LPVOID *a) {
    while (TRUE) {
        AmebaData *data = (AmebaData*) a;
        int i = data->coord_i;
        int j = data->coord_j;

        WaitForSingleObject(hMutex, INFINITE);
        if (i == saida[0] && j == saida[1]) {
            puts("!!!!! FIM !!!!!");
            TerminateThread(hLab, dwLab);
            TerminateThread(data->handle, data->id);
        }

        if (Verifica_Posicao(i + 1, j)) {
            matriz[i][j] = VISITADO;
            i += 1;
            Cria_Ameba(i, j);
        } else if (Verifica_Posicao(i - 1, j)) {
            matriz[i][j] = VISITADO;
            i -= 1;
            Cria_Ameba(i, j);
        } else if (Verifica_Posicao(i, j + 1)) {
            matriz[i][j] = VISITADO;
            j += 1;
            Cria_Ameba(i, j);
        } else if (Verifica_Posicao(i, j - 1)) {
            matriz[i][j] = VISITADO;
            j -= 1;
            Cria_Ameba(i, j);
        } else {
            matriz[i][j] = VISITADO;
            _endthread();
        }
        ReleaseMutex(hMutex);
    }
}

//------------------------------------------------------------------------------
void Cria_Ameba(int i, int j) {
    matriz[i][j] = OCUPADO;

    AmebaData *newData;
    newData = (AmebaData*) malloc(sizeof (AmebaData));
    newData->coord_i = i;
    newData->coord_j = j;
    newData->handle = (HANDLE) _beginthreadex(
            NULL,
            0,
            (CAST_FUNCTION) Ameba,
            (LPVOID) newData,
            0,
           (CAST_LPDWORD) & newData->id);
    return;
}

//------------------------------------------------------------------------------
BOOL Verifica_Posicao(int i, int j) {
    if (matriz[i][j] == LIVRE) {
        return TRUE;
    } else {
        return FALSE;
    }
}
