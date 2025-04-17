#include "../include/dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Lee el contenido del fichero hash organizado mediante el método de DISPERSIÓN según los criterios
// especificados en la práctica. Se leen todos los cubos completos tengan registros asignados o no. La
// salida que produce esta función permite visualizar el método de DISPERSIÓN
int leeHash(char *fichHash)
{ FILE *f;
  tipoCubo cubo;
  regConfig regC;
  int j,nCubo=0,densidadOcupacion;
  
   if ((f = fopen(fichHash,"rb"))==NULL) return -2;
   fread(&regC,sizeof(regConfig),1,f);
   fread(&cubo,sizeof(cubo),1,f); 
   while (!feof(f)){
	for (j=0;j<C;j++) {
        	if (j==0)    	printf("Cubo %2d (%2d reg. ASIGNADOS)",nCubo,cubo.numRegAsignados);
        	else 	if ((j==1) && cubo.desbordado) printf("DESBORDADO\t\t");
        		else printf("\t\t\t");
		if (j < cubo.numRegAsignados) 
			mostrarReg(&(cubo.reg[j]));
	    	else printf ("\n");
        	}
		nCubo++;
       fread(&cubo,sizeof(cubo),1,f);
   }
   fclose(f);
   printf("ORGANIZACIÓN MÉTODO DISPERSIÓN:\n\t %d CUBOS y %d CUBOS DESBORDE con capacidad %d\n",
   		regC.nCubos,regC.nCubosDes, C);   
   printf("Cubo de Desborde Actual: %d\n", regC.nCuboDesAct);  

   printf("\t Contiene %d registros de los cuales se han desbordado %d:\n",regC.numReg,regC.numRegDes);
   	
   densidadOcupacion = 100*regC.numReg/((regC.nCubos+regC.nCubosDes)*C);
   printf("Densidad ACTUAL de ocupación: %d \n\t(MÍNIMA permitida %f  MÁXIMA permitida %f)\n",
   				densidadOcupacion,regC.densidadMin,regC.densidadMax);
   
   
   if (densidadOcupacion>regC.densidadMax) {
   	printf("No se respeta la densidad máxima de ocupacion\n");
   }	

   if (densidadOcupacion<regC.densidadMin) {
   	printf("No se respeta la densidad mínima de ocupacion\n");
   }	
return 0;	
}



//...............................//
//............PARTE 1............//
//...............................//


int creaHvacio(char *fichHash, regConfig *reg){
    FILE *fHash = fopen(fichHash, "wb");
    if(fHash==NULL){
        return -2;
    }

    reg->numReg=0;
    reg->numRegDes=0;
    reg->nCuboDesAct=reg->nCubos;
    if(fwrite(reg, sizeof(regConfig), 1, fHash)!=1){
        return -2;
    }

    tipoCubo cubo;
    cubo.desbordado=0;
    cubo.numRegAsignados=0;

    for (int j=0; j < reg->nCubos; j++){
        if(fwrite(&cubo,sizeof(tipoCubo),1,fHash)!=1){
            fclose(fHash);
            return -2;
        }
    }

    for (int j=0;j<reg->nCubosDes;j++){
        if(fwrite(&cubo,sizeof(tipoCubo),1,fHash)!=1){
            fclose(fHash);
            return -2;
        }
    }


    fclose(fHash);
    return 0;

}


