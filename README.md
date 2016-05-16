在内存中模拟文件系统的操作
本程序用 c89编写
在此模拟文件系统中可以进行的操作有
    1：改变目录：cd 目录名          例：cd /usr/lib
    2：创建文件：edit 文件名        例：edit /usr/lib/file
    3：删除文件：del 文件名         例：del /usr/lib/file
    4：显示目录：dir                例：dir
    5：创建目录：md 目录名          例：md /home/user/doc
    6：删除目录：rd 目录名          例：rd /home/user/doc
其他说明：
    a:目录名和文件名均支持相对路径和绝对路径
    b:路径之间用符号 '/' 分割
    c:输入exit可以退出此模拟系统
    d:本系统初始时自带了一颗目录树
