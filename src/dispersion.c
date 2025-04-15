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



int creaHvacio(char *fichHash, regConfig *reg){
    FILE *fHash = fopen(fichHash, "wb");
    if(fHash==NULL){
        return -2;
    }

    reg->numReg=0;
    reg->numRegDes=0;
    reg->nCuboDesAct=reg->nCubos;

    tipoCubo cubo;
    cubo.desbordado=0;
    cubo.numRegAsignados=0;

    for (int j=0;j<reg->nCubos;j++){
        if(fwrite(&cubo,sizeof(cubo),1,fHash)!=1){
            fclose(fHash);
            return -2;
        }
    }

    for (int j=0;j<reg->nCubosDes;j++){
        if(fwrite(&cubo,sizeof(cubo),1,fHash)!=1){
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
        return -2;
    }

    FILE *fEntrada=fopen(fichEntrada, "rb");
    if(fEntrada==NULL){
        return -1;
    }

    FILE *fHash=fopen(fichHash, "r+b");
    if(fHash==NULL){
        fclose(fEntrada);
        return -2;
    }

    tipoReg reg;
    if(fread(&reg, sizeof(tipoReg), 1, fEntrada)!=1){
        return -1;
    }

    while(!feof(fEntrada)){
        if(insertar(fHash, &reg, regC)==-2){
            fclose(fHash);
            fclose(fEntrada);
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

    fclose(fHash);
    return 0;

}


int insertar(FILE *f, tipoReg *reg, regConfig *regC){
    tipoCubo cubo;
    
    for (int i = 0; i < regC->nCubos; i++){
        fseek(f, i*sizeof(tipoCubo), SEEK_SET);
        if(fread(&cubo, sizeof(tipoCubo), 1, f)!=1){
            return -2;
        }

        if(cubo.numRegAsignados < C){
            cubo.reg[cubo.numRegAsignados]=*reg;
            cubo.numRegAsignados++;
            fseek(f, i*sizeof(tipoCubo), SEEK_SET);
            if(fwrite(&cubo, sizeof(tipoCubo), 1, f)!=1){
                return -2;
            }
            regC->numReg++;
            return 0;
        }
    }
    
    
    return desborde(f,reg,regC);
    

}


int desborde(FILE *fHash, tipoReg *reg, regConfig *regC){
    tipoCubo cubo;
    long posicion=(regC->nCuboDesAct+regC->nCubos)*sizeof(tipoCubo);
    fseek(fHash,posicion, SEEK_SET);
    if(fread(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
        return -2;
    }
    
    if(cubo.numRegAsignados < C){
        //Hay espacio en el cubo de desborde
        cubo.reg[cubo.numRegAsignados]=*reg;
        regC->numReg++;
        cubo.numRegAsignados++;
        regC->numRegDes++;
        fseek(fHash,posicion, SEEK_SET);
        if(fwrite(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
            return -2;
        }

        //si se llena, siguiente cubo
        if(cubo.numRegAsignados==C){
            regC->nCuboDesAct++;
        }

    }else {
        //no hay espacio, nuevo cubo de desborde
        tipoCubo nuevo;
        nuevo.desbordado=0;
        nuevo.numRegAsignados=1;
        nuevo.reg[0]=*reg;
        

        fseek(fHash,0,SEEK_END);
        if(fwrite(&nuevo, sizeof(tipoCubo), 1, fHash)!=1){
            return -2;
        }

        regC->numReg++;
        regC->numRegDes++;
        regC->nCubosDes++;
        regC->nCuboDesAct=regC->nCubos+regC->nCubosDes-1;

        //poner como desbordado y escribir
        cubo.desbordado=1;
        fseek(fHash,posicion,SEEK_SET);
        if(fwrite(&cubo, sizeof(tipoCubo), 1, fHash)!=1){
            return -2;
        }
    }
    
    return 0;
}
