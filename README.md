# MiniCpp-Scanner-Parser
Analizador léxico y sintáctico de un lenguaje basado en C++ utilizado para la implementación del algoritmo de eliminacion de subexpresiones comunes (CSE)

El lenguaje sigue la siguiente sintaxis:
```
< PROGRAM >            - >  < DECLARATION >   
< DECLARATION  >       - >  < TYPE > ID LEFT_PARENTHESE RIGHT_PARENTHESE < COMPOUNT_STMT >
< TYPE >               - >  VOID | INT
< COMPOUNT_STMT >      - >  LEFT_BRACE < LOCAL_DECLARATIONS > < STATEMENT_LIST > RIGHT_BRACE
< LOCAL_DECLARATIONS > - >  < VAR_DECLARATION >*
< VAR_DECLARATION >    - >  < TYPE > ID SEMICOLON
< STATEMENT_LIST >     - >  < STATEMENT >*
< STATEMENT >          - >  < RETURN_STMT > | < EXPRESSION_STMT >
< EXPRESSION_STMT >    - >  < ASSIGN_EXPRESSION > SEMICOLON
< RETURN_STMT >        - >  RETURN < EXPRESSION > SEMICOLON
< ASSIGN_EXPRESSION >  - >  < FACTOR > ASSIGN < SIMPLE_EXPRESSION >
< SIMPLE_EXPRESSION >  - >  < TERM > ( < ADDOP > < TERM > )*
< ADDOP >              - >  PLUS | MINUS
< TERM >               - >  < FACTOR > ( < MULOP > < FACTOR > )*
< MULOP >              - >  MULTI | DIV
< FACTOR >             - >  LEFT_PARENTHESE < EXPRESSION > RIGHT_PARENTHESE | ID | NUM
```

En la carpeta test hay varios input para testear el programa. El programa generará un codigo de salida el cual tendrá eliminadas las subexpresiones comunes.
Ejecutando el comando:
```
make run
```
Se ejecutará el programa sobre todos los archivos de codigo dentro de la carpeta test, y se guardarán las salidas en la carpeta output.

Para solamente compilar el programa:
```
 make
````
Y despues para ejecutar el programa con un codigo empecifico ejecutar:
```
 ./bin/cse file
```

Puesto que en el parser se genera el arbol sintactico del código, modificando el main podremos mostrarlo por pantalla.

Para más especificacion sobre el proyecto, lee el archivo `FinalDoc-CSE.pdf`

**_PROYECTO REALIZADO PARA LA ASIGNATURA COMPILING TECHNIQUES CURSADA EN LA UNIVERSIDAD POLITECHNIKA WARSZAWSKA_**
