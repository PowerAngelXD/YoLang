# YoLang
![yopic.png](yopic.png)
Yolang是一个轻量级的，简单易上手的解释性编程语言，目前由PowerAngelXD（在gitee上名为CatBall）开发，后续可能还会有更多的开发者加入到Yolang项目的开发中
### 关于仓库
本项目在gitee和github均有仓库：[Github仓库](https://github.com/PowerAngelXD/YoLang) | [Gitee仓库](https://gitee.com/catball0x00/YoLang)，且两个仓库同步更新
### 构建
本项目由CMake构建，请您开发前确保您已经在您的设备上准备好开发环境，关于构建这里就不深入介绍了
### 许可证
本项目遵循MIT开源[许可证](LICENSE)
### 使用
关于如何使用Yolang及其Console，请参阅：[帮助文档](docs/usehelp.md)
### 下载
如果您要获取Yolang解释器的应用程序包，请在本仓库页面的右侧找到***Release***（在Gitee上，这个地方叫作“发行版”）并根据需要获取其中的版本\
而在Yolang，分为开发版本和发行版本两种版本号\
Yolang的开发完整版本号遵循以下规则：\
\
***Modifier***-**Year**-**Month**-***Number***
- Modifier：用于修饰当前版本的字段，例如：“LTS”，“Instable”，“Release”等
- Year：版本发布的年份
- Month：版本发布的月份
- Number：版本序号，为一个四位数字，起始值为：“0001”
例如：LTS-2022-5-0001 就是一个正确的Yolang版本号，它代表着一个LTS版本，在2022年5月份发布，序号为0001\
下面是对于修饰字段（Modifier）的说明：

|修饰字段名称|意义|
|-------|-------|
|LTS|长期支持版，会得到较长期的维护和支持|
|Instable|不稳定版，也叫做测试版，通常这种版本不适用于日常开发，只是用于测试某个新功能|
|Release|发行版，稳定的版本|
|Build|快照版本，拥有比Release和LTS版本更多的内容；相应的，比Instable版本更不稳定|

而在日常称呼中，您可以选择更简短的版本号称呼方式：\
***年份后两位***.***月数***.***Number字段中去掉开头所有0的部分***\
这样解释起来可能有点麻烦，下面来看一个实际的例子：\
完整版本号：LTS-2020-7-02030\
缩略版本号：20.7.2030\
Yolang的发行版本号遵循以下规则：\
\
***Yolang*** 数字·数字·数字\
例如：Yolang 1.0.0  就是一个符合规定的发行版本号\

### 未来会实现
- [ ] null值
- [ ] 变量默认初始化
- [ ] 完善的vm管理
- [ ] 变量初始化列表
- [ ] 多维数组
- [ ] 三元运算符
- [ ] Objects和对应操作