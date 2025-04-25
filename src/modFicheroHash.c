#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "../include/dispersion.h"

int main()
{ int nCubo,nCuboDes, posReg, error,bytesDesp;
  tipoReg *reg;
  char dni[9],provincia[11];
  int salir=1,i;
 

	#ifdef ALUM 
		error = buscar("../datos/alumnosC.hash","11592038");
		if (error)  printf("Error %d en la función bucar\n", error);
		error = modificar("../datos/alumnosC.hash","11592038","Sevilla");
		if (error)  printf("Error %d en la función modificar\n", error);	
		error = buscar("../datos/alumnosC.hash","11592038");

		
	#endif  	
	#ifdef ASIG
		error = buscar("../datos/asignaturasC.hash", 101116);
		if (error)  printf("Error %d en la función bucar\n", error);
		error= modificar("../datos/asignaturasC.hash", 101116,4.5, 1.5);
		if (error)  printf("Error %d en la función modificar\n", error);
		error = buscar("../datos/asignaturasC.hash", 101116);
	#endif

	if (error)  printf("Error %d en la función buscar\n", error);
}


