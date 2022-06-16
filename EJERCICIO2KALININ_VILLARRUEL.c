//BY IGNACIO VILLARRUEL AND ALEXANDER KALININ!
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h> //atoid

int esPalabra2 (const char *s); 
int columna(char letra);
void mensajeTipoDePalabra(int numero);   

int resultadoDeLaOperacion(int);
void reconocerCadena();

// DEFINICION DE VARIABLES GLOBALES 
int cantDecimal = 0,cantOctal = 0,cantHexadecimal =0 , cantErrorLexico = 0;
int j = 0, i = 0, k = 0, resultadoAcumulado;
char palabra[100]="1*2";    
	

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

int main(){
	
    reconocerCadena();
	if (cantOctal == 0 && cantHexadecimal == 0 && cantErrorLexico == 0 ){
		printf("RESULTADO FINAL %d \n ",resultadoDeLaOperacion(resultadoAcumulado) );
		}
		else {
		printf("\n CADENA INVALIDA , INGRESE UNA CADENA VALIDA.\n ");
	}
	
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
        cantOctal ++;
        break;
    case 2:
        cantDecimal++;
        break;
    case 3:
        cantHexadecimal++;
        break;
    case 4:
        cantHexadecimal++;
        break;
    case 5:
        cantOctal++;
        break;        
    case 6:
        cantErrorLexico++;
        break;
}
}

void reconocerCadena(){
	
    char palabraAux[100]={""};
    char palabraAuxEj2[100]={""};  
	int caracteresUsados = 0, caracteresUsados2,estadoQueDevuelve = 0;

    // OBTENGO LA LONGITUD DE CARACTERES USADOS Y me guardo la palabra en una nueva 
    while (palabra[caracteresUsados] != '\0'){
        palabraAuxEj2[i] = palabra[j];   
        i++;
        j++;    
        caracteresUsados++;  
    }   
    caracteresUsados2=caracteresUsados;
  
// HACIENDO EL REEMPLAZO DE + - * POR &
    while(caracteresUsados>=0){
            if(palabraAuxEj2[caracteresUsados-1] == '+'  ){  
                palabraAuxEj2[caracteresUsados-1] = '&';
            }
			else if(palabraAuxEj2[caracteresUsados-1] == '-'  ){  
                palabraAuxEj2[caracteresUsados-1] = '&';
            }
            else if(palabraAuxEj2[caracteresUsados-1] == '*'  ){  // AMPERSAN ES 38         
                palabraAuxEj2[caracteresUsados-1] = '&'; 
            }
			caracteresUsados--;
		}

    i=0, j=0;
    //********************************** CODIGO RECONOCEDOR EJ 1 *************************//
    do{
       if (palabraAuxEj2[k] != '&' ){
        palabraAux[i] = palabraAuxEj2[j];   
            i++;
            j++;    
            k++;     
        }
		else{      //  termino la palabra
        estadoQueDevuelve = esPalabra2(palabraAux);   
		mensajeTipoDePalabra(estadoQueDevuelve); 
		memset(palabraAux,0,100);

        i=0; // reinicio indice para palabraAux  
        j++; // me ubico en siguiente caracter despues de &
        k++; // me ubico en siguiente caracter de mi palabra original
        } // cuando  J sale de esa estructura tiene el tamaño total de la palabra 
    }while(palabraAuxEj2[j] != '\0');

    estadoQueDevuelve = esPalabra2(palabraAux);
    mensajeTipoDePalabra(estadoQueDevuelve);

	
}

