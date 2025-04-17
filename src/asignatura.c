#include <stdio.h>
#include <stdlib.h>
#include "../include/asignatura.h"
#include "../include/dispersion.h"

int funcionHash(tAsignatura *reg, int nCubos){
    if(nCubos<=0 || reg==NULL){
        return -1;
    }

    return reg->codigo % nCubos;
}


void mostrarReg(tAsignatura *reg){
    if (reg == NULL) {
        printf("Registro inválido\n");
        return;
    }

    printf("%-6d %-35s %1c %4.1f %4.1f %1c %1c %2d / %-2d\n", reg->codigo, reg->nombre, reg->curso, reg->creditosT, reg->creditosP,
           reg->tipo, reg->cuatrimestre, reg->numGrT, reg->numGrP);


}


int cmpClave(tAsignatura *reg1, tAsignatura *reg2){
    if (reg1 == NULL || reg2 == NULL) {
        printf("Registro inválido\n");
        return -2;
    }

    if(reg1->codigo>reg2->codigo){
        return 1;
    }else if(reg1->codigo==reg2->codigo){
        return 0;
    }else if(reg1->codigo<reg2->codigo){
        return -1;
    }


}



int buscar(char *fichero, int codigo){
    FILE *fHash=fopen(fichero, "rb");
    if(fHash ==NULL){
        printf("Error al abrir fichero hash\n");
        return -1;
    }

    tAsignatura regBusqueda;
    regBusqueda.codigo=codigo;

    tPosicion pos;

    int resultado=busquedaHash(fHash, &regBusqueda, &pos);
    switch (resultado)
    {
    case 0:
        printf("La Asignatura es:\n");
        mostrarReg(&regBusqueda);
        printf("Asignatura encontrada en cubo %d\n", pos.cubo);
        if(pos.cuboDes != -1){
            printf("Desbordado en cubo %d\n", pos.cuboDes);
        }
        printf("En la posicion %d \n", pos.posReg);

        break;
    case -1:
        printf("Asignatura con codigo %d no encontrado \n", codigo);
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


int modificar(char *fichero, int codigo,float creditosT, float creditosP){
   if (fichero == NULL || codigo <=0 || creditosP <=0|| creditosT<=0) {
        printf("Error: Parámetros inválidos\n");
        return -5;
    }
    
    FILE *fHash=fopen(fichero, "r+b");
    if(fHash ==NULL){
        printf("Error al abrir fichero hash\n");
        return -1;
    }

    tAsignatura asig;
    tPosicion pos;
    asig.codigo=codigo;
    
    int resultado=busquedaHash(fHash, &asig, &pos);
    if(resultado!=0){
        fclose(fHash);
        return resultado;
    }

    asig.creditosT=creditosT;
    asig.creditosP=creditosP;
    resultado=modificarReg(fHash, &asig, &pos);
    fclose(fHash);
    if (resultado != 0) {
        printf("Error al modificar: %d\n", resultado);
    } else {
        printf("Asignatura actualizada correctamente con codigo %d\n", codigo);
    }

    return resultado;
}