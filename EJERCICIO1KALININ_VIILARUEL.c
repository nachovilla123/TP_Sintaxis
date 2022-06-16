//BY IGNACIO VILLARRUEL AND ALEXANDER KALININ!

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int esPalabra2 (const char *s); 
int columna(char letra);
void mensajeTipoDePalabra(int numero);       

static int tablaDeTransicion[8][7] = {
//            {0}[1-7][8-9][a-fA-F][xX]{no rec} {signo}.
      /* q0  */{ 1, 2, 2, 6, 6, 6 , 7},
      /* q1+ */{ 5, 5, 6, 6, 3, 6 , 6}, // Estado final 1 Octal (cero)                                 
      /* q2+ */{ 2, 2, 2, 6, 6, 6 , 6}, // Estado final 2 de Decimal
      /* q3  */{ 4, 4, 4, 4, 6, 6 , 6}, // Transicion de 0 a [xX]
      /* q4+ */{ 4, 4, 4, 4, 6, 6 , 6}, // Estado final 4 de Hexadecimal
      /* q5+ */{ 5, 5, 6, 6, 6, 6 , 6}, // Estado final 5 de Octal
      /* q6+ */{ 6, 6, 6, 6, 6, 6 , 6}, // Estado final 3,6 No se reconoce
      /* q7  */{ 2, 2, 2, 6, 6, 6 , 6} //  Estado De signo para el decimal                               
};

// DECLARACION DE VARIABLES GLOBALES PARA SER CONTADORES DE LOS CASOS
int cantDecimal = 0,cantOctal = 0,cantHexadecimal =0 , cantErrorLexico = 0;


int main(){
	
    int j = 0, i = 0, k = 0,estadoQueDevuelve = 0,contadorParaVaciarVectorAux = 0 ;
     
    char palabraAux[100]={""};
    char palabra[100]="0x1&r";     // ACA SE METE CADENA A ANALIZAR
       
    do{
       if (palabra[k] != '&' ){
            
        palabraAux[i] = palabra[j];   
            i++;
            j++;    
            k++;
			contadorParaVaciarVectorAux++;      
        }
		else{      //  termino la palabra
        estadoQueDevuelve = esPalabra2(palabraAux);   
   
		mensajeTipoDePalabra(estadoQueDevuelve); // Printea La clasificacion de la cadena y aumenta en 1 su tipo
		
        //Reiniciamos la palabra auxiliar para volver a cargar desde la posicion 0 los nuevos caracteres
		while(contadorParaVaciarVectorAux>0){
			palabraAux[contadorParaVaciarVectorAux-1]="";
			contadorParaVaciarVectorAux--;
		}
        i=0; // reinicio indice para palabraAux  
        j++; // me ubico en siguiente caracter despues de &
        k++; // me ubico en siguiente caracter de mi palabra original
        
        } 
        
    }while(palabra[j] != '\0');
    
    estadoQueDevuelve = esPalabra2(palabraAux);
    mensajeTipoDePalabra(estadoQueDevuelve);


printf("\n\nMOSTRANDO DATOS FINALES: \n");
printf("CANTIDAD DE DECIMALES:       %d \n",cantDecimal);
printf("CANTIDAD DE OCTALES:         %d \n",cantOctal);
printf("CANTIDAD DE HEXADECIMALES:   %d\n",cantHexadecimal);
printf("CANTIDAD DE ERRORES LEXICOS: %d\n",cantErrorLexico);

    return 0;
} 

int esPalabra2 (const char *s){ /* Automata 2 */
                      
    int e; /* estado actual del automata */
    unsigned int i; /* recorre la cadena */

for (e=0,i=0; s[i]!='\0'; i++){   

    e = tablaDeTransicion [e][columna(s[i])];
}

return e; /* Retorna el estado final */
} /* fin EsPalabra2 */


int columna(char letra){ 
   int columnaADevolver = 0;
   switch(letra)
   {
    case '0': // Caso de 0
        columnaADevolver = 0;
        break;

    case 49 ... 55: // Caso de 1-7
        columnaADevolver = 1;
        break;

    case 56 ... 57: // Caso de 8-9
        columnaADevolver = 2;
        break;

    case 65 ... 70: // Caso de A-F (mayuscula)
    case 97 ... 102: // Caso de a-f (minuscula)
        columnaADevolver = 3;
        break;

    case 'X': // Caso x (minuscula)
    case 'x': // Caso X (mayuscula)
        columnaADevolver = 4;
        break;

    case '+': // Caso +  (signos)
    case '-': // Caso -
        columnaADevolver = 6;
        break;

    default: // Caso de Error, si no devuelve ninguno de los casos anteriores: {no rec} en tabla
        columnaADevolver = 5;
        break;
   }
   return columnaADevolver;
}


void mensajeTipoDePalabra(estadoQueDevuelve){ 
	switch(estadoQueDevuelve){
    case 1:
        printf("Es octal , palabra vacia \n ");
        cantOctal ++;
        break;
    case 2:
        printf("Es decimal\n"); 
        cantDecimal++;
        break;
    case 3:
        printf("Es hexadecimal, palabra vacia\n");
        cantHexadecimal++;
        break;
    case 4:
        printf("Es hexadecimal\n");
        cantHexadecimal++;
        break;
    case 5:
        printf("Es octal\n");
        cantOctal++;
        break;        
    case 6:
        printf("No pertenece al lenguaje\n");
        cantErrorLexico++;
        break;
}
}