int resultadoDeLaOperacion(resultadoAcumulado){
	
	char palabraConA[100]={""};
	char palabraConSumasYRestas[100]={""};
	char caracteresAnterioresInvertidos[100]={""};
	char caracteresPosteriores[100]={""};
	
	int resultadoMultiplicacion = 0,moverposicionporsigno = 0,signo = 1, resto=0,caracteresAnterioresEnterosINV=0,caracteresAnterioresEnteros = 0 , caracteresPosterioresEnteros = 0;

	//********************** DECLARACION DE VARIABLES LOGICA SUMA **********************
	char vectorAuxiliarSuma[100]={""}; 
    int valor = 0,posicionVectorAuxiliarSuma = 0, PosicionPalabraAnalizando = 0;
	int cantidadElementosUsados = 0;
	

	printf("\n CADENA VALIDA , PROCEDIENDO A REALIZAR LA OPERACION...\n ");	
		
	while(palabra[cantidadElementosUsados] != '\0' ){
			cantidadElementosUsados++;      
        }

	strcpy(palabraConA,palabra);  //utilizar palabraConA para guardar la cadena sin las opreacinoes de multiplicacion

    for( k=0; k < cantidadElementosUsados ; k++ ){
          j=0;
          if(palabra[k]=='*'){
          	
              i=k;  j=0;                                      
            
            do{ 
                if(palabra[i-1] != '+' && palabra[i-1] != '-'){    
            
                  caracteresAnterioresInvertidos[j] = palabra[i-1];
                  palabra[i]='a';                                               
				  palabraConA[i-1]='a';
                    j++;  
                    i--;
                  } else{
                      i--;
                    }

          }while(palabra[i] != '+' &&  palabra[i] != '-' && i>=0);
          
            if(palabra[i] == '-'){
             palabraConA[i]='a';	
              signo = -1;           
            }else{
              signo = 1;
            }  
      
  /////////////////////////////////////////////// HACIENDO PARA LOS CARACTERES POSTERIORES ///////////////////////////////
              i=k, j=0;
              do{ 
                
                if(palabra[i+1] != '+' && palabra[i+1] != '-'){
                      
                  caracteresPosteriores[j] = palabra[i+1]; 
                  palabraConA[i]='a';
				  palabraConA[i+1]='a'; 
                  j++;
                  i++;
                  } else{
                          i++;
                		}
          }while(palabra[i] != '+' &&  palabra[i] != '-' && palabra[i] != '\0');   

    caracteresAnterioresEnterosINV = atoi(caracteresAnterioresInvertidos);  // Convierto el vectorAnterioresInv de chars a entero

	//Algoritmo para invertir un entero
   while(caracteresAnterioresEnterosINV>0){
    	resto                          = caracteresAnterioresEnterosINV%10;
    	caracteresAnterioresEnterosINV = caracteresAnterioresEnterosINV/10;
    	caracteresAnterioresEnteros    = caracteresAnterioresEnteros*10+resto;	
	  }

    caracteresPosterioresEnteros = atoi(caracteresPosteriores); 
    resultadoMultiplicacion      = signo * caracteresAnterioresEnteros * caracteresPosterioresEnteros;
    resultadoAcumulado           = resultadoAcumulado + resultadoMultiplicacion;
    
	memset(caracteresPosteriores,0,100);
	memset(caracteresAnterioresInvertidos,0,100);
	
          } // MUERE IF
      } // muere el for
	
	i=0;j=0;

	for(i; i<cantidadElementosUsados; i++){ //GUARDAMOS EN UN VECTOR LA OPERACION RESTANTE A RESOLVER
		if(palabraConA[i]!= 'a' ){ 
			palabraConSumasYRestas[j]=palabraConA[i];
			j++;
		}
	}
	
// COMIENZA LA IMPLEMENTACION DEL CODIGO PARA RESOLVER LA SUMA   

while(palabraConSumasYRestas[PosicionPalabraAnalizando] !='\0'){
	

    	posicionVectorAuxiliarSuma=0;
    	
    	while(palabraConSumasYRestas[PosicionPalabraAnalizando] != '+' && palabraConSumasYRestas[PosicionPalabraAnalizando] != '-' && palabraConSumasYRestas[PosicionPalabraAnalizando] !='\0'){
    		vectorAuxiliarSuma[posicionVectorAuxiliarSuma + moverposicionporsigno] = palabraConSumasYRestas[PosicionPalabraAnalizando]; 
        	posicionVectorAuxiliarSuma++;
        	PosicionPalabraAnalizando++;
		}

		valor = atoi(vectorAuxiliarSuma);
    	resultadoAcumulado = resultadoAcumulado + valor;
		
        memset(vectorAuxiliarSuma,0,100);
	    memset(vectorAuxiliarSuma,0,100);
	    memset(vectorAuxiliarSuma,0,100);
    	vectorAuxiliarSuma[0] = palabraConSumasYRestas[PosicionPalabraAnalizando]; // guardo el +- que leyo en while
        PosicionPalabraAnalizando++;                                                                   
    	moverposicionporsigno = 1;		 
			
	}
	printf("\n EL RESULTADO DE LA OPERACION ASIGNADA ES : %d\n",resultadoAcumulado );		
			
		return resultadoAcumulado;
}
