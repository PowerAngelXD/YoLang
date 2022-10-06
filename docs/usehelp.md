# Yolang帮助文档 - YoLang Doc
本文档用于帮助用户快速上手yolang及其Console，将分为“Console”和“Yolang语言本体”两部分进行\
Yolang的源码文件后缀为: ".yo"
### Yolang语言本体
#### 1，Yolang内置类型
在Yolang中，内置了五种类型，请参见下面的表格：
|类型名称|中文说明|
|:--:|:--:|
|integer|整型|
|decimal|小数（用C++中的float实现）|
|boolean|布尔值|
|string|字符串|
|null|空类型|
|object|实例类型|

需要注意的是，上表所罗列的，除object，string以外的类型名称在Yolang中均为关键字，同时在1.2.0版本之后，char类型已被移除\
***例：***
```go
var str:string = "hello"; # 声明了一个字符串类型的变量
```
#### 2，表达式，运算符
截至目前的yolang版本，表达式大致分为以下几种：
|名称|意义|
|:--:|:--:|
|AddExpression|基础表达式，四则运算和模运算|
|BoolExpression|布尔表达式|
|ListExpression|列表表达式，类似于“[1,2,3,4,5,5]”这种的都叫做列表表达式|
|StructExpression|类列表结构体表达式，与列表表达式不同的是他的写法，但是使用方式与列表表达式相同|
|NewExpression|new表达式，初始化结构体或者类|
|SpecialExpression|特殊表达式，指那些能够单独成stmt的表达式，例如自增加表达式，自减表达式和赋值表达式|

下面对这些表达式中的部分进行解读：
- AddExpression\
    AddExpression其实就是最为常见的基础表达式，支持四则运算和模运算，但是目前版本中不支持从乘方
    最简单的AddExpression例子：
    ```
    1+1
    2*(3/4) # 正确，括号内的是一个WholeExpression，被包装在PrimExpression内
    ```
- BoolExpression\
    BoolExpression为布尔表达式，支持的运算符有：== != > < <= >= ! || && 几种
    最简单的BoolExpression例子：
    ```
    1==1
    !2!=3 || 3>=4
    ```

