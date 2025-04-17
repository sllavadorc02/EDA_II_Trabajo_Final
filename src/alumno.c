#include <stdio.h>
#include <stdlib.h>
#include "../include/alumno.h"
#include "../include/dispersion.h"

int funcionHash(tAlumno *reg,int nCubos){
    if(nCubos<=0 || reg==NULL){
        return -1;
    }

    int num=atoi(reg->dni);
    return num % nCubos;

}


void mostrarReg(tAlumno *reg){
    if (reg == NULL) {
        printf("Registro inválido\n");
        return;
    }
    printf("%9s\t%s %s %s\t%s\n", reg->dni, reg->nombre, reg->ape1, reg->ape2, reg->provincia);
}


int cmpClave(tAlumno *reg1, tAlumno *reg2){
    if (reg1 == NULL || reg2 == NULL) {
        printf("Registro inválido\n");
        return -2;
    }
    return strcmp(reg1->dni, reg2->dni);
}

int buscar(char *fichero, char *dni){
    FILE *fHash=fopen(fichero, "rb");
    if(fHash ==NULL){
        printf("Error al abrir fichero hash\n");
        return -1;
    }

    tAlumno regBusqueda;
    strncpy(regBusqueda.dni, dni, sizeof(regBusqueda.dni));

    tPosicion pos;

    int resultado=busquedaHash(fHash, &regBusqueda, &pos);
    switch (resultado)
    {
    case 0:
        printf("El alumno es :\n");
        mostrarReg(&regBusqueda);
        printf("Alumno encontrado en cubo %d\n", pos.cubo);
        if(pos.cuboDes != -1){
            printf("Desbordado en cubo %d\n", pos.cuboDes);
        }
        printf("En la posicion %d \n", pos.posReg);

        break;
    case -1:
        printf("Alumno con DNI %s no encontrado \n", dni);
        break;
    
    case -2:
        printf("Error de lectura del archivo\n");
        break;

    case -5:
        printf("Error interno en la busqueda\n");
        break;

    default:
        break;
    }

    fclose(fHash);
    return resultado;


}

int modificar(char *fichero, char *dni,char *provincia){
    if (fichero == NULL || dni == NULL || provincia == NULL) {
        printf("Error: Parámetros inválidos\n");
        return -5;
    }
    
    FILE *fHash=fopen(fichero, "r+b");
    if(fHash ==NULL){
        printf("Error al abrir fichero hash\n");
        return -1;
    }

    tAlumno alumno;
    tPosicion pos;
    strncpy(alumno.dni, dni, sizeof(alumno.dni));
    
    int resultado=busquedaHash(fHash, &alumno, &pos);
    if(resultado!=0){
        fclose(fHash);
        if(resultado==-1){
            return -1;
        }
        return resultado;
    }

    strncpy(alumno.provincia, provincia, sizeof(alumno.provincia));
    resultado=modificarReg(fHash, &alumno, &pos);
    fclose(fHash);
    if (resultado != 0) {
        printf("Error al modificar: %d\n", resultado);
    } else {
        printf("Alumno actualizado correctamente con DNI %s\n", dni);
    }

    return resultado;

}