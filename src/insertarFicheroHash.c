#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "../include/dispersion.h"

int main(){
   int nCubo,nCuboDes, posReg, error,bytesDesp;
    tipoReg reg;
  
  int salir=1,i;
 

	#ifdef ALUM 
        strncpy(reg.dni, "1231234", sizeof(reg.dni));
        strncpy(reg.nombre, "Sandra", sizeof(reg.nombre));
        strncpy(reg.ape1, "Llavador", sizeof(reg.ape1));
        strncpy(reg.ape2, "Cano", sizeof(reg.ape1));
        strncpy(reg.provincia, "Badajoz", sizeof(reg.provincia));
		error = inserta("../datos/alumnosC.hash", &reg);
		if (error)  printf("Error %d en la función inserta\n", error);
		error = leeHash("../datos/alumnosC.hash");
		
	#endif  	
	#ifdef ASIG
        reg.codigo=12355;
        strncpy(reg.nombre, "Asigantura de Sandra III", sizeof(reg.nombre));
        reg.curso = 'S';           
        reg.creditosT = 3;      
        reg.creditosP = 3;
        reg.tipo = 'T';            
        reg.cuatrimestre = 'S';    
        reg.numGrT = 2;
        reg.numGrP = 5;
		error = inserta("../datos/asignaturasC.hash", &reg);
		if (error)  printf("Error %d en la función inserta\n", error);
		error = leeHash("../datos/asignaturasC.hash");
	#endif

	if (error)  printf("Error %d en la función buscar\n", error);	



}