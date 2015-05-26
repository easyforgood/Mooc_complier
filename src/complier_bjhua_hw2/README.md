###Homework2

####一、课程总结

本节课讲的是词法分析。

上节课我们知道了编译器是一个非常模块化的东西，是一个非常流水化的结构。

词法分析器是编译器中的第一个模块，他的输入主要就是以文件形式存在的**字符流**，
输出是可以被程序语言内部定义的词法单元，被称为**记号流**

这就是词法分析器的工作，将写好源代码的字符流转换成程序语言内部的记号流

例如：
一段程序是这样的
``` c
  if(x > = 3)
    todo()
```
切分成记号流
```
  IF LPRAREN IDINT(x) GTEQ INT(3)
    FUNC("todo")
```
类似这样。

(*这里需要注意 回车"\n" "\r\n"和空格" "的存在*)

实现这样的词法分析器主要有两种方式：

- 手工构造： 通过 逐个读取字符进行手工判断

- 自动构造：利用正则表达式进行判断

（作者注：这个地方有所质疑，没体会到有多大区别，可能用过flex才知道吧）

个人觉得都是 状态转移图这种思想和原理

分析一下作业的状态转移图：

规则：

- 读取IF

- 读取标识符

- 读取整型数


![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab2/lab3.jpg)

实际上是一个NFA。

还好比较简单，要是复杂了照着这个图这样写代码就很混乱了。




####二、代码分析：

上一张图已经说明了状态转移，实现代码如下：

**这里可以直接用空格或者换行考虑进行字符截断，所以不需要考虑rollback，因为空格和换行肯定是没有意义的换成"（"就不行了**

```c
//词法分析
void lexer(int ch){
    switch(state){
        case 0:
            //第一次输入，可以选择状态
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
        //上次输入为i，输入为空格说明是ID了直接输出，其他情况还需要进行判断
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
        //数字。空格或回车直接截断输出
        case 2:
            if(ch == ' ' || ch == '\n'){
                printINT();
            }
            break;
        //标识符。同样直接截断
        case 3:
            if(ch == ' ' || ch =='\n'){
                printID();
            }
            break;
        //已经输入了if，如果截断了直接输出IF，省略state:5 ,否则跳入ID
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
```

####三、结果截图

测试文本：

![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab2/lab2.jpg)

![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab2/lab1.jpg)




