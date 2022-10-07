### [返回到目录](content.md)
## 第三章：动态类型和静态类型
在这个章节中，您将会知悉Yolang中的动态类型（dynamic）和静态类型（static）\
\
在Yolang中，对于所有以声明的不加修饰符的变量，它的类型只能是初始化时的类型，如果赋给它其它类型的值，yovm在运行时会给予报错；但是在有的情况下，我们需要这样一个可以随时改变类型的变量，因此————dynamic和static便应运而生\
常见的声明一个dynamic或者static变量的方式如下，他们两个在Yolang中都是修饰符，因此以后遇到了新的修饰符按照下面这个写法就可以了
```go
dynamic var a: string = "hello";
static var b: integer = 0;
var c: boolean = false;
```
上述代码的第一行便是一个声明dynamic的语句，在这行语句运行后，以下代码都会是合法的：
```go
a=3;
a=null;
a="hello world";
```
可见，当变量声明为了dynamic类型变量，关于这个变量的赋值运算中的右值可以与它原本的类型不相同\
\
第二行代码与第三行代码是等价的，第二行代码唯一的不同就是显式指明了变量b是一个静态变量，因为其被声明为了静态变量，所以一下代码是非法的：
```go
b=null;
b="hello world";
```
可见，当变量被声明为static类型后，关于这个变量的赋值运算中的右值必须与它原本的类型相符\
\
再来说说第三行代码，这行代码可能看不出声明的是个dynamic还是static变量，但是在Yolang中，所有不加修饰符dynamic或static的变量都将被认为是static变量，也就是说，以下代码也是非法的：
```go
c=1;
c="world!";
```
因此在第二章中，`var n;`这种初始化方法就是无意义的，因为它默认值是null且为static，这意味着它的类型永远只能是null，不能是其他的类型\
\
此外，使用dynamic和static还有一项禁忌的点————不能在声明常量时指明为dynamic还是static，例如如下代码就全部是非法的：
```go
dynamic const a: integer =1;
static const b: string ="hello";
```
为什么？因为常量是不变的量，在它初始化过后它的值就只能是初始化时的值，如果对其进行赋值运算就会报错；\
而dynamic和static本身的作用是指定当 __变量__ 进行赋值运算后的赋值的方式规定；很显然，常量不能赋值，因此这样进行的修饰是无意义的，同时也会被禁止