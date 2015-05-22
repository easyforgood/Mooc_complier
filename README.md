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

``` 
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



具体代码见：src/complier_bjhua_hw1.c 

####三、秀截图

![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab1/1.jpg)

![pic](https://raw.githubusercontent.com/easyforgood/Mooc_complier/master/pic/lab1/3.jpg)

逆序输出意思是按照执行顺序输出！

