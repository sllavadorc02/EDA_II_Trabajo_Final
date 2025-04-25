#include <stdio.h>
#include <stdlib.h>
#include "../include/alumno.h"
#include "../include/dispersion.h"

int funcionHash(tAlumno *reg,int nCubos){
    if(nCubos<=0 || reg==NULL){
        return -1;
    }

    long num=atoi(reg->dni);
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
    strncpy(regBusqueda.dni, dni, sizeof(regBusqueda.dni)-1);
    
    tPosicion pos;
    printf("Buscando DNI: '%s'\n", regBusqueda.dni);
    int resultado=busquedaHash(fHash, &regBusqueda, &pos);
    switch (resultado)
    {
    case 0:
        
        mostrarReg(&regBusqueda);
        printf("CUBO %d CUBODESBORDE %d posicion %d\n", pos.cubo, pos.cuboDes, pos.posReg);

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

    tAlumno alumno={0};
    tPosicion pos;
    strncpy(alumno.dni, dni, sizeof(alumno.dni));
    
    printf("Buscando DNI: %s\n", dni); // Depuración
    int resultado=busquedaHash(fHash, &alumno, &pos);
    if(resultado!=0){
        fclose(fHash);
        printf("Error %d en la funcion buscar\n", resultado);
        return resultado;
    }

    printf("Registro encontrado: DNI=%s, Provincia=%s\n", alumno.dni, alumno.provincia); // Depuración

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






int inserta(char *fichero, tAlumno *reg){
    if (fichero == NULL || reg == NULL) {
        printf("Error: Parámetros inválidos\n");
        return -5;
    }

    FILE *fHash=fopen(fichero, "r+b");
    if(fHash ==NULL){
        printf("Error al abrir fichero hash\n");
        return -1;
    }

    regConfig regC;
    fseek(fHash, 0, SEEK_SET);
    if(fread(&regC, sizeof(regConfig), 1, fHash)!=1){
        printf("Error al leer regConfig\n");
        return -1;
    }

    int err=insertar(fHash, reg, &regC);
    if(err==0){
        printf("Alumno insertado con exito\n\n");
    }else{
        printf("Error %d de la funcion insertar...", err);
    }

    fclose(fHash);
    return err;
}