int creaHash(char *fichEntrada, char *fichHash, regConfig *regC){
    if(regC->densidadMax>100 || regC->densidadMax <=0 || regC ->densidadMin<=0 ||
        regC->densidadMin>100 || regC->densidadMin>regC->densidadMax ||
        regC->nCubos<=0 || regC->nCubosDes<0 ){

        return -5;
    }

    //crear hash vacio
    if(creaHvacio(fichHash, regC)==-2){
        printf("Error en creaHvacio..\n");
        return -2;
    }

    FILE *fEntrada=fopen(fichEntrada, "rb");
    if(fEntrada==NULL){
        printf("Error al abrir fEntrada...\n");
        return -1;
    }

    FILE *fHash=fopen(fichHash, "r+b");
    if(fHash==NULL){
        fclose(fEntrada);
        printf("Error al abrir fHash...\n");
        return -2;
    }

    tipoReg reg;
    
    if(fread(&reg, sizeof(tipoReg), 1, fEntrada)!=1){
        fclose(fEntrada);
        fclose(fHash);
        printf("Error al leer fuera del bucle...\n");
        return -1;
    }

    while(!feof(fEntrada)){
        if(insertar(fHash, &reg, regC)==-2){
            fclose(fHash);
            fclose(fEntrada);
            printf("Error al insertar registro..\n");
            return -2;
        }
        
        fread(&reg, sizeof(tipoReg), 1, fEntrada);
    }

    fclose(fEntrada);
    int capTotal=(regC->nCubos+regC->nCubosDes)*C;
    float densidad=(100.0*(regC->numReg))/capTotal;

    if(densidad < regC->densidadMin){
        fclose(fHash);
        return -4;
    }

    if(densidad > regC->densidadMax){
        fclose(fHash);
        return -3;
    }

    fseek(fHash, 0, SEEK_SET);
    if(fwrite(regC, sizeof(regConfig), 1, fHash) != 1) {
        fclose(fHash);
        printf("Error al escribir el regConfig...\n");
        return -2;
    }

    fclose(fHash);
    return 0;

}


int insertar(FILE *f, tipoReg *reg, regConfig *regC){
    tipoCubo cubo;
    int numcubo=funcionHash(reg, regC->nCubos);
    
    fseek(f, sizeof(regConfig)+numcubo*sizeof(tipoCubo), SEEK_SET);
    if(fread(&cubo, sizeof(tipoCubo), 1, f)!=1){
        printf("Error al leer cubo...\n");
        return -2;
    }

    if(cubo.numRegAsignados < C){
        cubo.reg[cubo.numRegAsignados]=*reg;
        cubo.numRegAsignados++;
        fseek(f, sizeof(regConfig)+numcubo*sizeof(tipoCubo), SEEK_SET);
        if(fwrite(&cubo, sizeof(tipoCubo), 1, f)!=1){
            printf("Error al escribir cubo...\n");
            return -2;
        }
        regC->numReg++;
        return 0;
    }else{
        cubo.desbordado=1;
        fseek(f, sizeof(regConfig)+numcubo*sizeof(tipoCubo), SEEK_SET);
        if(fwrite(&cubo, sizeof(tipoCubo), 1, f)!=1){
            printf("Error al escribir flag desbordado...\n");
            return -2;
        }
        return desborde(f,reg,regC);
    }
    

}


