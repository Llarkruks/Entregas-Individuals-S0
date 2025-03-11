#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  

// Función para verificar si una cadena es palíndromo
int es_palindromo(const char *cadena) {
    int inicio = 0, fin = strlen(cadena) - 1;
    
    while (inicio < fin) {
        if (cadena[inicio] != cadena[fin])
            return 0; // No es palíndromo
        inicio++;
        fin--;
    }
    return 1; // Es palíndromo
}

int main() {
    char entrada[200] = "1/Ana/2/Miguel/1/otto/2/Juan";
    char resultado[300] = "";
    
    // Separar la cadena por "/"
    char *token = strtok(entrada, "/");

    while (token != NULL) {
        int codigo = atoi(token);  
        token = strtok(NULL, "/");
        
        if (token == NULL) break;  // Evitar errores si falta información

        char nombre[50];
        strcpy(nombre, token);

        char temp[100];  // Almacenar el resultado temporalmente
        
        if (codigo == 1) {
            // Verificar si es palíndromo
            sprintf(temp, "[%s: %s palíndromo] - ", nombre, es_palindromo(nombre) ? "ES" : "NO ES");
        } 
        else if (codigo == 2) {
            // Convertir a mayúsculas
            char mayusculas[50];
            for (int i = 0; nombre[i] != '\0'; i++) {
                mayusculas[i] = toupper(nombre[i]);
            }
            mayusculas[strlen(nombre)] = '\0';
            sprintf(temp, "[%s] - ", mayusculas);
        } 
        else {
            sprintf(temp, "[ERROR: código %d inválido] - ", codigo);
        }

        strcat(resultado, temp);
        token = strtok(NULL, "/");
    }

    // Eliminar el último " - " si existe
    int longitud = strlen(resultado);
    if (longitud > 3)
        resultado[longitud - 3] = '\0';

    printf("Salida final:\n%s\n", resultado);
    return 0;
}