#### 3，变量/常量的声明与使用
在Yolang中，变量与常量的声明方法如下：
```go
var a=1;              # 变量声明方式
var b:bool = true;    # 变量声明且规定类型
const c="hello";      # 常量声明方式
const d:char = '\n';  # 常量声明且规定类型
var e=2, u:string="u";# 一次性声明多个变量
dynamic var d=9;
var g;
```
而在Yolang中，不能以关键字为变量/常量名称，同时，标识符开头只能为：“_”，“$”或者字母\
对于变量/常量的生命，你也可以不对其进行初始化，他的默认值将为null，且是dynamic变量
其中，冒号后面接的是类型说明符，均为关键字，用于规定变量类型；如果传入的值不是规定的类型，Yolang解释器会报错：
```
An exception is thrown when the program is running:
{{TypeError}}:
    The expected type does not match the type given by the actual expression
line: X, column: X
```
同时，在新版本1.2.15中，我们引入了dynamic和static关键字，用于规定变量是否可以在赋值的时候更改类型，请看下面的例子：
```go
static var a=1;
var b=5;
dynamic c=5;

a = "hello" # 报错
b = null; # 报错
c = false; # 不报错
```
如上，我们可以知道，如果不加dynamic或static关键字修饰，变量默认为static；如果对static变量进行改变类型的赋值，会触发下面的报错：
```
An exception is thrown when the program is running:
{{TypeError}}:
    The type of assignment to static variable: 'XX' must be its original type
line: X, column: X
```
同时还需要注意，常量不能用dynamic或者static修饰，否则会发生下面的报错：
```
An exception is thrown when the program is running:
{{SyntaxError}}:
    Constants cannot use modifier: 'static'/'dynamic'
line: X, column: X
```
下面介绍变量赋值的方法\
变量赋值的使用方法如下（承接上文）
```go
a = 2;   # pass
b = 5;   # 报错：类型不符
d = 'o'; # 报错，常量不能被重新赋值
```
需要注意的是，目前的版本中Yolang不支持隐式类型转换，所以赋值的时候一定要检查类型，否则会出现下面这样的错误：
```
An exception is thrown when the program is running:
{{TypeError}}:
    The type before and after assignment is inconsistent!
line: X, column: X
```
而对于常量，如果赋值，会出现如下错误：
```
An exception is thrown when the program is running:
{{ConstantError}}:
    A constant cannot be assigned
line: X, column: X

```
#### 4，列表操作
在Yolang中，您可以按照如下方式声明并初始化一个列表：
```go
var list = [1,2,3,4,5];
var listA: string = ["hello", "world"];
```
上面两种代码都是合法的，下面阐述它们的区别：\
第一行代码声明了一个类型为整型的列表，类型由YVM自动推导\
第二行代码声明了一个类型为字符串的列表，类型由用户给出（关于类型说明符，可以看上一节的介绍）\
需要注意的是，Yolang中的列表每一个元素必须类型一致，否则会出现以下错误:
```
An exception is thrown when the program is running:
{{TypeError}}:
    All elements in the list must be of the same type
line: X, column: X
```
下面讲述如何利用列表（承接上文代码）
```go
list[0] = 9;
listA = ["opo", "lll"];
```
第一行代码是对列表对应索引的元素赋值，如果索引超出列表范围，会出现下面的错误：
```
An exception is thrown when the program is running:
{{ListError}}:
    Index out of range of list
line: X, column: X
```
第二行代码，是将整个列表重新赋值，需要注意的是，新的列表长度必须和旧列表长度一致，否则会产生以下报错：
```
An exception is thrown when the program is running:
{{TypeError}}:
    The type before and after assignment is inconsistent!
line: X, column: X
```
而如果将一个不是列表的变量赋值为一个列表或者将一个列表赋值为一个不是列表的量也会产生如上报错
#### 5，条件控制
###### if elif else语句
在Yolang中，最简单的条件控制语句如同下面的方式书写：
```go
if (condition) {
    ..... // write somthing...
}
```
如您所见，这是一个ifstmt，如果您需要更加精细的条件控制，可以使用一下的结构（condition为布尔表达式）
```go
if (condition1) {
    // ...
}
elif (condition2) {
    // ..
}
else {
    // ..
}
```
如果condition1为真，则将会进入if块并执行其内部的所有代码；然后跳出该条件控制；对于elif也是相同的\
但是如果condition1和condition2都不能被满足，则执行else内部的所有代码
###### while语句
在Yolang中，一个标准的while语句如下书写：
```go
while (condition) {
    // ..
}
```
当while运行时，如果condition为真（condition为布尔表达式），则运行while下语句块的内容，直到condition为假，就运行下一个stmt\
如下代码则是一个死循环，他会一直运行下去，因为括号里的布尔表达式为true
```go
while (true) {
    // ..
}
```
###### repeat语句
为了简化部分操作，Yolang发明出了repeat这个东西，它的写法大致如下：
```go
repeat (times) {
    // ..
}
```
其中，times是一个AddExpression，这个表达式的结果将决定repeat体内的代码块被重复运行几次\
而为了某些操作的简便，在repeatstmt中自动创建了名为 "\_ritx_"（没有反斜杠，只有两个下划线）的变量（也被称作外置计数器）；它的值随repeat内置的计数器（这个计数器的起始值为0）的变化而变化\
例如：
```go
repeat (3) {
    out _rit0_;
}
```
上述代码的输出结果为：
```
0
1
2
```
而为了应对多个repeatstmt嵌套导致的变量重命名冲突，在每个外置计数器的后面编上了数字序号，每嵌套一层，后面的数字就会加一 \
请看如下代码：
```go
repeat (3) {
    // _rit0_
    repeat (5) {
        // _rit1_
        repeat (10) {
            // _rit2_
        }
    }
}
```
###### for语句
和C++一样，Yolang的for语句写法如下：
```go
for( definition ; condition ; operation) {
    
}
```
下面来逐步讲解：\
definition：是对for内计数器的声明，统一使用Yolang的变量声明语句；当然你也可以在外部自己声明一个计数器，然后放入到for中使用，如下：
```go
var count = 0;
for (; count < 10; count ++) {
    
}
```
如您所见，definition是可以省略的\
condition：是一个布尔表达式，与while的condition作用一样，您也可以将其省略，但是会让您的程序陷入死循环\
operation：是对您的计数器的操作，通常可以是自增加，自减等操作
###### break语句
在Yolang中，break语句使用这样的写法：
```go
break;
```
break语句只能被用在循环体中，当他作用时，会跳出当前所在的循环体
###### defer语句
在Yolang中，defer语句使用这样的写法：
```go
defer statement;
```
其中，statement可以是任意一个语句，下面请看例子
```go
defer println(1);
println(2);
println(3);
```
上述程序的输出应为：
```
2
3
1
```
如您所见，defer语句就是将当前的stmt重新push到stmt栈尾的，其原理十分简单
###### 函数
在Yolang中，一般这样定义一个函数，下面看一个定义函数的例子
```go
func null fn(a:integer) {
    repeat(a){
        println("hello world");
    }
}
```
其中，fn下的花括号部分，也就是一个BlockStatement，我们将其称之为函数体\
而func则是定义函数的关键字，它后面跟着的null是这个函数返回值的类型，目前只支持基础类型和复合列表类型的返回值，其他类型的返回值将在后续逐一支持\
小括号内的是所定义函数的形式参数列表，以: 'identifier : type'的格式出现\
\
相信您已经掌握了在Yolang中定义一个函数了，下面来看它的使用方法：
```go
func integer add(a:integer, b:integer) {
    return a+b;
}

var a=add(1,2);

println(a);
```
没错，调用Yolang的函数就是这么简单，只需要将函数的标识符写上，后面加上一堆小括号，里面写上所传递的实际参数，这样函数就可以工作了！\
当然，如果传入了不正确类型或者不正确个数的参数，会触发下面的报错：
```
An exception is thrown when the program is running:
{{FunctionCallingError}}:
    Overloaded function with no specified arguments
line: X, column: X
```