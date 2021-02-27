# blog_system

bin目录下：存放了前端模块的实现

index.html 使用html+css+js实现，使用技术：Vue.js、jQuery Ajax

数据库原始文件（blog_system.sql），可直接导入mysql数据库中使用

src目录下：存放主要源文件+makefile文件

database.hpp:数据库调用模块,后台执行的业务逻辑对数据库增删查改

封装了C语言系列的mysql函数，更改ip地址为本机地址。

连接数据库，传入用户名密码，库的名称，默认3306端口  

   初始化: mysql_init;
   
   连接服务器: mysql_real_connect; 
   
   设置字符集: mysql_set_character_set;
   
   选择数据库: mysql_select_db
   
   遍历结果: mysql_fetch_row;
   
   保存结果集: mysql_store_result;
   
   获取结果行数: mysql_num_rows;
   
   获取结果列数: mysql_num_fields
   
   遍历结果: mysql_fetch_row;
   
   释放结果集: mysql_free_result;
   
   关闭数据库: mysql_close;
   
   获取接口错误原因: mysql_error

举例：组织sql语句，调用mysqlquery函数

     //向tag中插入新的标签
     bool InsertTag(Json::Value& tag)
     {
          #define INSERT_TAG "insert into tag values(null,'%s');"
          char tmp[1024]={0};
          sprintf(tmp,INSERT_TAG,tag["tag_name"].asCString());
          bool ret=MysqlQuery(_mysql,tmp);
          return ret;
      }


httpsever.cpp:http服务器模块，基于httplib搭建，采用RESTful风格进行接口设计，对用户不同的请求进行相应的业务处理。
      
      初始化server类
      Server svr;
      //设置逻辑根目录
      svr.set_base_dir("../bin/ww");  
      //Resful接口设计
      svr.Post("/blog",InsertBlog);//插入博客
      svr.Put(R"(/blog/(\d+))",UpdateBlog);//更新博客
      svr.Delete(R"(/blog/(\d+))",DeleteBlog);//删除博客
      svr.Get("/blog",GetAllBlog);//查询所有博客
      svr.Get(R"(/blog/(\d+))",GetOneBlog);//查询一个博客
  
      svr.Post("/tag",InsertTag);//插入标签
      svr.Put(R"(/tag/(\d+))",UpdateTag);//更新标签
      svr.Delete(R"(/tag/(\d+))",DeleteTag);//删除标签
      svr.Get("/tag",GetAllTag);//查询所有标签
      svr.Get(R"(/tag/(\d+))",GetOneTag);//查询一个标签
      
      //监听1201端口
      svr.listen("0.0.0.0",1201);


makefile：make进行编译，需要安装jsoncpp+mariadb+cpp-httplib

后期测试使用postman工具：
    
工具官网地址：https://www.postman.com/

