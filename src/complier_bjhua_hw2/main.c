/**
 * homework2
 * 课程总结、代码解释和效果截图参见：
 * https://github.com/easyforgood/Mooc_complier/blob/master/src/complier_bjhua_hw2/README.md
 * 或者项目主页的介绍
 * 
 **/

#include<stdio.h>
#include<stdlib.h>
#define FILENAME "testfile"
//需要的全局变量
FILE *fp; //文件指针
int line=1; //对应行号
int linePos=1; //对应行列号
int startPos=0; //上一个词的起点位置
int nowPos=0; //文件指针现在的位置 = ftell(fp)
/*
用作状态转移的参数
0 1 2 3 4这几种状态
0 代表无任何输入的状态，非最终状态
1 第一次输入i的状态，非最终状态
2 第一次输入为数字的状态，最终状态，代表输入的是NUM，第一次输入为数字，说明只能为数字或者异常输入
3 第二次输入为f,临界状态，可能为非最终，也可能为最终，代表输入为IF。这里偷懒了。
4 输入为ID，最终状态。
*/ 
int state=0;
//print IF
void printIF(){
    printf("IF(%d,%d)\n",line,linePos);
    nowPos=ftell(fp);
    startPos=nowPos;
    state=0;
}
//print ID
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
//print NUM
void printINT(){
     int tempPos=ftell(fp);
    fseek(fp,startPos-tempPos,SEEK_CUR);
    char *s=(char *)malloc(sizeof(char)*(nowPos-startPos));
    fread(s,sizeof(char),nowPos-startPos,fp);     
    printf("NUM(%s)(%d,%d)\n",s,line,linePos);
    nowPos=ftell(fp);
    startPos=nowPos;
    state=0;
}

//词法分析
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
    //每次操作都对齐nowPos
    nowPos=ftell(fp);

}
void main(int argc,char *argv[]) //命令行参数
{
    //默认文件名为testfile
    //文件操作参考了网上的，见：testFile.c文件
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
    //列号，行行号记录
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
