#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <stack>
#include <bitset>
#include <sys/stat.h>   //file system stuff
#include <sys/types.h>  //file system stuff
#include <unistd.h>   //file system stuff
#include <dirent.h>
#include <stdlib.h>
#include <algorithm>
#include <codecvt>

using namespace std;

bool PLATFORM_WIN_32 = false;
bool PLATFORM_WIN_64 = false;
bool PLATFORM_LINUX_32 = false;
bool PLATFORM_LINUX_64 = false;
bool PLATFORM_WEB = false;
bool PLATFORM_ANDROID = false;
string PROJECT_NAME = "";
string PROJECT_CATEGORY = "AudioVideo";
string APP_TYPE = "Application";
bool TERMINAL_FLAG = false;
string PROJECT_DIR = "";
string OUTPUT_DIR = "";
bool ENABLE_WEB_THREADS = false;
string ICON = "";
string SOURCE = "";
string ANDROID_APP_ID = "";
string ANDROID_ORIENTATION = "";
string ANDROID_KEYSTORE = "";
string ANDROID_KEYSTORE_PASS = "";
string ANDROID_ALIAS = "";
string ANDROID_ALIAS_PASS = "";
bool ANDROID_RELEASE = false;
bool ANDROID_DEBUG = false;
string ANDROID_JAVA_DIR = "";
string PROJECT_OUTDIR_NAME = "";

string PROJECT_CBC = "";

string DIST_PKG_PATH = "";

struct dirent *rc_entry;
DIR *rc_dir;

