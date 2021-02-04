#include"database.hpp"

int main()
{
    //Tag tag;
    Blog blog;
    Json::Value v;
    v["blog_id"]=3;
    /*v["lable_id"]=4;
    v["blog_title"]="博客测试";
    v["content"]="611111111111111111111116666";
    */
    //blog.InsertBlog(v);
    //blog.UpdateBlog(v);
    //blog.GetAllBlog(&v);
    //blog.GetOneBlog(&v);
    //v["tag_name"]="C++";
    //tag.InsertTag(v);
    //tag.DeleteTag(7);
    //tag.DeleteTag(8);
    //tag.DeleteTag(9);
    //v["tag_id"]=3;
    //v["tag_name"]="VC";
    //tag.UpdateTag(v);
    
    //tag.GetAllTag(&v);
    //Json::StyledWriter w;
    Json::StyledWriter w;
    cout<<w.write(v)<<endl;   
    
    return 0;
}