int desborde(FILE *fHash, tipoReg *reg, regConfig *regC){
    tipoCubo cubo;
    


    long posicion=sizeof(regConfig)+(regC->nCuboDesAct)*sizeof(tipoCubo);
    fseek(fHash,posicion, SEEK_SET);
    if(fread(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
        printf("Error al leer cubo desbordado...\n");
        return -2;
    }
    
    if(cubo.numRegAsignados < C){
        //Hay espacio en el cubo de desborde
        cubo.reg[cubo.numRegAsignados]=*reg;
        
        cubo.numRegAsignados++;
        
        fseek(fHash,posicion, SEEK_SET);
        if(fwrite(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
            printf("Error al escribir cubo desbordado...\n");
            return -2;
        }

        regC->numRegDes++;
        regC->numReg++;
        //si se llena, siguiente cubo
        if(cubo.numRegAsignados==C){
            regC->nCuboDesAct++;
        }

        fseek(fHash,0,SEEK_SET);
        if(fwrite(regC, sizeof(regConfig), 1, fHash)!=1){
            printf("Error al escribir config en desbordado...\n");
            return -2;
        }

        
        

    }else {
        //no hay espacio, nuevo cubo de desborde
        tipoCubo nuevo;
        nuevo.desbordado=0;
        nuevo.numRegAsignados=1;
        nuevo.reg[0]=*reg;
        

        fseek(fHash,0,SEEK_END);
        if(fwrite(&nuevo, sizeof(tipoCubo), 1, fHash)!=1){
            printf("Error al escribir cubo final...\n");
            return -2;
        }

        regC->numReg++;
        regC->numRegDes++;
        regC->nCubosDes++;
        regC->nCuboDesAct=regC->nCubosDes-1;
        fseek(fHash,0,SEEK_SET);
        if(fwrite(regC, sizeof(regConfig), 1, fHash)!=1){
            printf("Error al escribir config en desbordado...\n");
            return -2;
        }

        //poner como desbordado y escribir
        cubo.desbordado=1;
        fseek(fHash,posicion,SEEK_SET);
        if(fwrite(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
            printf("Error al escribir flag de desbordado en desbordado...\n");
            return -2;
        }
    }
    
    


    return 0;
}



//...............................//
//............PARTE 2............//
//...............................//


int busquedaHash(FILE *fHash, tipoReg *reg, tPosicion *posicion){
    regConfig regC;
    fseek(fHash, 0, SEEK_SET);
    if(fread(&regC, sizeof(regConfig), 1, fHash)!=1){
        printf("Error al leer el fichero reg...\n");
        return -2;
    }
    
    int numcubo=funcionHash(reg, regC.nCubos);
    if(numcubo==-1){
        printf("Error de funcionHash, parametros incorrectos...\n");
        return -5;
    }

    tipoCubo cubo;
    fseek(fHash, sizeof(regConfig)+numcubo*sizeof(tipoCubo), SEEK_SET);
    if(fread(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
        printf("Error al leer un cubo... \n");
        return -2;
    }

    posicion->cubo=numcubo; //numero de cubo
    posicion->cuboDes=-1;
    if(cubo.desbordado==0){
        //no está desbordado
        
        for(int i=0; i<cubo.numRegAsignados; i++){
            if(cmpClave(reg, &cubo.reg[i])==0){
                *reg=cubo.reg[i];
                posicion->posReg=i;
                return 0;
            }else if(cmpClave(reg, &cubo.reg[i])==-2){
                printf("Error al comparar clave, parametros incorrectos");
                return -5;
            }
        }
    
        
        
    }else{
        tipoCubo cuboDes;
        int numcuboD=0;
        long posDesborde = sizeof(regConfig) + regC.nCubos * sizeof(tipoCubo);
        fseek(fHash, posDesborde , SEEK_SET);
        while(numcuboD < regC.nCubosDes){
            
            if(fread(&cuboDes, sizeof(tipoCubo), 1, fHash)!=1){
                printf("Error al leer cubo de desborde %d\n", numcuboD);
                return -2;
            }
            
            
            for(int i=0; i<cuboDes.numRegAsignados; i++){
                if(cmpClave(reg, &cuboDes.reg[i])==0){
                    *reg=cuboDes.reg[i];
                    posicion->cuboDes=numcuboD;
                    posicion->posReg=i;
                    return 0;
                }else if(cmpClave(reg, &cuboDes.reg[i])==-2){
                    printf("Error al comparar clave, parametros incorrectos");
                    return -5;
                }
            }

            numcuboD++;
            
        }
        
        
    }


    return -1;

}



int modificarReg(FILE *fHash, tipoReg *reg, tPosicion *posicion){
    if(fHash==NULL || reg==NULL || posicion==NULL){
        return -5;
    }

    

    regConfig regC;
    fseek(fHash,0,SEEK_SET);
    if(fread(&regC, sizeof(regConfig), 1, fHash)!= 1){
        return -2;
    }
    
    tipoCubo cubo;
    int resultado=busquedaHash(fHash, reg, posicion);
    if(resultado==0){
        
        

        if(posicion->cuboDes==-1){
            
            fseek(fHash, sizeof(regConfig)+posicion->cubo*sizeof(tipoCubo), SEEK_SET);
            if(fread(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
                return -2;
            }

            if (posicion->posReg < 0 || posicion->posReg >= cubo.numRegAsignados) {
                printf("Posición de registro inválida\n");
                return -5;
            }

            cubo.reg[posicion->posReg]=*reg;
            fseek(fHash, sizeof(regConfig)+posicion->cubo*sizeof(tipoCubo), SEEK_SET);
            if(fwrite(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
                return -2;
            }

        }else{

            fseek(fHash, sizeof(regConfig)+(regC.nCubos + posicion->cuboDes)*sizeof(tipoCubo), SEEK_SET);
            if(fread(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
                return -2;
            }

            if (posicion->posReg < 0 || posicion->posReg >= cubo.numRegAsignados) {
                printf("Posición de registro inválida\n");
                return -5;
            }

            cubo.reg[posicion->posReg]=*reg;
            fseek(fHash, sizeof(regConfig)+(regC.nCubos + posicion->cuboDes)*sizeof(tipoCubo), SEEK_SET);
            if(fwrite(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
                return -2;
            }


        }


    }else if(resultado==-1){
        return -1;
    }else{
        return -5;
    }



    return 0;

}