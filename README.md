# Mooc_complier




保健哥Mooc上的课程 具体戳：[Mooc_编译原理_华保健](http://mooc.study.163.com/learn/USTC-1000002001#/learn/announce)

这里是我写的Homework的源代码欢迎大家交流指正

邮箱：siplexy.pi@outlook.com

###Homework1

####一、课程总结
这一节主要是介绍一下编译器的构成。
在我看来编译器是非常模块化的东西。

最简单的编译器由  语法分析 -> 词法分析 -> 语义分析 -> 代码生成

但编译器的工作远不止此，为了提高代码运行的效率和质量，后面好需要做一系列的优化，后期处理。

正规化、控制流分析、SSA等等等


这一节主要的工作是 简单了解一下一个编译器的工作

我们把一个简单程序转换成了一个简单的伪汇编

语法：

- 整形常量  int 

- 加运算   +

可用汇编代码：

- push a

- add

一个简单的实例是：

  **计算：  2+3+4**

对应的汇编代码就是

    push 2
    push 3
    add
    push 4
    add

为了方便转换这个代码，我们用于语法树的形式表示：

                +
               / \
              +   4
             / \
            2   3

我们可以发现，通过后序遍历 （左右中）

就可以构造出整个 汇编代码了。





####二、作业解释：

**常量折叠 Constant_folding**：
是其中一种被很多现代编译器使用的编译器最优化技术，常量折叠是在编译时间简单化常量表达的一个过程。

对于上述例子2+3+4，就是生成汇编代码的时候，只需要一条 

    push 9
    

就可以了

- List_reverse_print 实现

``` c
//TODO
void List_reverse_print (struct List_t *list)
{
  //TODO();
  if(list==NULL){
      return;
  }
  else{
      List_reverse_print(list->next);
  }
  switch(list->instr->kind){
      case STACK_ADD:
         printf("add\n");break;
      case STACK_PUSH:
         printf("push %d\n",((struct Stack_Push *)(list->instr))->i);
         break;
  }
  
}
```

这是逆序输出。所以是**先遍历，再输出**，利用递归的特性

- compile() No Constant Folding

``` c
void compile (struct Exp_t *exp)
{
  switch (exp->kind){
  case EXP_INT:{
    struct Exp_Int *p = (struct Exp_Int *)exp;
    emit (Stack_Push_new (p->i));
    break;
  }
  case EXP_SUM:{
    //TODO()
    struct Exp_Sum *p=(struct Exp_Sum *)exp;
    compile(p->left);
    compile(p->right);
    emit(Stack_Add_new());  //if you dont want to use Constant folding
    break;
  }
  default:
    break;
  }
}
```

- compile() Constant Folding

``` c
int pop(){
    struct Stack_Push * p=(struct Stack_Push *)(all->instr);
    all=all->next;
    return p->i;
}

void compile (struct Exp_t *exp)
{
  switch (exp->kind){
  case EXP_INT:{
    struct Exp_Int *p = (struct Exp_Int *)exp;
    emit (Stack_Push_new (p->i));
    break;
  }
  case EXP_SUM:{
    //TODO()
    struct Exp_Sum *p=(struct Exp_Sum *)exp;
    compile(p->left);
    compile(p->right);
    int a=pop();
    int b=pop();
    emit(Stack_Push_new(a+b));
    break;
  }
  default:
    break;
  }
}
```
这里附加了一个int pop()函数，用于弹出栈顶的元素。
相加计算结果之后再push



具体代码见：[src/complier_bjhua_hw1.c](https://github.com/easyforgood/Mooc_complier/blob/master/src/complier_bjhua_hw1.c) 

####三、秀截图

![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab1/1.jpg)

![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab1/3.jpg)

逆序输出意思是按照执行顺序输出！

-----


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


![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab2/3.jpg)

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






