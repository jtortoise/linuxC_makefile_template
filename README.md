# Linux环境C语言编程Makefile多级目录简单模板

C语言工程项目的Makefile模板，支持一级目录，二级目录，多级目录的Makefile模板。

## 测试平台：

* Linux操作系统环境，elemantary OS(基于ubuntu16.04)

  ![eleOS](image/eleOS.png) 

* make版本，4.1

  ![make](image/make.png) 

## 目录结构：

``` cassandra
linuxC_makefile_template
|—— image ******************* 放笔记用的图片
|—— material **************** 放笔记用的一些资料文档
|—— multilevel_dirs ********* 多级目录的Makefile模板示例
|—— onelevel_dirs *********** 一级目录的Makefile模板示例
|—— secondlevel_dirs ******** 二级目录的Makefile模板示例
```

## Makefile模板功能：

* 支持自动推导头文件依赖

  修改项目工程中的头文件，能够导致包含该文件的对象能够重新编译。在之前一些简单的Makefile模板中，当修改头文件时，重新执行`make`，项目不会进行对应模块的编译。

* 支持多级目录结构划分

  多级目录的Makefile模板，使得在顶层执行make的时候，能够对多个目录下的所有源文件进行重新编译。这样子，我们就能够把代码按照模块功能的划分组织我们的代码，而编译的时候，仅仅通过一条`make`命令就可以帮助我们完成复杂的编译和链接，多级目录比较适合一些稍微复杂的项目工程。

* 支持一级目录和二级目录结构划分

  Makefile模板支持多级目录的编译，也就意味着能够支持简单目录结构的编译，在这里给出相应的示例。

## 使用：

简单介绍`make`实现多级目录编译的`Makefile`编写思想以及如何在多级目录下添加新的子模块下的`Makefile`的添加。

### 顶层目录添加的Makefile

``` makefile
TOPDIR:=$(shell pwd)/
CC:=gcc
export TOPDIR CC

exclude_dirs:=inc lib

include $(TOPDIR)Makefile.env
```

* 传递顶层目录的绝对路径(`TOPDIR`)

  顶层目录的Makefile指定顶层目录的绝对路径，并且将这个变量传递到下一层的Makefile中，这样子所有的Makefile就知道顶层目录在哪里。

* 传递CC变量(`CC`)

  统一整个项目使用变量CC指定的编译器进行编译。

* `exclude_dirs`

  排除指定的目录不用进入到里面执行`make`编译。 

注意：传递`TOPDIR`目录时，理论上目录`/xx/xx`和`/xx/xx/`指的都是相同的目录，但在这里统一使用第二种表示，这样子是为了方便执行`make clean`能够清除复制的文件的时候不会报错。

###src目录添加的Makefile

``` makefile
exclude_dirs:=

include $(TOPDIR)Makefile.env
```

直接扫描当前目录下的各个模块，进入到里面执行`make`编译，如果不想要某一个模块也参与编译，可以通过变量`exclude_dirs`指定。

### 一般模块目录添加的Makefile

对于一般模块，通过将其所有的源文件链接成一个动态库，然后放到顶层目录下的`lib`目录下面，让其它模块调用。

``` makefile
LIB:=libmylib.so
LIBPATH:=$(TOPDIR)lib/

CPPFLAGS:=-I $(TOPDIR)inc
CFLAGS:=
LDFLAGS:=
LDFLAGS+= -lm

include $(TOPDIR)Makefile.env
```

* 指定生成的动态库名称(`LIB`)
* 指定存放动态库的路径(`LIBPATH`)
* 指定编译器参数，即生成的库文件所需要的信息
  * CPPFLAGS：指定头文件的位置
  * CFLAGS：编译器编译参数
  * LDFLAGS：指定要链接的库位置和库名称等

注意：`LIBPATH`指定的目录变量也是以`/xx/xx/`的格式。

### 主模块目录添加的Makefile

主模块里面包含主函数，在这里统一使用main文件夹来存放，若改成别的文件夹名，编译顺序会乱掉，从而报错。

``` makefile
vpath %.so $(TOPDIR)lib

TARGET:=make_test
EXEPATH:=$(TOPDIR)

TARGET_LIBS:=libmylib.so

CPPFLAGS:= -I $(TOPDIR)inc
CFLAGS:= -Wl,-rpath=$(TOPDIR)lib
LDFLAGS:= -L $(TOPDIR)lib 
LDFLAGS+= -lmylib

include $(TOPDIR)Makefile.env
```

* vpath关键字指定依赖的库文件.so到指定目录查找

  主程序的编译依赖生成的库文件，库文件改变，主程序应当也需要重新编译。

* 指定生成的程序名称(`TARGET`)

* 指定程序存放的路径(`EXEPATH`)

  可以放到顶层目录去，一执行`make`完毕，可以直接在当前目录下执行文件，方便。

* 指定主程序依赖的库文件列表(`TARGET_LIBS`)

  这是主程序依赖的库文件列表，如果不写，但又依赖某个生成的库文件，当这个库文件更新时候，主程序不会更新，这是不对的。所以，需要写好主函数依赖的库文件列表，使得主程序能够及时更新。

