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
        return;
    }
    return strcmp(reg1->dni, reg2->dni);
}

int buscar(char *fichero, char *dni){


}

int modificar(char *fichero, char *dni,char *provincia){


}