#include <stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<math.h>
//Los nombres de las bases de datos
char ch_nombre_de_base_de_datos[10]="datos.csv";
char ch_frases[11]="frases.txt";

//Definicion de las funciones
void vo_menu(void);
char *linea(int linea, char *filename);
int in_lineas(int primera,int final,char *archivo);
int* ordenar(char* base_de_datos,int edad);
int error(char *mensaje,int code);
int string_a_entero(char* str);
float string_a_float(char* str);
char* columna(char* linea,int num_columna);
float fl_comparar(float* medias, int CPK, int plaquetas, float creatinina, int sodio);
float obtener_riesgo(float valor, float min, float max, float newmin, float newmax, float overflow_max, float overflow_min);
void vo_start();
//Definicion del tipo de dato bool como un entero de 8 bits, que ocupa menos memoria
typedef uint8_t bool;
float *fl_W(int* datos,bool anemia, bool diabetes, bool presion_alta, bool hombre, bool fuma,int edad, int CPK, int plaquetas, float creatinina, int sodio);
int main(){
uint8_t z=0;
//Bucle infinito del menu
while (!z)
{ 
//Primero el menú
vo_menu();
char *w = linea(1,ch_frases);
printf("\n%s\n",w);
//Cada que se llama a linea, se de liberar, pues usa malloc
free(w);
int opcion;
//Recibe un numero del usuario
scanf("%i",&opcion);
switch (opcion){
    case 1:
        system("cat info.txt");
        break;
    case 2:
        system("cat proyecto.c");
        break;
    case 3:
        break;
    case 4:
        vo_start();
        break;
    case 5:
        z=1;
    default:
        z=1;
        break;
    
}
}
return 0;
}
void vo_start(){
    //Pedir los datos
    bool x[5];
    uint8_t edad;
    int si=0;
    //valor alto es peor
    int creatinine_phosphokinase=0; //crseatinina fosokinasa o CPK, segun el artículo, nivles altos indican irregularidades y mayor riesgo.
    int ejection_fraction=0; //Fraccion de eyeccion, mide la cantidad de sangre bombeada al momento de un ataque, no se utiliza
    //Si es menor aumenta el riesgo
    int platelets =0; //plaquetas
    //Alto es malo
    float serum_creatinine=0; //creatinina en la sangre
    //Nivel bajo es peor
    int serum_sodium=0;//Sodio en la sangre
    int time=0;//Indica el tiempo de seguimiento tras el ataque, no se utiliza
    printf("%s",linea(2,ch_frases)); 
    scanf("%hhi",&edad);
    for (int i=0;i<5;i++){
       char *str=linea(3+i,ch_frases);
       printf("%s",str); 
       free(str);
       scanf("%i",&si);
       x[i]= si;
    }
    //Tomar las otras medidas
    printf("\n%s",linea(8,ch_frases)); 
    scanf("%i",&creatinine_phosphokinase);
    printf("\n%s",linea(9,ch_frases)); 
    scanf("%i",&platelets);
    printf("\n%s",linea(10,ch_frases)); 
    scanf("%f",&serum_creatinine);
    printf("\n%s",linea(11,ch_frases)); 
    scanf("%i",&serum_sodium);
    //Tomar las edades mayores y menores de la base de datos
    //La base de datos aun no esta parseada, ni ordenada, asi que toca ordenar para obtener los datos necesarios
    int *orden = ordenar(ch_nombre_de_base_de_datos,edad);
    //Ya ordenada, se calculan las medias, pasando el arreglo que contiene las posiciones de los datos ordenados por edad
    float *fl_medias = fl_W(orden,x[0],x[1],x[2],x[3],x[4],edad,creatinine_phosphokinase,platelets,serum_creatinine,serum_sodium);
    //Aqui ya se calcularon las 4 medias de cada parametro, las cuales se van a comparar con los datos del usuario.
    float fl_riesgo_final=fl_comparar(fl_medias,creatinine_phosphokinase,platelets,serum_creatinine,serum_sodium);
    //Aqui solo se va a imprimir los resultados en pantalla
    char *salida;
    int i;
    i=17+(int)roundf(fl_riesgo_final);
    salida=linea(i,ch_frases);
    printf("\n%s",salida);
    free(salida);
}
float fl_comparar(float* medias, int CPK, int plaquetas, float creatinina, int sodio){
    //Aqui se comparan las 4 medias con lo valores del usuario    
    //Para compararlas se obtine el valor maximo y minimo de los 4 parametros en la base de datos y se les asigna el valor de 5 y 3 respectivamente
    //El valor medio es 4, un valor menor separado por el mismo intervalo es 2, etc.
    //Tambien hay que tomar en cuenta si los valores altos son peores o mejores
    int i=0;
    while (1){
        char *c=linea(i,ch_nombre_de_base_de_datos);    
        if (c==0)break;
        free(c);
        i++;
    }
    int in_database_size=i-2;
    int in_min_max_CPK[2]={10000000,0};//Valor mayor es peor
    float fl_min_max_plaquetas[2]={10000000,0};//Menor es peor
    float fl_min_max_creatinina[2]={10000000,0};//Mayor es peor
    int in_min_max_sodio[2]={1000000,0};//Menor es peor
    //Obtiene el valor maximo y minimo de cada parametro
    for (i=2;i<in_database_size;i++){ //i=2 porque la primera linea es de texto
    char* t = linea(i,ch_nombre_de_base_de_datos);
    char *c=columna(t,3);
    int x =string_a_entero(c);
    if(in_min_max_CPK[0]>x)in_min_max_CPK[0]=x;
    if(in_min_max_CPK[1]<x)in_min_max_CPK[1]=x;
    free(c);
    c=columna(t,7);
    float y =string_a_float(c);
    if(fl_min_max_plaquetas[0]>y)fl_min_max_plaquetas[0]=y;
    if(fl_min_max_plaquetas[1]<y)fl_min_max_plaquetas[1]=y;
    free(c);
    c=columna(t,8);
    y =string_a_float(c);
    if(fl_min_max_creatinina[0]>y)fl_min_max_creatinina[0]=y;
    if(fl_min_max_creatinina[1]<y)fl_min_max_creatinina[1]=y;
    free(c);
    c=columna(t,9);
    x =string_a_entero(c);
    if(in_min_max_sodio[0]>x)in_min_max_sodio[0]=x;
    if(in_min_max_sodio[1]<x)in_min_max_sodio[1]=x;
    free(c);
    free(t);
    }
    //Ahora, tenemos dos intervalos, el que va de [minimo,maximo] y el que va de [1,5]
    //Recordar que al valor minimo se le va a asignar un riesgo de 2, a la mediana de los datos un 3.5, y al maximo un riesgo de 5
    //Para valores que se salen del rango, si son mayores al maximo, se mantiene el riesgo en 5, si son menores al minimo, se baja a 2 y a 1, de manera lineal
    //         |Rango de|
    //         |datos  |
    // |-------| ------|
    //        min     max
    // |---|---|---|---|  -->Rango de valores
    // 1   2   3   4   5  -->Riesgo
    //Para hacer la conversion a un rango de 1-5, se obtiene un factor de proporcion
    float riesgo[4];
    //CPK
    riesgo[0]=obtener_riesgo((float)CPK,(float)in_min_max_CPK[0],(float)in_min_max_CPK[1],3.0,5.0,1.0,5.0);
    //printf("\nMin CPK: %i, Max CPK: %i, User Value: %i, Risk [1-5]: %f",in_min_max_CPK[0],in_min_max_CPK[1],CPK,riesgo[0]);
    //Plaquetas
    riesgo[1]=obtener_riesgo((float)plaquetas,fl_min_max_plaquetas[0],fl_min_max_plaquetas[0],3.0,5.0,1.0,5.0);
    //Como este es un valor al que si es menor implica mas riesgo, hay que invertir el riesgo
    riesgo[1]=6-riesgo[1];
    ///printf("\nMin Plaquetas: %f, Max Plaquetas: %f, User Value: %i, Risk [1-5]: %f",fl_min_max_plaquetas[0],fl_min_max_plaquetas[1],plaquetas,riesgo[1]);
    //Creatinina
    riesgo[2]=obtener_riesgo(creatinina,fl_min_max_creatinina[0],fl_min_max_creatinina[0],3.0,5.0,1.0,5.0);
    //printf("\nMin Creatinina: %f, Max Creatinina: %f, User Value: %f, Risk [1-5]: %f",fl_min_max_creatinina[0],fl_min_max_creatinina[1],creatinina,riesgo[2]);
    //Sodio
    riesgo[3]=obtener_riesgo((float)sodio,(float)in_min_max_sodio[0],(float)in_min_max_sodio[1],3.0,5.0,1.0,5.0);
    //Como este es un valor al que si es menor implica mas riesgo, hay que invertir el riesgo
    riesgo[3]=6-riesgo[3];
    //printf("\nMin Sodio: %i, Max Sodio: %i, User Value: %i, Risk [1-5]: %f",in_min_max_sodio[0],in_min_max_sodio[1],sodio,riesgo[3]);
    //Hasta aqui ya estan los 4 riesgos en las diferentes areas, ahora, solo hay que darle al usuario el mayor de estos 4 riesgos
    float fl_retorna=riesgo[0];
    fl_retorna<riesgo[1] ? riesgo[1]: fl_retorna;
    fl_retorna<riesgo[2] ? riesgo[2]: fl_retorna;
    fl_retorna<riesgo[3] ? riesgo[3]: fl_retorna;
    fl_retorna<riesgo[4] ? riesgo[4]: fl_retorna;
    return fl_retorna;
}
float obtener_riesgo(float valor, float min, float max, float newmin, float newmax, float overflow_min, float overflow_max){
    //Convierrte un rango de valores en otro, tambien se podria haber usado map(), pero aqui se pone un limite del que no se puede salir [1-5]
    float factor = (newmax-newmin)/(max-min);
    float riesgo = ((valor-min)*factor)+newmin;
    if (riesgo<overflow_min)riesgo=overflow_min;
    if (riesgo>overflow_max)riesgo=overflow_max;
    return riesgo;
}
float *fl_W(int* datos,bool anemia, bool diabetes, bool presion_alta, bool hombre, bool fuma,int edad, int CPK, int plaquetas, float creatinina, int sodio)
{
    //Asignar los pesos, dependiendo de os datos que coincidan
    int i=0;
    while (datos[i]!=0)i++;
    const int in_sizeof_datos=i;
    int database_age[in_sizeof_datos];
    bool database_anemia[in_sizeof_datos];
    int database_CPK[in_sizeof_datos];
    bool database_diabetes[in_sizeof_datos];
    bool database_presion_alta[in_sizeof_datos];
    float database_plaquetas[in_sizeof_datos];
    float database_creatinina[in_sizeof_datos];
    int database_sodio[in_sizeof_datos];
    bool database_sexo[in_sizeof_datos];
    bool database_fuma[in_sizeof_datos];
    bool database_muerte[in_sizeof_datos];
    float W[in_sizeof_datos];
    for (i=0;i<in_sizeof_datos;i++){
        char* t = linea(datos[i],ch_nombre_de_base_de_datos);
        //Recorrer la linea, convertir cada valor a su tipo de dato, detenerse en las coma y guardarlo en su respectivo arreglo
        char* c=columna(t,1);
        database_age[i]=string_a_entero(c);
        free(c);
        c=columna(t,2);
        database_anemia[i]=(bool)string_a_entero(c);
        free(c);
        c=columna(t,3);
        database_CPK[i]=string_a_entero(c);
        free(c);
        c=columna(t,4);
        database_diabetes[i]=(bool)string_a_entero(c);
        free(c);
        c=columna(t,6);
        database_presion_alta[i]=(bool)string_a_entero(c);
        free(c);
        c=columna(t,7);
        database_plaquetas[i]=string_a_float(c);
        free(c);
        c=columna(t,9);
        database_sodio[i]=string_a_entero(c);
        free(c);
        c=columna(t,10);
        database_sexo[i]=(bool)string_a_entero(c);
        free(c);
        c=columna(t,11);
        database_fuma[i]=(bool)string_a_entero(c);
        free(c);
        c=columna(t,13);
        database_muerte[i]=(bool)string_a_entero(c);
        free(c);
        c=columna(t,8);
        database_creatinina[i]=string_a_float(c);
        free(c);
        free(t);
        //Hacer las comparaciones para asignar los pesos a esta linea
        W[i]=1;
        if (database_anemia[i]==anemia)W[i]+=in_sizeof_datos*.05;
        if (database_diabetes[i]==diabetes)W[i]+=in_sizeof_datos*.05;
        if (database_presion_alta[i]==presion_alta)W[i]+=in_sizeof_datos*.05;
        if (database_sexo[i]==hombre)W[i]+=in_sizeof_datos*.05;
        if (database_fuma[i]==fuma)W[i]+=in_sizeof_datos*.05;
    }
    //Obtener la fl_media de llos daos relevantes
    float* fl_media=(float*)malloc(4*sizeof(float));
    for (i=0;i<4;i++)fl_media[i]=0;
    float Wtotal=0;
    for (i=0;i<in_sizeof_datos;i++){
        Wtotal+=W[i];
        fl_media[0]+=database_CPK[i];
        fl_media[1]+=database_plaquetas[i];
        fl_media[2]+=database_sodio[i];
        fl_media[3]+=database_creatinina[i];
    }
    for (i=0;i<4;i++)fl_media[i]/=Wtotal;
    //memecounter para contar la memoria alocada
    //printf("\n%ld", malloced_memory_usage);
    return fl_media;
}
char* columna(char* linea,int num_columna){
    //Devuelve la columna de la linea, tomando una , como el separador
    //Iterar los caracteres de la linea, hasta que se encuentre una coma
    int largo=0, c=0, i=0, d=0, e=0;
    for (i=0;c<num_columna;i++){
        if (linea[i]==',' | linea[i]=='\n'){
            c++;
            if (c==num_columna-1)d=i;
            else if (c==num_columna){
                e=i;
                break;
                }
        }
    }
    if (num_columna!=1)d++;//quitar la coma que sale al inicio
    largo=e-d;
    char *devuelve = (char*)malloc(largo+1);
    for (i=d;i<e;i++)devuelve[i-d]=linea[i];
    //Fin de la linea, es ncesario, sino se confunde al programa
    devuelve[largo]='\0';
    return devuelve;
}
float string_a_float(char* str){
    return atof(str);
}
int string_a_entero(char* str){
    return atoi(str);
}
int* ordenar(char* base_de_datos,int edad){
    int i=0;
    //Crear un array conteniendo un par de numeros: la edad y la linea en que se encuentra
    //Contar las lineas en el archivo
    while (1){
        char *c=linea(i,base_de_datos);    
        if (c==0){
            //Ya se tiene el tamaño de la base de datos
            break;
        }
        free(c);
        i++;
    }
    const int database_size=i-2;
    //cada entrada del arreglo tiene dos datos: el numero para ordenar (edad) y la linea en que se encuentra
    int arreglo_a_ordenar[database_size][2];
    i=2;
    while(1){
    char *c=linea(i,base_de_datos);
    if (c==0)break;
    //El rango de edades solo es de dos dígitos, ademas esta primero en la base de datos
    char e[2]={c[0],c[1]};
    free(c);
    int edad= string_a_entero(e);
    arreglo_a_ordenar[i-2][0]=edad;
    arreglo_a_ordenar[i-2][1]=i;
    i++;
    }

    //////////Aqui comienza el ordenamiento//////////
    ///Se utilizara insertion sort, que tiene  complejidad de O(n^2)
    i = 1;
    while (i < database_size){
    int j = i;
    while (j > 0 && arreglo_a_ordenar[j-1][0] > arreglo_a_ordenar[j][0]){
        int temp[2];
        temp[0] = arreglo_a_ordenar[j][0];
        temp[1] = arreglo_a_ordenar[j][1];
        arreglo_a_ordenar[j][0]=arreglo_a_ordenar[j-1][0];
        arreglo_a_ordenar[j][1]=arreglo_a_ordenar[j-1][1];
        arreglo_a_ordenar[j-1][0]=temp[0];
        arreglo_a_ordenar[j-1][1]=temp[1];
        j--;
    }
    i++;
    }
    ////Aqui el arreglo ya esta ordenado, ahora hay que filtrar las edades, devolviendo los datos para esa edad, una edad menor y una edad mayor
    int w=0;
    int diferentes_edades=0;
    for (i=0;i<database_size;i++){
        if (w!=arreglo_a_ordenar[i][0]){
            w=arreglo_a_ordenar[i][0];
            diferentes_edades++;
        }
    }
    const int d = diferentes_edades;
    int edades[d];
    diferentes_edades=0;
    w=0;
    for (i=0;i<database_size;i++){
        if (w!=arreglo_a_ordenar[i][0]){
            w=arreglo_a_ordenar[i][0];
            edades[diferentes_edades]=arreglo_a_ordenar[i][0];
            diferentes_edades++;
        }
    }
    int min_distancia=database_size+1;
    int min,x,max;
    for (i=0;i<d;i++){
        if (min_distancia>abs(edades[i]-edad)){
            min_distancia=abs(edades[i]-edad);
            if (i!=0)min=edades[i-1];
            else min=edades[i];
            x=edades[i];
            if (i!=d)max=edades[i+1];
            else max=edades[d];
        }
    }
    //Buscar y retornar un arreglo con las líneas en que se encuentran los datos para el rango de edad [min,max]
    int m,M;
    for (i=0;i<database_size;i++){
        if (arreglo_a_ordenar[i][0]==min){
            m=i;
            break;
        }
    }
    for(i=m;i<database_size;i++){
        if (arreglo_a_ordenar[i][0]==max){
            M=i;
            break;
        }
    }
    const int data_length=M-m;

    int* retorna=(int*)malloc((data_length+1)*sizeof(int));
    for (i=m;i<M;i++){
        retorna[i-m]=arreglo_a_ordenar[i][1];
    }
    //Asi se puede marcar el final del arreglo, pues no hay una linea 0
    retorna[data_length]=0;
    //printf("\n%ld", malloced_memory_usage);
    return retorna;  
}
int in_lineas(int primera,int final,char *archivo){
    //Imprime el rango de lineas indicado
    int i=0;
    for (i=primera;i<=final;i++){
        char *c=linea(i,archivo);
    if((int)c[0]!=0){
        printf("%s",c);
        free(c);
        }
    else break;
    } 
    return i;
}
int error(char *mensaje,int code){
//Funcion de error, por si hay algun error para abrir el archivo
printf("%s\n",mensaje);
exit(code);
return code;
}
char *linea(int linea, char *filename){
//Guarda la linea indicada del archivo en un puntero
FILE *file;
if ((file = fopen(filename,"r")) == NULL){
error("Problema Abriedo el archivo",-1);
return filename;
}
//Devuelve la línea del archivo con nombre filename como un puntero a un char
char c;
int line=0, start=0, end=0;
while(line!=linea){
c=' ';
start=end;
//Iterar hasta llegar al caracter de fin delinea
while (c!='\n'){
fseek(file, 0, SEEK_CUR);
fread(&c,1,1,file);
end++;
if (feof(file)){
    //terminó de leer
    fclose(file); 
    return 0;
}
}
line++;
}
const int longitud= end-start;
char *devuelve=(char*)malloc(sizeof(char)*longitud+1);//lONGITUD+1 PORQUE TAMBIEN SE TOMA EN CUENTA EL CARACTER NULL FINAL DEL STRING
fseek(file,start,SEEK_SET);
for (int i=0;i<longitud;i++){
fseek(file,0,SEEK_CUR);
fread(&devuelve[i],1,1,file);
}
fclose(file);
devuelve[longitud]='\0';//Caracter nulo para finalizar el string
return devuelve;
}
void vo_menu(){
//Imprime en pantalla todas las entradas del menu, guardadas en menu.txt
in_lineas(12,17,ch_frases);
}
