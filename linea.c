#include <stdio.h>
#include<stdlib.h>
main(){
char *w = linea(1,"archivo.txt");
printf("\n%s\n",w);
free(w);
}
char *linea(int linea, char *filename){// Devuelve un puntero a la string
FILE *file;
//Abrir el archivo
if ((file = fopen(filename,"r")) == NULL){
//error("Problema Abriedo el archivo",-1);
return filename;
}
//Devuelve la línea del archivo con nombre filename como un puntero a un char

char c=' ';
int line=0;
int start=0;
int end=0;
while(line!=linea){
c=' ';
start=end;
while (c!='\n'){ //\n es el divisor de las lineas
fseek(file, 0, SEEK_CUR);
fread(&c,1,1,file);
end++;
if (feof(file)){
    //terminó de leer
    fclose(file); 
    //printf("FIN");
    return 0;
}
///printf("%c",c);
}
line++;
}
//En este punto ya se tiene la posicion del inicio y fin de la linea, solo queda guardarla en un arreglo
//printf("start:%d, end:%d\n",start,end);
const int longitud= end-start;
char *devuelve=(char*)malloc(sizeof(char)*longitud+1);//lONGITUD+1 PORQUE TAMBIEN SE TOMA EN CUENTA EL CARACTER NULL FINAL DEL STRING
//char devuelve[longitud];
int i=0;
fseek(file,start,SEEK_SET);
for (i=0;i<longitud;i++){
fseek(file,0,SEEK_CUR);
fread(&devuelve[i],1,1,file);
}
fclose(file);
//getchar();
return devuelve;
}