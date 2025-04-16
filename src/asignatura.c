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


    
}


int modificar(char *fichero, int codigo,float creditosT, float creditosP){



}