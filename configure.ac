#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([myfcp],[2.0.1],[yldfree@163.com])

#包的版本号
VERSION=2.0.1

#生成库的版本号
LT_VERSION=2:0:1

AC_CONFIG_SRCDIR([configure.ac])

#将一些辅助脚本文件或m4文件放到m4目录
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([m4])

#AM_PATH_LOG4C([log])

AM_CONFIG_HEADER(src/config.h)

#输出能被Makefile.am使用的变量
AC_SUBST(VERSION)   
AC_SUBST(LT_VERSION)

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stddef.h stdlib.h string.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile 
		 src/Makefile 
		 src/myfcp/Makefile 
		 src/example/Makefile ])
AC_OUTPUT
