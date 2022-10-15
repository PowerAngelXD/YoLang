### [返回到目录](content.md)
## 第六章：条件控制
在本章中，您将了解到Yolang中的条件控制语句

#### if语句
首先，您将了解最为基础的if语句，它的大致格式是这样的：
```go
if (condition) {
    ...
}
```
一个if语句分为了两个部分——条件部分和if执行体（即花括号部分）\
整个if语句的执行流程与其他编程语言的流程无异——其本质都是根据condition的真假（true或false），再判断是否执行花括号内的内容。比如如果condition为真，则执行花括号内部分\
下面来看一个用到它的例子：
```go
var a = input();
if(a == "hello") {
    println("hello world!");
}
```
最终，如果用户的输入是“hello world”，则会出现如下输出内容：
```
hello world!
```
#### elif语句
