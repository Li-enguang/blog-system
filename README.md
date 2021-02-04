# blog_system

bin目录下：存放了前端模块的实现，使用html+css+js实现，使用技术：Vue.js、jQuery Ajax

数据库原始文件，可直接导入mysql数据库中

src目录下：存放主要源文件+makefile文件

database.hpp:数据库调用模块，封装了C语言系列的mysql函数，需更改ip地址为本机地址。

httpsever.cpp:http服务器模块，基于httplib搭建，采用RESTful风格进行接口设计，对用户不同的请求进行相应的业务处理。

makefile：make进行编译，需要安装jsoncpp+mariadb+cpp-httplib

后期测试使用postman工具：
    
官网地址：https://www.postman.com/

