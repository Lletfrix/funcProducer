#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXBUFF 100
#define CLRDATA 6
#define SEPDATA strlen("\n")

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))


#define BL "  "
#define CL1 "\x1B[30m"
#define CL2 "\x1B[31m"
#define CL3 "\x1B[32m"
#define CL4 "\x1B[33m"
#define CL5 "\x1B[34m"
#define CL6 "\x1B[35m"
#define CL7 "\x1B[36m"
#define CL8 "\x1B[37m"
#define CL9 "\x1B[90m"
#define CL10 "\x1B[91m"
#define CL11 "\x1B[92m"
#define CL12 "\x1B[93m"
#define CL13 "\x1B[94m"
#define CL14 "\x1B[95m"
#define CL15 "\x1B[96m"
#define CL16 "\x1B[97m"
#define RST "\x1B[0m"
#define PX "\u2588\u2588"

typedef struct {
  unsigned int width;
  unsigned int height;
  char* pixels;
} Image;

Image* image_ini(){
  Image* img;
  img = calloc(1, sizeof(Image));
}

void image_destroy(Image* img){
  if (!img) return;
  if(img->pixels)
    free(img->pixels);
  free(img);
}

void print_header(FILE* fp, const char* filename) {
  fprintf(fp,"#include <stdio.h>\n");
  fprintf(fp,"#include <stdlib.h>\n\n");
  fprintf(fp,"#define BL \"  \"\n");
  fprintf(fp,"#define CL1 \"\\x1B[30m\"\n");
  fprintf(fp,"#define CL2 \"\\x1B[31m\"\n");
  fprintf(fp,"#define CL3 \"\\x1B[32m\"\n");
  fprintf(fp,"#define CL4 \"\\x1B[33m\"\n");
  fprintf(fp,"#define CL5 \"\\x1B[34m\"\n");
  fprintf(fp,"#define CL6 \"\\x1B[35m\"\n");
  fprintf(fp,"#define CL7 \"\\x1B[36m\"\n");
  fprintf(fp,"#define CL8 \"\\x1B[37m\"\n");
  fprintf(fp,"#define CL9 \"\\x1B[90m\"\n");
  fprintf(fp,"#define CL10 \"\\x1B[91m\"\n");
  fprintf(fp,"#define CL11 \"\\x1B[92m\"\n");
  fprintf(fp,"#define CL12 \"\\x1B[93m\"\n");
  fprintf(fp,"#define CL13 \"\\x1B[94m\"\n");
  fprintf(fp,"#define CL14 \"\\x1B[95m\"\n");
  fprintf(fp,"#define CL15 \"\\x1B[96m\"\n");
  fprintf(fp,"#define CL16 \"\\x1B[97m\"\n");
  fprintf(fp,"#define RST \"\\x1B[0m\"\n");
  fprintf(fp,"#define PX \"\\u2588\\u2588\"\n\n");
  fprintf(fp, "#define clear() printf(\"\\033[H\\033[J\")\n");
  fprintf(fp, "void gt(int x, int y){\n printf(\"\\033[%%d;%%dH\", x, y);\n}");
  fprintf(fp,"void %s_print (int x_0, int y_0){\n\n", filename);
}

void print_header2(FILE*fp, const char* filename){
  fprintf(fp,"void %s_print (int x_0, int y_0){\n\n", filename);
}

void print_end(FILE* fp){
  fprintf(fp,"\n}");
}

int read_pixels (const char* filename, Image* img){
  FILE* fp;
  char buffer[MAXBUFF], str[6];
  if (!filename || !img) return 0;
  fp = fopen(filename, "r");
  /*
    Gets width and height
  */
  fgets(buffer,MAXBUFF,fp);
  sscanf(buffer,"# ImageMagick pixel enumeration: %d,%d,255,srgb", &img->width, &img->height);
  /*
    Alloc memory for pixels data
  */
  img->pixels= calloc(img->width*img->height, (SEPDATA+CLRDATA)*sizeof(char)+1);
  if(!img->pixels){
    fclose(fp);
     return 0;
  }

  /*
    Charge pixel data;
  */
  int i = 0;
  while (i < img->width * img->height) {
    fgets(buffer, MAXBUFF, fp);
    sscanf(buffer,"%*d,%*d: (%*d,%*d,%*d)  #%s  srgb(%*d,%*d,%*d)", str);
    strcat(str,"\n");
    strcat(img->pixels,str);
    i++;
  }
  fclose(fp);
  return 1;
}

void print_main(FILE* fp, const char* filename){
  if (!fp) return;
  fprintf(fp, "void main() {\n\tclear();\n\t%s_print(1,1);\n\tfflush(stdout);\n\twhile(1){}\n}", filename);
}

