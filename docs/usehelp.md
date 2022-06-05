# Yolang帮助文档
本文档用于帮助用户快速上手yolang及其Console，将分为“Console”和“Yolang语言本体”两部分进行
### Yolang语言本体
#### 1，Yolang内置类型
在Yolang中，内置了五种类型，请参见下面的表格：
|类型名称|中文说明|
|:--:|:--:|
|integer|整型|
|decimal|小数（用C++中的float实现）|
|boolean|布尔值|
|char|字符|
|string|字符串|

需要注意的是，上表所罗列的类型名称在Yolang中均为关键字\
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
```
而在Yolang中，不能以关键字为变量/常量名称，同时，标识符开头只能为：“_”，“$”或者字母\
其中，冒号后面接的是类型说明符，均为关键字，用于规定变量类型；如果传入的值不是规定的类型，Yolang解释器会报错：
```
An exception is thrown when the program is running:
{{TypeError}}:
    The expected type does not match the type given by the actual expression
line: X, column: X
```
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
