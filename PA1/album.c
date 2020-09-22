#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include "readline.h"
#include <ctype.h>


int main(int argc, char * argv[])
{   

    if(argv[1] == NULL) {
        fprintf(stdout, "Enter a paramter like photos/*.jpg\n");
        return -1;
    }
    
    
    //initializes variables
    char * exts[]= {".jpg", ".gif", ".png", ".bmp", ".svg", ".webp", ".ico", ".raw"};
    char str[128];
    char *token;
    strcpy(str, argv[1]);
    struct dirent *de;
    //split arg for directory
    char *directory = strcat(strtok_r(str, "/", &token), "/");
    char *ext;
    //find extension
    ext = strrchr(argv[1], '.');
    //compare extension to supported ones
    int len = sizeof(exts)/sizeof(exts[0]);
    int w = 0;
    for(int j = 0; j < len; ++j){
        if(!strcmp(exts[j], ext)) {
            w=1;
        }
    }
    if(w == 0){
        fprintf(stdout, "%s is an unsupported file type\n", ext);
        return -1;
    }

    int count = 0;
    //opens directory
    DIR *dr = opendir(directory);

    if(dr == NULL) {
        fprintf(stdout, "Could not open directory %s\n", directory);
        return -1;
    }

    //Beginning of webpage
    FILE *ptrFile = fopen("index.html", "w");
    fprintf(ptrFile, "<!DOCTYPE html>\n");
    fprintf(ptrFile, "<html>\n");
    fprintf(ptrFile, "<head>\n");
    fprintf(ptrFile, "<title>Emilia Bourgeois PA1 Photo Album</title>\n");
    fprintf(ptrFile, "<link rel=\"stylesheet\" href=\"main.css\">\n");
    fprintf(ptrFile, "</head>\n");
    fprintf(ptrFile, "<body>\n");
    fprintf(ptrFile, "<h1>Emilia Bourgeois PA1 Photo Album</h1>\n");
    fprintf(ptrFile, "<ul>\n");

    //while loop to loop through files
    while((de = readdir(dr)) != NULL){
        //stores name
        char *name = de->d_name;
        if(strstr(name, ext) != NULL){
            //performs transformations as long as file is of the specified extension
            //initializes file names
            char in[100];
            strcpy(in, directory);
            strcat(in, de->d_name);
            char thumb[100];
            strcpy(thumb, directory);
            strcat(thumb, "thumb");
            strcat(thumb, de->d_name);
            char final[100];
            strcpy(final, directory);
            strcat(final, "final");
            strcat(final, de->d_name);
           
            
            //forks for thumbnail
            fprintf(stdout, "-> Converting: '%s'\n", in);
            pid_t rc = fork();
            if (0 == rc) {
                execlp("convert", "convert", "-geometry", "25%", in, thumb, NULL);
                fprintf(stdout, "**error: cannot convert '%s'.**\n", in);
                exit(-1);
            }
            //forks for displaying image
            pid_t view = fork();
            if(0 == view) {
                sleep(2); //basic sleep so the resize works
                execlp("display", "display", thumb, NULL);
                fprintf(stdout, "**error: cannot display '%s'.**\n", in);
                exit(-1);
            }
            //reads in user input for angle of rotation
            const int lineLength = 50;
            char line[lineLength]; 
            char * d;
            int x = 1;
            int n = 0;
            while(x != 0){
                n = 0;
                
                fprintf(stdout, "Enter rotation (0 for no rotation): ");
                if (readLine(line, lineLength)) {
                    if(strlen(line) <= 1){ //case for if only enter is pressed for input
                        n = 1;
                        fprintf (stdout, "Please enter a number\n");
                    }
                    for (int i=0;i<strlen(line) - 1; i++) {
                        if (!isdigit(line[i])) {
                            if (n == 0){
                                fprintf (stdout, "Please enter a number\n");
                            }
                            n = 1;
                        }
                    }
                }
                if(n == 0){
                    x = 0;
                    d = line;
                }
            }
            //rotate fork
            pid_t rotate = fork();
            if(0 == rotate){
                execlp("convert", "convert", in, "-geometry", "75%", "-rotate", d, final , NULL);
                fprintf(stdout, "**error: cannot rotate '%s'.**\n", in);
                exit(-1);
            }
            //caption input
            char * caption;
            fprintf(stdout, "Enter caption: ");
            if (readLine(line, lineLength)) {
                caption = line;
            }

            //adds images as a list 
            fprintf(ptrFile, "<li>\n");
            fprintf(ptrFile, "<h3>%s</h3>\n", caption);
            fprintf(ptrFile, "<a href=\"%s\"><img src=\"%s\"></a>\n", final, thumb);
            fprintf(ptrFile, "</li>\n");
            //kills display
            kill(view, SIGTERM);
        }
        else{
            //if file is unsupported throws error
            fprintf(stdout, "Incorrect input for: %s, file should be a %s file\n", name, ext);
            continue; 
        }
        //count to track # of files
        count++;
    }
    //yells at you if there were no valid input files
    if(count == 0){
        fprintf(stdout, "No images found\n");
        return -1;
    }
    //ends and closes html doc
    fprintf(ptrFile, "</ul>\n");
    fprintf(ptrFile, "</BODY>\n"); 
    fprintf(ptrFile, "</HTML>"); 
    fclose(ptrFile); 

    return 0;
}
