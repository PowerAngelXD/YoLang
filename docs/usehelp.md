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
#### 2，变量/常量的声明与使用
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