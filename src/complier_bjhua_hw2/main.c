#include<stdio.h>
#include<stdlib.h>
#define FILENAME "testfile"

FILE *fp;
int line=1;
int linePos=1;
int startPos=0;
int nowPos=0;
//0 1 2 3 4 
int state=0;
void printIF(){
    printf("IF(%d,%d)\n",line,linePos);
    nowPos=ftell(fp);
    startPos=nowPos;
    state=0;
}
void printID(){
    int tempPos=ftell(fp);
    fseek(fp,startPos-tempPos,SEEK_CUR);
    char *s=(char *)malloc(sizeof(char)*(nowPos-startPos));
    fread(s,sizeof(char),nowPos-startPos,fp);
    printf("ID(%s)(%d,%d)\n",s,line,linePos);
    nowPos=ftell(fp);
    startPos=nowPos;
    state=0;
}
void printINT(){
     int tempPos=ftell(fp);
    fseek(fp,startPos-tempPos,SEEK_CUR);
    char *s=(char *)malloc(sizeof(char)*(nowPos-startPos));
    fread(s,sizeof(char),nowPos-startPos,fp);     
    printf("INT(%s)(%d,%d)\n",s,line,linePos);
    nowPos=ftell(fp);
    startPos=nowPos;
    state=0;
}


void lexer(int ch){
    switch(state){
        case 0:
            if (ch == 'i'){
                state=1;
            }
            else if (ch>='0' && ch <= '9'){
                state=2;
            }
            else if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_'){
                state=3;
            }
            else{
                startPos++;
            }
            break;
        case 1:
            if (ch == 'f'){
                state=4;
            }
            else if (ch == ' ' || ch == '\n'){
                printID();
            }
            else{
                state=3;
            }
            break;
        case 2:
            if(ch == ' ' || ch == '\n'){
                printINT();
            }
            break;
        case 3:
            if(ch == ' ' || ch =='\n'){
                printID();
            }
            break;
        case 4:
            if (ch == ' ' || ch == '\n'){
                printIF();
            }
            else{
                state=3;
            }
            break;
        default:break;

    }
    nowPos=ftell(fp);

}
void main(int argc,char *argv[]) //命令行参数
{
    int ch;
    char *filename=FILENAME;
    if(argc==2)
    {
       filename=argv[1]; 
    }
    
    if((fp=fopen(filename,"r"))==NULL)
    {
        printf("The file <%s> can not be opened.\n",argv[1]);
        return;
    }
    int state;
    nowPos=startPos=ftell(fp);
    ch=fgetc(fp); 
    while(ch!=EOF) 
    {
        linePos++;
        if (ch == '\n'){
            line++;
            linePos=1;
        }
        lexer(ch);
        ch=fgetc(fp);
    } 
    lexer(' ');
    fclose(fp); 
}
