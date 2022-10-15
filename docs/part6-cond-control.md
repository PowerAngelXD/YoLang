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
elif语句语法类似于if语句，具体格式如下：
```go
elif (condition) {
    ...
}
```
需要注意的是，elif语句的前提必须是已经存在一个与其邻近的if语句，否则类似于如下的情况会产生一个报错：
```go
if (true) {
    println("hello");
}
var a=1;
elif (true) {
    println("hello");
}
```
或者是
```go
elif (true) {
    println("hello");
}
```
第二种情况是毫无上下文的时候直接使用elif语句导致的错误\
elif语句的流程与if一样，当上一个elif语句或者if语句不成立且当前elif的condition成立时，便会进入花括号内的内容开始执行
#### else语句
Yolang中，else语句的写法如下：
```go
else {
    ...
}
```
