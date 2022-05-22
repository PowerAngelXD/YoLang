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