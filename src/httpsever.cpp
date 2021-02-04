#include "database.hpp"
#include "httplib.h"

using namespace httplib;

Tag* tag=new Tag;
Blog* blog=new Blog;

void InsertBlog(const Request& req,Response& res)
{
    //把req中的正文进行反序列化
    Json::Reader reader;
    Json::Value bg;
    Json::FastWriter fw;
    Json::Value errmsg;
    bool ret=reader.parse(req.body,bg);
    if(!ret)
    {
        cout<<"InsertBlog parse blog failed"<<endl;
        res.status=400;
        errmsg["ok"]=false;
        errmsg["reason"]="parse blog json failed";
        res.body=fw.write(errmsg);
        return;
    }
    cout<<"InsertBlog parse blog success"<<endl;
    ret=blog->InsertBlog(bg);
    if(!ret)
    {
        cout<<"InsertBlog insert into database failed"<<endl;
        res.status=500;
        return;
    }
    res.status=200;
    cout<<"InsertBlog insert into database success"<<endl;
    return;
}
void UpdateBlog(const Request& req,Response& res)
{
    int blog_id=stoi(req.matches[1]);
    Json::Value bg;
    Json::Reader reader;
    bool ret=reader.parse(req.body,bg);
    if(!ret)
    {
        cout<<"UpdateBlog parse blog failed"<<endl;
        res.status=400;
        return;
    }
    cout<<"UpdateBlog parse blog success"<<endl;

    bg["blog_id"]=blog_id;
    ret=blog->UpdateBlog(bg);
    if(!ret)
    {
        cout<<"UpdateBlog  database failed"<<endl;
        res.status=500;
        return;
    }
    res.status=200;
    cout<<"UpdateBlog database success"<<endl;
    return;
}
void DeleteBlog(const Request& req,Response& res)
{
    int blog_id=stoi(req.matches[1]);
    bool ret=blog->DeleteBlog(blog_id);
    if(!ret)
    {
        cout<<"DeleteBlog delete from database failed"<<endl;
        res.status=500;
        return;
    }
    res.status=200;
    cout<<"DeleteBlog delete from database success"<<endl;
    return;
}
void GetAllBlog(const Request& req,Response& res)
{
    Json::Value bgs;
    bool ret=blog->GetAllBlog(&bgs);
    if(!ret)
    {
        cout<<"GetAllBlog get all blog from database failed"<<endl;
        res.status=500;
        return;
    }
    Json::StyledWriter sw;
    //res.body=sw.write(bgs);
    res.set_content(sw.write(bgs),"application/json");
    res.status=200;
    cout<<"GetAllBlog get all blog from database success"<<endl;
    return;
}
void GetOneBlog(const Request& req,Response& res)
{
    Json::Value bg;
    int blog_id=stoi(req.matches[1]);
    bg["blog_id"]=blog_id;
    bool ret=blog->GetOneBlog(&bg);
    if(!ret)
    {
        cout<<"GetOneBlog get one blog from database failed"<<endl;
        res.status=500;
        return;
    }
    Json::FastWriter fw;
    //res.body=fw.write(bg);
    res.set_content(fw.write(bg),"application/json");
    cout<<"GetOneBlog get one blog from database success"<<endl;
    res.status=200;
    return;
}
void InsertTag(const Request& req,Response& res)
{
    //把req中的正文进行反序列化
    Json::Reader reader;
    Json::Value tg;
    Json::FastWriter fw;
    Json::Value errmsg;
    bool ret=reader.parse(req.body,tg);
    if(!ret)
    {
        cout<<"InsertTag parse tag failed"<<endl;
        res.status=400;
        errmsg["ok"]=false;
        errmsg["reason"]="parse tag json failed";
        res.body=fw.write(errmsg);
        return;
    }
    cout<<"InsertTag parse tag success"<<endl;
    ret=tag->InsertTag(tg);
    if(!ret)
    {
        cout<<"InsertTag insert into database failed"<<endl;
        res.status=500;
        return;
    }
    res.status=200;
    cout<<"InsertTag insert into database success"<<endl;
    
}
void UpdateTag(const Request& req,Response& res)
{
    int tag_id=stoi(req.matches[1]);
    Json::Value tg;
    Json::Reader reader;
    bool ret=reader.parse(req.body,tg);
    if(!ret)
    {
        cout<<"Updatetag parse Tag failed"<<endl;
        res.status=400;
        return;
    }
    cout<<"Updatetag parse Tag success"<<endl;

    tg["tag_id"]=tag_id;
    ret=tag->UpdateTag(tg);
    if(!ret)
    {
        cout<<"Updatetag  database failed"<<endl;
        res.status=500;
        return;
    }
    res.status=200;
    cout<<"Updatetag database success"<<endl;
    return;
}
void DeleteTag(const Request& req,Response& res)
{
    int tag_id=stoi(req.matches[1]);
    bool ret=tag->DeleteTag(tag_id);
    if(!ret)
    {
        cout<<"DeleteTag delete from database failed"<<endl;
        res.status=500;
        return;
    }
    res.status=200;
    cout<<"DeleteTag delete from database success"<<endl;
    return;
}
void GetAllTag(const Request& req,Response& res)
{
    Json::Value tgs;
    bool ret=tag->GetAllTag(&tgs);
    if(!ret)
    {
        cout<<"GetAllTag get all tag from database failed"<<endl;
        res.status=500;
        return;
    }
    Json::StyledWriter sw;
    //res.body=sw.write(tgs);
    res.set_content(sw.write(tgs),"application/json");
    res.status=200;
    cout<<"GetAllTag get all tag from database success"<<endl;
    return;
}
void GetOneTag(const Request& req,Response& res)
{
    Json::Value tg;
    int tag_id=stoi(req.matches[1]);
    tg["tag_id"]=tag_id;
    bool ret=tag->GetOneTag(&tg);
    if(!ret)
    {
        cout<<"GetOnetag get one Tag from database failed"<<endl;
        res.status=500;
        return;
    }
    Json::StyledWriter sw;
    //res.body=sw.write(tg);
    res.set_content(sw.write(tg),"application/json");
    cout<<"GetOnetag get one Tag from database success"<<endl;
    res.status=200;
    return;
}

void Login(const Request& req,Response& res)
{
    Json::Value req_json;
    Json::Value res_json;
    Json::Reader reader;
    reader.parse(req.body,req_json);
    cout<<req_json["username"]<<endl;;
    cout<<req_json["passwd"]<<endl;
    if(req_json["username"]=="1234"&&req_json["passwd"]=="1234")
    {
        res_json["is_username"]==true;
        res_json["is_passwd"]==true;
    }
    Json::FastWriter fw;
    fw.write(res_json);
    return;
}
int main()
{
    Server svr;
    svr.set_base_dir("../bin/ww");
    
 //   svr.Post("/",Login);

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
    
    svr.listen("0.0.0.0",1201);
    return 0;
}

