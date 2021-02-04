#include <mutex>
#include <iostream>
#include <string>
#include<mysql/mysql.h>
#include<json/json.h>

//#define MYSQL_HOST "172.25.150.42"
#define MYSQL_HOST "127.0.0.1"
using namespace std;

MYSQL* MysqlInit(MYSQL* mysql)
{
    mysql=mysql_init(NULL);
    if(mysql==NULL)
    {
        printf("mysql init failed\n");
        return NULL;
    }
    if(!mysql_real_connect(mysql,MYSQL_HOST,"root","","blog_system",3306,NULL,0))
    {
        //出错了
        fprintf(stderr,"mysql connect failed error : %s\n",mysql_error(mysql));
        mysql_close(mysql);
        return NULL;
    }
    cout<<"mysql connect success"<<endl;
    //设置字符集
    mysql_set_character_set(mysql,"utf8");

    //选择数据库 返回值0成功
    if(mysql_select_db(mysql,"blog_system"))
    {
        fprintf(stderr,"mysql select database failed error : %s\n",mysql_error(mysql));
        mysql_close(mysql);
        return NULL;
    }
    cout<<"mysql select database success"<<endl;
    return mysql;
}

bool MysqlQuery(MYSQL* mysql,const char* sql)
{
    if(mysql_query(mysql,sql))
    {
        fprintf(stderr,"mysql exec sql:[%s] failed error : %s\n",sql,mysql_error(mysql));
        return false;
    }
    printf("mysql exec sql:[%s] success\n",sql);
    return true;
}

void MysqlDestroy(MYSQL* mysql)
{
    if(mysql)
    {
        mysql_close(mysql);
    }
}

class Tag
{
public:
    Tag()
    {
        _mysql=MysqlInit(_mysql);
    }
    
    bool InsertTag(Json::Value& tag)
    {
        #define INSERT_TAG "insert into tag values(null,'%s');"
        char tmp[1024]={0};
        sprintf(tmp,INSERT_TAG,tag["tag_name"].asCString());
        bool ret=MysqlQuery(_mysql,tmp);
        return ret;
    }

    bool UpdateTag(Json::Value& tag)
    {
        #define UPDATE_TAG "update tag set tag_name='%s' where tag_id=%d;"
        char tmp[1024]={0};
        sprintf(tmp,UPDATE_TAG,tag["tag_name"].asCString(),tag["tag_id"].asInt());
        bool ret=MysqlQuery(_mysql,tmp);
        return ret;
    }

    bool DeleteTag(int tag_id)
    {
        #define DELETE_TAG "delete from tag where tag_id=%d;"
        char tmp[1024]={0};
        sprintf(tmp,DELETE_TAG,tag_id);
        bool ret=MysqlQuery(_mysql,tmp);
        return ret;
    }

    bool GetAllTag(Json::Value* tags)
    {
        #define GETALL_TAG "select * from tag;"
        _mutex.lock();
        bool ret=MysqlQuery(_mysql,GETALL_TAG);
        if(!ret)
        {
            _mutex.unlock();
            return false;
        }

        MYSQL_RES* res=mysql_store_result(_mysql);
        _mutex.unlock();
        if(res==NULL)
        {
            fprintf(stderr,"mysql store all result failed error : %s\n",mysql_error(_mysql));
            return false;
        }
        int row_num=mysql_num_rows(res);
        for(int i=0;i<row_num;i++)
        {
            MYSQL_ROW row=mysql_fetch_row(res);
            Json::Value tag;
            tag["tag_id"]=stoi(row[0]);
            tag["tag_name"]=row[1];
            tags->append(tag);
        }
        mysql_free_result(res);
        return true;
    }

    bool GetOneTag(Json::Value* tag)
    {
        
        #define GETONE_TAG "select tag_name from tag where tag_id=%d;"
        char tmp[1024]={0};
        sprintf(tmp,GETONE_TAG,(*tag)["tag_id"].asInt());
        _mutex.lock();
        bool ret=MysqlQuery(_mysql,tmp);
        if(!ret)
        {
            _mutex.unlock();
            return false;
        }

        MYSQL_RES* res=mysql_store_result(_mysql);
        _mutex.unlock();
        if(res==NULL)
        {
            fprintf(stderr,"mysql store one result failed error : %s\n",mysql_error(_mysql));
            return false;
        }
        int row_num=mysql_num_rows(res);
        if(row_num!=1)
        {
            printf("mysql get one result error\n");
            mysql_free_result(res);
            return false;
        }
        MYSQL_ROW row=mysql_fetch_row(res);
        (*tag)["tag_name"]=row[0];
        mysql_free_result(res);
        return true;
    }
private:
    MYSQL* _mysql;
    mutex _mutex;
};

