drop database if exists blog_system;
create database if not exists  blog_system;

use blog_system;

drop table if exists tag;
create table if not exists tag
(
    tag_id int primary key auto_increment comment '标签id',
    tag_name varchar(32) comment '标签名称'
);

drop table if exists blog;
create table if not exists blog
(
    blog_id int primary key auto_increment comment '博客id',
    lable_id int comment '所属标签id',
    blog_title varchar(255) comment '博客标题',
    content text comment '博客内容',
    blog_time timestamp  comment '博客创建时间'
);

insert into tag values(null,'C'),(null,'C++'),(null,'DS'),(null,'Linux'),(null,'MySQL');
insert into blog values(null,1,'C语言博客','C语言简单',null),
                       (null,2,'C++博客','C++语言很难',null);