int image_printfunc(const char* filename, Image* img, bool full){
  int wd, ht, wcursor, hcursor, i, offset, charWrited;
  char color[6], *data=img->pixels, lastcolor[6]="000000";
  bool boolBlank=true;
  char* funcName;
  FILE* fp;
  if(!img){
    return 0;
  }
  wd = img->width;
  ht = img->height;

  fp=fopen(filename, "w");

  funcName = malloc (strlen(filename)+1);
  strcpy(funcName, filename);
  funcName[strlen(filename)-1]='\0';
  funcName[strlen(filename)-2]='\0';

  if (full)
    print_header(fp, funcName);
  else
    print_header2(fp, funcName);

  for(i=0, wcursor=0, hcursor=0; i<wd*ht; i++, data+=offset, wcursor++, strcpy(lastcolor, color)){
    sscanf(data, "%s\n%n", color, &offset);
    /*
      One row is already printed
    */
    if(wcursor >= wd) {
      wcursor %= wd;
      hcursor++;
      if(boolBlank){
        fseek(fp, -1*charWrited, SEEK_CUR);
        charWrited=fprintf(fp, "printf(\"\\n\");gt((x_0 + %d), (y_0 + 2*%d));\n", hcursor, wcursor);
        charWrited-=13;
      }else{
        charWrited=fprintf(fp, "\"\\n\");\ngt((x_0 + %d), (y_0 + 2*%d));\n", hcursor, wcursor);
        charWrited -= 6;
        boolBlank = true;
      }
    }

    wcursor %= wd;

    if (strcmp(color, "140C1C")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL1 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL1 PX ");
      }
    }else if (strcmp(color, "442434")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL2 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL2 PX ");
      }
    }else if (strcmp(color, "30346D")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL3 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL3 PX ");
      }
    }else if (strcmp(color, "4E4A4E")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL4 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL4 PX ");
      }
    }else if (strcmp(color, "854C30")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL5 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL5 PX ");
      }
    }else if (strcmp(color, "346524")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL6 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL6 PX ");
      }
    }else if (strcmp(color, "D04648")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL7 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL7 PX ");
      }
    }else if (strcmp(color, "757161")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL8 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL8 PX ");
      }
    }else if (strcmp(color, "597DCE")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL9 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL9 PX ");
      }
    }else if (strcmp(color, "D27D2C")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL10 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL10 PX ");
      }
    }else if (strcmp(color, "8595A1")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL11 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL11 PX ");
      }
    }else if (strcmp(color, "6DAA2C")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL12 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL12 PX ");
      }
    }else if (strcmp(color, "D2AA99")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL13 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL13 PX ");
      }
    }else if (strcmp(color, "6DC2CA")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL14 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL14 PX ");
      }
    }else if (strcmp(color, "DAD45E")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL15 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL15 PX ");
      }
    }else if (strcmp(color, "DEEED6")==0){
      if(boolBlank){
        fprintf(fp, "printf(CL16 PX ");
        boolBlank = false;
      }else if(strcmp(color, lastcolor)==0){
        fprintf(fp, "PX ");
      }else{
        fprintf(fp, "CL16 PX ");
      }
    }else{
      if(boolBlank){
        fseek(fp, -1*charWrited, SEEK_CUR);
        charWrited=fprintf(fp, "gt((x_0 + %d), (y_0 + 2*%d));\n", hcursor, wcursor + 1);
      }else{
        charWrited=fprintf(fp, ");\ngt((x_0 + %d), (y_0 + 2*%d));\n", hcursor, wcursor + 1);
        charWrited-=3;
        boolBlank = true;
      }
    }
  }
  if(!boolBlank)
    fprintf(fp, ");\n");
  print_end(fp);
  if(full)
    print_main(fp, funcName);
  free(funcName);
  fclose(fp);
}


int main(int argc, char const *argv[]) {
  Image* img;
  bool full;
  img = image_ini();
  if (argc < 4){
    printf("usage: ./img2func <imageMagickOut.txt> <cfile.c> <full func>\n In full/func just introduce any of those 2\n");
  }
  if (0 == read_pixels(argv[1], img)){
    image_destroy(img);
    return -1;
  }
  if (0 == strcmp("full", argv[3])){
    full = true;
  }else if(0 == strcmp("func", argv[3])){
    full = false;
  }else{
    printf("You should introduce either func or full.\n\tusage: ./img2func <imageMagickOut.txt> <cfile.c> <full/func>");
    image_destroy(img);
  }
  image_printfunc(argv[2], img, full);
  image_destroy(img);
  return 0;
}
