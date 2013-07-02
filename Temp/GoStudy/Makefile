#从比较老的文档中抄的(比如使用了 gomake Makefile 来编译 )
#新的版本中使用Go命令行工具, 不再使用makefile

include $(GOROOT)/src/Make.inc

TARG=helloworld
GOFILES=\
	helloworld.go\
	

#可执行程序的makefile	
include $(GOROOT)/src/Make.cmd

#包的makefile
#include $(GOROOT)/src/Make.pkg