int dirExist(std::string d_path)
{
    struct stat info;

    if(stat( d_path.c_str(), &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

std::string dirFirst (std::string rc_dir_path)
{
    rc_dir = opendir (rc_dir_path.c_str());
    //string s = "";

    if ((rc_entry = readdir (rc_dir)) != NULL)
    {
        //cout << "ERROR: " << rc_entry->d_name;
        return rc_entry->d_name;
    }
    return "";
}

std::string dirNext()
{
    if( (rc_entry = readdir(rc_dir))!=NULL)
        return rc_entry->d_name;
    return "";
}

int dirCreate(std::string d_path)
{
    if(mkdir(d_path.c_str())!=0)
    {
        cout << "ERROR: Could not make directory" << endl;
        return 0;
    }
    return 1;
}

int dirDelete(std::string d_path)
{
    #if defined(WIN32) || defined(WIN64)
    string cmd = "rmdir /S /Q \"" + d_path + "\"";
    #else
    string cmd = "rm -rf \"" + d_path + "\"";
    #endif // defined

    return system(cmd.c_str());
}

string getDirPath(string path)
{
    string mod_path = path;

    #if defined(WIN32) || defined(WIN64)
    string d_sep = "\\";
    #else
    string d_sep = "/";
    #endif // defined

    while(mod_path.find_first_of(d_sep) >= 0 && mod_path.find_first_of(d_sep) < mod_path.length())
    {
        if(dirExist(mod_path))
            return mod_path;
        int d_sep_pos = mod_path.find_last_of(d_sep);
        if(d_sep_pos < 0 || d_sep_pos >= mod_path.length())
            return "";
        else if(d_sep_pos == 0)
            return d_sep;
        mod_path = mod_path.substr(0, d_sep_pos);
    }
    return "";
}

int fileCopy(std::string src_file, std::string dst_file)
{
    std::ifstream  src(src_file.c_str(), std::ios::binary);
    std::ofstream  dst(dst_file.c_str(), std::ios::binary);
    if(!(src.is_open() && dst.is_open()))
    {
        if(src.is_open())
            src.close();
        if(dst.is_open())
            dst.close();
        return 0;
    }
    dst << src.rdbuf();
    src.close();
    dst.close();
    return 1;
}

int fileDelete(std::string tgt_file)
{
    if(remove(tgt_file.c_str())==0)
        return 1;
    else
        return 0;
}

int fileExist(std::string tgt_file)
{
    std::ifstream infile(tgt_file.c_str());
    bool fx = infile.good();
    infile.close();
    return (int)fx;
}

int fileMove(std::string src, std::string dst)
{
    int fm = rename(src.c_str(), dst.c_str());
    if(fm == 0)
        return 1;
    else
        return 0;
}

int fileRename(std::string src, std::string dst)
{
    int fm = rename(src.c_str(), dst.c_str());
    if(fm == 0)
        return 1;
    else
        return 0;
}


void debug_output()
{
    cout << "PLATFORM_WIN_32 = " << (PLATFORM_WIN_32 == true ? "true" : "false") << endl;
    cout << "PLATFORM_WIN_64 = " << (PLATFORM_WIN_64 == true ? "true" : "false") << endl;
    cout << "PLATFORM_LINUX_32 = " << (PLATFORM_LINUX_32 == true ? "true" : "false") << endl;
    cout << "PLATFORM_LINUX_64 = " << (PLATFORM_LINUX_64 == true ? "true" : "false") << endl;
    cout << "PLATFORM_WEB = " << (PLATFORM_WEB == true ? "true" : "false") << endl;
    cout << "PLATFORM_ANDROID = " << (PLATFORM_ANDROID == true ? "true" : "false") << endl;
    cout << "PROJECT_NAME = " << PROJECT_NAME << endl;
    cout << "PROJECT_CATEGORY = " << PROJECT_CATEGORY << endl;
    cout << "APP_TYPE = " << APP_TYPE << endl;
    cout << "TERMINAL_FLAG = " << (TERMINAL_FLAG == true ? "true" : "false") << endl;
    cout << "PROJECT_DIR = " << PROJECT_DIR << endl;
    cout << "OUTPUT_DIR = " << OUTPUT_DIR << endl;
    cout << "ENABLE_WEB_THREADS = " << (ENABLE_WEB_THREADS == true ? "true" : "false") << endl;
    cout << "ICON = " << ICON << endl;
    cout << "SOURCE = " << SOURCE << endl;
    cout << "ANDROID_APP_ID = " << ANDROID_APP_ID << endl;
    cout << "ANDROID_ORIENTATION = " << ANDROID_ORIENTATION << endl;
    cout << "ANDROID_KEYSTORE = " << ANDROID_KEYSTORE << endl;
    cout << "ANDROID_KEYSTORE_PASS = " << ANDROID_KEYSTORE_PASS << endl;
    cout << "ANDROID_ALIAS = " << ANDROID_ALIAS << endl;
    cout << "ANDROID_ALIAS_PASS = " << ANDROID_ALIAS_PASS << endl;
    cout << "ANDROID_RELEASE = " << (ANDROID_RELEASE ? "true" : "false") << endl;
    cout << "ANDROID_DEBUG = " << (ANDROID_DEBUG ? "true" : "false") << endl;
    cout << "ANDROID_JAVA_DIR = " << ANDROID_JAVA_DIR << endl;
}

void set_project_outdir_name()
{
    PROJECT_OUTDIR_NAME = PROJECT_NAME;
    string tmp = "";
    for(int i = 0; i < PROJECT_OUTDIR_NAME.length(); i++)
    {
        string c = PROJECT_OUTDIR_NAME.substr(i,1);
        if(c.compare(" ")==0 || c.compare("\t")==0 || c.compare("\n")==0 || c.compare("\r")==0)
            c = "_";
        tmp += c;
    }
    PROJECT_OUTDIR_NAME = tmp;
}

void set_param(string param, string param_val)
{
    if(param.compare("TGT_PLATFORM")==0)
    {
        string arg = "";
        param_val +=  ",";
        for(int i = 0; i < param_val.length(); i++)
        {
            string c = param_val.substr(i,1);
            if(c.compare(",")==0)
            {
                if(arg.compare("WIN_32")==0)
                    PLATFORM_WIN_32 = true;
                else if(arg.compare("WIN_64")==0)
                    PLATFORM_WIN_64 = true;
                else if(arg.compare("LINUX_32")==0)
                    PLATFORM_LINUX_32 = true;
                else if(arg.compare("LINUX_64")==0)
                    PLATFORM_LINUX_64 = true;
                else if(arg.compare("WEB")==0)
                    PLATFORM_WEB = true;
                else if(arg.compare("ANDROID")==0)
                    PLATFORM_ANDROID = true;

                arg = "";
            }
            else
                arg += c;
        }
    }
    else if(param.compare("PROJECT_NAME")==0)
    {
        PROJECT_NAME = param_val;
        set_project_outdir_name();
    }
    else if(param.compare("PROJECT_CATEGORY")==0)
    {
        PROJECT_CATEGORY = param_val;
    }
    else if(param.compare("APP_TYPE")==0)
    {
        APP_TYPE = param_val;
    }
    else if(param.compare("TERMINAL_FLAG")==0)
    {
        TERMINAL_FLAG = (param_val.compare("true")==0 ? true : false);
    }
    else if(param.compare("PROJECT_DIR")==0)
    {
        PROJECT_DIR = param_val;
    }
    else if(param.compare("OUTPUT_DIR")==0)
    {
        OUTPUT_DIR = param_val;
    }
    else if(param.compare("ENABLE_WEB_THREADS")==0)
    {
        ENABLE_WEB_THREADS = (param_val.compare("true")==0 ? true : false);
    }
    else if(param.compare("ICON")==0)
    {
        ICON = param_val;
    }
    else if(param.compare("SOURCE")==0)
    {
        SOURCE = param_val;
    }
    else if(param.compare("ANDROID_APP_ID")==0)
    {
        ANDROID_APP_ID = param_val;
    }
    else if(param.compare("ANDROID_ORIENTATION")==0)
    {
        ANDROID_ORIENTATION = param_val;
    }
    else if(param.compare("ANDROID_KEYSTORE")==0)
    {
        ANDROID_KEYSTORE = param_val;
    }
    else if(param.compare("ANDROID_KEYSTORE_PASS")==0)
    {
        ANDROID_KEYSTORE_PASS = param_val;
    }
    else if(param.compare("ANDROID_ALIAS")==0)
    {
        ANDROID_ALIAS = param_val;
    }
    else if(param.compare("ANDROID_ALIAS_PASS")==0)
    {
        ANDROID_ALIAS_PASS = param_val;
    }
    else if(param.compare("ANDROID_RELEASE")==0)
    {
        ANDROID_RELEASE = (param_val.compare("1")==0 || param_val.compare("true")==0 ? true : false);
    }
    else if(param.compare("ANDROID_DEBUG")==0)
    {
        ANDROID_DEBUG = (param_val.compare("1")==0 || param_val.compare("true")==0 ? true : false);
    }
    else if(param.compare("ANDROID_JAVA_DIR")==0)
    {
        ANDROID_JAVA_DIR = param_val;
    }

}

bool parse(string arg)
{
    string param[2];
    int p_index = 0;
    bool in_quote = false;
    for(int i = 0; i < arg.length(); i++)
    {
        string c = arg.substr(i, 1);
        if(c.compare(" ")==0 && (!in_quote))
        {
            set_param(param[0], param[1]);
            param[0] = "";
            param[1] = "";
            p_index = 0;
        }
        else if(c.compare("\"")==0)
        {
            in_quote = !in_quote;
        }
        else if(c.compare("=")==0)
        {
            p_index = (p_index == 0 ? 1 : 0);
        }
        else
            param[p_index] += c;
    }

    string end_char = OUTPUT_DIR.substr(OUTPUT_DIR.length()-1, 1);


    #if defined(WIN32) || defined(WIN64)
    if(end_char.compare("\\")!=0)
    {
        OUTPUT_DIR += "\\";
    }
    #else
    if(end_char.compare("/")!=0)
    {
        OUTPUT_DIR += "/";
    }
    #endif // defined

    OUTPUT_DIR += PROJECT_OUTDIR_NAME;

    if(!dirExist(PROJECT_DIR))
        return false;

    if(!dirExist(OUTPUT_DIR))
    {
        dirCreate(OUTPUT_DIR);
    }

    if(!dirExist(OUTPUT_DIR))
        return false;

    return true;
}

string appendFileToPath(string path, string file)
{
    if(path.length() < 1)
        return file;

    string end_char = path.substr(path.length()-1, 1);

    #if defined(WIN32) || defined(WIN64)
    if(end_char.compare("\\")!=0)
    {
        path += "\\";
    }
    #else
    if(end_char.compare("/")!=0)
    {
        path += "/";
    }
    #endif // defined

    path +=  file;

    return path;
}

bool dist_win(int arch)
{
    string arch_label = "";
    string WIN_DIST_DIR = DIST_PKG_PATH;

    if(arch == 32)
    {
        if(!PLATFORM_WIN_32)
            return true;
        arch_label = "_WIN32";
        WIN_DIST_DIR = appendFileToPath(WIN_DIST_DIR, "WIN_32");
    }
    else if(arch == 64)
    {
        if(!PLATFORM_WIN_64)
            return true;
        arch_label = "_WIN64";
        WIN_DIST_DIR = appendFileToPath(WIN_DIST_DIR, "WIN_64");
    }
    else
    {
        cout << "Error: Invalid Window Arch -> " << arch << endl;
        return false;
    }

    string WIN_OUTPUT_DIR = OUTPUT_DIR;

    string end_char = WIN_OUTPUT_DIR.substr(WIN_OUTPUT_DIR.length()-1, 1);

    #if defined(WIN32) || defined(WIN64)
    if(end_char.compare("\\")!=0)
    {
        WIN_OUTPUT_DIR += "\\";
    }
    #else
    if(end_char.compare("/")!=0)
    {
        WIN_OUTPUT_DIR += "/";
    }
    #endif // defined

    WIN_OUTPUT_DIR += PROJECT_OUTDIR_NAME + arch_label;

    if(dirExist(WIN_OUTPUT_DIR))
        dirDelete(WIN_OUTPUT_DIR);

    dirCreate(WIN_OUTPUT_DIR);

    if(!dirExist(WIN_OUTPUT_DIR))
        return false;

    //cout << "WIN32_DIR = " << WIN32_OUTPUT_DIR << endl;


    //-----COPY PROJECT FILES TO OUTPUT DIRECTORY-----
    string dfile = dirFirst(PROJECT_DIR);

    while(dfile.compare("") != 0)
    {

        string dfile_src = appendFileToPath(PROJECT_DIR, dfile);
        string dfile_dst = appendFileToPath(WIN_OUTPUT_DIR, dfile);
        //cout << "file: " << dfile << endl;

        fileCopy(dfile_src, dfile_dst);

        dfile = dirNext();
    }

    //-----COPY DISTRIBUTABLE BINARIES TO OUTPUT DIRECTORY-----
    dfile = dirFirst(WIN_DIST_DIR);

    while(dfile.compare("") != 0)
    {

        string dfile_src = appendFileToPath(WIN_DIST_DIR, dfile);
        string dfile_dst = appendFileToPath(WIN_OUTPUT_DIR, dfile);
        //cout << "file: " << dfile << endl;

        fileCopy(dfile_src, dfile_dst);

        dfile = dirNext();
    }

    return true;
}


int main(int argc, char * argv[])
{
    string args = "";
    args = "TGT_PLATFORM=WIN_32,WIN_64,LINUX_64,WEB PROJECT_NAME=\"tile demo\" PROJECT_CATEGORY=AudioVideo APP_TYPE=Application TERMINAL_FLAG=false PROJECT_DIR=\"C:\\dev_libs\\RCBasic-Studio\\bin\\Release305\\examples\\tile_demo\" OUTPUT_DIR=\"C:\\Users\\Shadow\\Desktop\\test\" ENABLE_WEB_THREADS=false ICON=\"C:\\dev_libs\\RCBasic-Studio\\gfx\\symbol_fn_item.png\" SOURCE=\"test_demo.bas\" ANDROID_APP_ID=\"app.test.com\" ANDROID_ORIENTATION=\"default\" ANDROID_KEYSTORE=\"mykeystore.ts\" ANDROID_KEYSTORE_PASS=\"\" ANDROID_ALIAS=\"myAlias\" ANDROID_ALIAS_PASS=\"\" ANDROID_RELEASE=1 ANDROID_DEBUG=0 ANDROID_JAVA_DIR=\"jv_src.java\" RCBASIC_STUDIO=1";

    DIST_PKG_PATH = getDirPath(argv[0]);
    DIST_PKG_PATH = appendFileToPath(DIST_PKG_PATH, "..");
    DIST_PKG_PATH = appendFileToPath(DIST_PKG_PATH, "dist");

    //cout << "dist path = " << DIST_PKG_PATH << endl;

    if(argc > 1)
        args = argv[1];

    if(!parse(args))
        return 1;

    string build_cmd = "rcbasic_build \"" + appendFileToPath(PROJECT_DIR, SOURCE) + "\"";

    if(SOURCE.length() < 5)
    {
        cout << "Error: Missing *.bas source file" << endl;
        return 1;
    }

    if(SOURCE.substr(SOURCE.length()-4).compare(".bas")!=0)
    {
        cout << "Error: Missing *.bas source file" << endl;
        return 1;
    }

    string PROJECT_CBC = SOURCE.substr(0, SOURCE.length()-4) + ".cbc";
    //cout << "cbc = " << PROJECT_CBC << endl;

    system(build_cmd.c_str());

    if(!fileExist(appendFileToPath(PROJECT_DIR, PROJECT_CBC)))
    {
        cout << "Error: Failed to build " << SOURCE << endl;
        return 1;
    }

    if(PROJECT_CBC.compare("main.cbc")!=0)
    {
        fileRename(appendFileToPath(PROJECT_DIR, PROJECT_CBC), appendFileToPath(PROJECT_DIR, "main.cbc"));
        PROJECT_CBC = "main.cbc";
    }

    dist_win(32);
    dist_win(64);

    debug_output();

    return 0;
}
