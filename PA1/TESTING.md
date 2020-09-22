# PA1 Testing (Emilia Bourgeois)
## Directory 
```C 
char *directory = strcat(strtok_r(str, "/", &token), "/");
char *ext;

DIR *dr = opendir(directory);

if(dr == NULL) {
    fprintf(stdout, "Could not open directory %s\n", directory);
    return 0;
}
```
This block ensures validity of the directory. In all honesty I did not have to test this at all because I somehow got it right on the first go.
## Extension
```C
char *ext;
ext = strrchr(argv[1], '.');
int len = sizeof(exts)/sizeof(exts[0]);
int w = 0;
for(int j = 0; j < len; ++j){
    if(!strcmp(exts[j], ext)) {
        w=1;
    }
}
if(w == 0){
    fprintf(stdout, "%s is an unsupported file type\n", ext);
    return 0;
```
This code computes the size of the array to loop through it to see if the extension found from splitting the string is in the array of supported extensions
## Degree input
```C
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
```
My inexperience with C made this block quite troubling, but after testing the degree inputs with chars, a mix of numbers and chars, and just the enter key I finally got this part to work.
## Supported files
```C
char *name = de->d_name;
if(strstr(name, ext) != NULL){
//stuff
}
else{
    //if file is unsupported throws error
    fprintf(stdout, "Incorrect input for: %s, file should be a %s file\n", name, ext);
    continue; 
}
//count to track # of files
count++;
if(count == 0){
        fprintf(stdout, "No images found\n");
        return -1;
    }
```
This block ensures that the user knows if they had an incorrect file in their photos folder. I tested it with fake and real extensions and it still works.