* 指定编译器的参数

  - CPPFLAGS：指定头文件的位置

  - CFLAGS：

    编译器编译参数，指定程序运行时到指定的位置寻找动态库文件，不指定的话，程序执行的时候可能会出现找到库文件的，当然你也可以把库文件丢到类似`/lib`,`/usr/lib`等目录下，执行的时候是找的到的，项目在测试过程中是不建议这么做的。

  - LDFLAGS：指定要链接的库位置和库名称等

以上的介绍是多级目录Makefile模板的使用方法，对于一级目录和二级目录的使用也是类似，直接生成可执行文件，不需要生成库文件，这些在自己写一些小项目的时候做为Makefile来管理工程是非常方便的，具体的写法直接看下示例就行了。

##实现思想：

在这里，先阅读一篇文章：[一个适用于层级目录结构的makefile模版](http://www.cnblogs.com/coderkian/p/3479564.html) ，这个项目的实现也基本都是参考这篇文章来写的，核心思想不变，在细节上做了一些改善。

### Makefile.env

能够实现多级目录编译的Makefile，完全依赖一个公共Makefile模板实现，在这个模板里面，能够实现三个基本的功能：

* 动态库的生成及复制到指定路径(可选)；
* 可执行目标文件生成及复制到指定路径(可选)；
* 递归执行每个目录下的Makefile(一定执行)；

一个模块，是要生成库文件(普通模块)还是可执行文件(主模块)，只需要给对应的变量(`LIB`,`TARGET`)赋值，比如在`main`模块里面(包含`main.c`文件)要生成可执行文件，也就是程序，直接在该模块下面的Makefile文件这么写上：`TARGET:=main`，就代表这个模块要生成一个叫main的可执行文件，相应的：`LIB:=libmylib.so`就是要生成一个动态库，目前也只支持动态库生成。

在这里，对公共的Makefile文件`Makefile.env`每个模块作个简单介绍：

####目录搜寻

``` makefile
dirs:=$(shell find . -maxdepth 1 -type d)
dirs:=$(basename $(patsubst ./%,%,$(dirs)))
maindir:=$(findstring main,$(dirs))
dirs:=$(filter-out $(exclude_dirs) main,$(dirs))
SUBDIRS:=$(dirs) $(maindir)
```

这一部分的代码，用来查找需要编译的目录，并将主模块main目录放到最后才执行，因为主模块的编译依赖其它模块，必须在其它模块之后执行编译，做了个简单目录调整，确保main模块在最后才执行。在这里，如果不想对一些目录编译的话，通过指定变量`exclude_dirs`来排除不需要编译的目录，如：

``` makefile
exclude_dirs:=module1 module2 ...
```

#### 源文件搜寻

``` makefile
SRC:=$(wildcard *.c)
OBJS:=$(SRC:%.c=%.o)
DEPENDS:=$(SRC:%.c=%.d)
```

简单查找下当前目录下所有的.c文件，放到SRC变量中，简单的转换成.o(可重定位目标文件)和.d(头文件依赖关系文件)。

####all伪目标

``` makefile
all:$(TARGET) $(LIB) subdirs
```

在这里，是这个Makefile需要执行的规则，`$(TARGET)`和`$(LIB)`是可选的，不需要执行这两个目标时候执行留空，这样子目标`all`就会变成:

``` makefile
all: subdirs
```

也就是只执行subdirs规则，这个规则在后面介绍。

#### subdirs规则

``` makefile
subdirs:$(SUBDIRS)
	@for dir in $(SUBDIRS); \
	do $(MAKE) -C $$dir all || exit 1; \
	done
```

就是进入到该目录下的每个子目录执行里面的`Makefile`文件，实现递归编译，这样子，当我们添加新的模块时候，就能够自动进行编译了。

####$(TARHET)规则

``` makefile
$(TARGET):$(OBJS) $(TARGET_LIBS)
	$(CC) $(CPPFLAGS) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	$(if $(EXEPATH),@cp $@ $(EXEPATH))
```

这个规则目前是用来生成可执行文件的，可执行文件依赖当前模块下的一堆`.o`文件(`$(OBJS)`)以及库文件`.so`(`$(TARGET_LIBS)`)。然后就执行gcc进行编译，根据一堆编译器参数，生成可执行文件，最后判断下存放程序的路径(`$(EXEPATH)`)是不是空，不是空就复制下，要是空的就不用复制。 

* `TARGET_LIBS`：指定程序依赖的库文件列表
* `EXEPATH`：指定程序存放的路径
* `TARGET`：程序名

#### $(LIB)规则

``` makefile
$(LIB):CFLAGS+= -fPIC
$(LIB):$(OBJS)
	$(CC) -shared $(CPPFLAGS) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	$(if $(LIBPATH),@cp $@ $(LIBPATH))
```

在这里，默认生成动态库，所以给这个模式的变量`CFLAGS`强制加上`-fPIC`选项，然后编译器根据一堆编译参数，由外部指定，生成一个库文件，再拷贝到对应的位置(`$(LIBPATH)`)。

* `LIB`：库文件名
* `LIBPATH`：存放库文件路径

#### $(OBJS)规则

``` makefile
$(OBJS):CFLAGS+= -c
$(OBJS):%.o:%.c
	$(CC) $(CPPFLAGS) $< -o $@ $(CFLAGS)
```

把每个源文件`.c`编译成可重定位的目标文件。

#### $(DEPENDS)规则

``` makefile
-include $(DEPENDS)
$(DEPENDS):%.d:%.c
	@set -e; $(RM) $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@; \
    $(RM) $@.$$$$
```

这个规则就是用来推导源文件`.c`的头文件依赖关系，然后存到`.d`文件去。

#### clean伪目标

``` makefile
clean:
	@for dir in $(SUBDIRS); \
	do $(MAKE) -C $$dir clean || exit 1; \
	done
	$(RM) $(TARGET) $(LIB) $(OBJS) $(DEPENDS)
	$(if $(notdir $(LIBPATH)$(LIB)),$(RM) $(LIBPATH)$(LIB))
	$(if $(notdir $(EXEPATH)$(TARGET)),$(RM) $(EXEPATH)$(TARGET))
```

把生成的`.o .d .so`等编译产生的文件都清除掉，递归扫描每个目录清除。

### 调用Makefile.env

从`Makefile.env`文件的实现中可以看出来，想要执行相应模块功能，直接给相应模块的变量进行赋值控制就行了，就像在使用章节中所展示的那样，定义变量，然后`include`下`Makefile.env`文件。

## 参考链接

1. coderkian. 一个适用于层级目录结构的makefile模版[DB/OL]. http://www.cnblogs.com/coderkian/p/3479564.html

## 附录A：知识点

- 从代码到生成可执行文件的四个阶段[^1]

- Makefile中gcc编译程序各阶段的参数变量[^2][^3]

- Makefile中的通配符[^4]

  - `%`

    适用用于Makefile中的函数中的模式匹配以及静态模式和多模式规则中模式匹配。

  - `*, ?, [xxx]`

    Shell中的通配符，适用于变量的定义以及规则中shell命令。

- Makefile中的vpath关键字[^5]

  让Makefile到指定的目录查找匹配模式的文件。

  语法：`vpath <pattern> <path> `，如：`vpath %.c ../src`

- Makefile中的字符串处理函数[^6]

- gcc动态库以及静态库生成[^7][^8][^9]

  - gcc的三种`library`简单介绍[^7]

    1. 静态库(`Static Library`)

       程序编译的时候链接上去。

    2. 动态库(`Shared Library`)

       在程序加载到内存的时候，也就是程序启动的时候，才加载库函数。

    3.  dll(`Dynamically loaded libraries`)

       程序在运行的过程中才加载库函数，也就是需要的时候才加载。

  - 利用ldd查看程序链接库的地址情况[^8]

    执行命令：`ldd 程序名`； 

    简单示例：

    ![ldd](image/ldd.png) 


  - 注意事项：
    1. 生成单个可重定位的目标文件，`gcc`需加上`-fPIC`选项；
    2. 在制作自己的动态库中引用其他的库函数，在生成动态库时，需链接上相应的库；
- Makefile中实现自动生成头文件的依赖[^10][^11]
- shell相对路径转绝对路径[^12]

## 附录A：参考链接

1. Randal E.Bryant，David O'Hallaron. 深入理解计算机系统(第三版)[M].  龚奕利，贺莲. 机械工业出版社，2016：3-3.
2. 陈皓. 跟我一起写Makefile[DB/OL]. 2005：65-66. 
3. 宋劲杉. Linux C编程一站式学习[DB/OL]. http://docs.linuxtone.org/ebooks/C&CPP/c/index.html . B. 编译开发工具小结：gcc常用选项. 
4. 陈皓. 跟我一起写Makefile[DB/OL]. 2005：18-19.
5. 陈皓. 跟我一起写Makefile[DB/OL]. 2005：19-20.
6. 陈皓. 跟我一起写Makefile[DB/OL]. 2005：44-51.
7. kaineshu. \[轉貼\]用gcc 自製 Library[DB/OL]. https://kaineshu.wordpress.com/2007/05/02/%E8%BD%89%E8%B2%BC%E7%94%A8gcc-%E8%87%AA%E8%A3%BD-library/
8. vfhky. Linux gcc编译生成静态库和共享动态库的过程[DB/OL]. https://typecodes.com/cseries/gccgensharedlib.html
9. René Nyffenegger's. Creating a shared and static library with the gnu compiler(gcc)[DB/OL]. http://www.adp-gmbh.ch/cpp/gcc/create_lib.html
10. 陈皓. 跟我一起写Makefile[DB/OL]. 2005：15-16.
11. 陈皓. 跟我一起写Makefile[DB/OL]. 2005：24-25.
12. maimake. shell 相对路径转绝对路径[DB/OL]. http://blog.sina.com.cn/s/blog_7a691e910101gnra.html