class Blog
{
public:
    Blog()
    {
        _mysql=MysqlInit(_mysql);
    }
    
    bool InsertBlog(Json::Value& blog)
    {
        #define INSERT_BLOG "insert into blog values(null,'%d','%s','%s',null);"
        int len=blog["content"].asString().size()+4096;
        char* tmp=(char*)malloc(len);
        sprintf(tmp,INSERT_BLOG, blog["lable_id"].asInt(),blog["blog_title"].asCString(),blog["content"].asCString());
        bool ret= MysqlQuery(_mysql,tmp);
        free(tmp);
        return ret;
    }

    bool UpdateBlog(Json::Value& blog)
    {
        #define UPDATE_BLOG "update blog set lable_id=%d,blog_title='%s',content='%s' where blog_id=%d;"
        int len=blog["content"].asString().size()+4096;
        char* tmp=(char*)malloc(len);
        sprintf(tmp,UPDATE_BLOG,blog["lable_id"].asInt(),blog["blog_title"].asCString(),blog["content"].asCString(),blog["blog_id"].asInt());
        bool ret= MysqlQuery(_mysql,tmp);
        free(tmp);
        return ret;
    }
    
    bool DeleteBlog(int blog_id)
    {
        #define DELETE_BLOG "delete from blog where blog_id=%d;"
        char tmp[1024]={0};
        sprintf(tmp,DELETE_BLOG,blog_id);
        bool ret= MysqlQuery(_mysql,tmp);
        return ret;
    }
    
    bool GetAllBlog(Json::Value* blogs)
    {
        #define GETALL_BLOG "select blog_id,lable_id,blog_title,blog_time from blog;"
        _mutex.lock();
        bool ret=MysqlQuery(_mysql,GETALL_BLOG);
        if(!ret)
        {
            _mutex.unlock();
            return false;    
        }
        //保存结果集
        MYSQL_RES* res= mysql_store_result(_mysql);
        _mutex.unlock();
        if(res==NULL)
        {
            fprintf(stderr,"mysql store all result failed error : %s\n",mysql_error(_mysql));
            return false;
        }
        //遍历结果集
        int row_num=mysql_num_rows(res);
        for(int i=0;i<row_num;i++)
        {
            MYSQL_ROW row=mysql_fetch_row(res);
            Json::Value blog;
            blog["blog_id"]=stoi(row[0]);
            blog["lable_id"]=stoi(row[1]);
            blog["blog_title"]=row[2];
            blog["blog_time"]=row[3];
            blogs->append(blog);
        }
        mysql_free_result(res);
        return true;
    }

    bool GetOneBlog(Json::Value* blog)
    {
        #define GETONE_BLOG "select lable_id,blog_title,content,blog_time from blog where blog_id=%d;"
        char tmp[1024]={0};
        sprintf(tmp,GETONE_BLOG,(*blog)["blog_id"].asInt());
        _mutex.lock();
        bool ret= MysqlQuery(_mysql,tmp);
        if(ret==false)
        {
            _mutex.unlock();
            return false;
        }
        MYSQL_RES* res= mysql_store_result(_mysql);
        _mutex.unlock();
        if(res==NULL)
        {
            fprintf(stderr,"mysql store one result failed error : %s\n",mysql_error(_mysql));
            return false;
        }
        int row_num=mysql_num_rows(res);
        if(row_num!=1)
        {
            printf("mysql get one result error\n");
            mysql_free_result(res);
            return false;
        }
        MYSQL_ROW row=mysql_fetch_row(res);
        (*blog)["lable_id"]=stoi(row[0]);
        (*blog)["blog_title"]=row[1];
        (*blog)["content"]=row[2];
        (*blog)["blog_time"]=row[3];
        mysql_free_result(res);
        return true;
    }

private:
    MYSQL* _mysql;
    mutex _mutex;
};


