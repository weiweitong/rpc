





### substr

```C++
string substr (size_t pos = 0, size_t len = npos) const;
```

Generate substring

Returns a newly constructed [string](http://www.cplusplus.com/string) object with its value initialized to a copy of a substring of this object.



```c++
// 记得用 -std=11
size_t TrimStringT(const string& input,
                   const string& trim_chars,
                   size_t pos,
                   string* output)
{
  const size_t last_char = input.length() - 1;
  const size_t first_good_char = input.find_first_not_of(trim_chars);
	const size_t last_good_char = input.find_last_not_of(trim_chars);

  *output = input.substr(first_good_char, last_good_char - first_good_char + 1);
	// only set successful
  return 0;
}


// another knowledge of find_first_not_of
// Find absence of character in string
size_t find_first_not_of (const string& str, size_t pos = 0) const noexcept;
size_t find_first_not_of (const char* s, size_t pos = 0) const;
size_t find_first_not_of (const char* s, size_t pos, size_t n) const;
size_t find_first_not_of (char c, size_t pos = 0) const noexcept;


// another knowledge of find_last_not_of
// Find non-matching character in string from the end
size_t find_last_not_of (const string& str, size_t pos = npos) const noexcept;
size_t find_last_not_of (const char* s, size_t pos = npos) const;
size_t find_last_not_of (const char* s, size_t pos, size_t n) const;
size_t find_last_not_of (char c, size_t pos = npos) const noexcept;


/*
另一种典型的从文件中读取line
并且用分隔符
*/
size_t seperateStr(const string& str,
                 const string& delimiters,
                 std::vector<std::string>* tokens)
{
  tokens->clear();
  
  size_t start = str.find_first_not_of(delimiters);
  while(start != string::npos)
  {
    size_t end = str.find_first_of(delimiters, start+1);
    if (end = string::npos)
    {
      tokens->push_back(str.substr(start));
      break;
    } else
    {
      tokens->push_back(str.substr(start, end - start));
      start = str.find_first_not_of(delimiters, end + 1);
    }
  }
  return tokens->size();
}

```





### atof



```c++
double atof (const char* str);

string str;
float f = atof(str.c_str());
```





### ifstream

```c++
bool Load(const std::string& file_path){
  std::ifstream input_f(file_path.c_str() , std::ios_base::in);
  if (!input_f.is_open()) {
    P_LOG(ERROR) << "Failed to open [" << file_path << "] for input.";
    return false;
  }
}
```





### ToString

```c++
class Dict{
  std::string ToString();
}

std::string Dict::ToString() const {
  std::ostringstream oss;
  oss << "PosTransDict";
  return oss.str();
}
```





### typedef

creates an alias that can be used anywhere in place of a type name.

```c++
// simple typedef
typedef unsigned long ulong;
 
// the following two objects have the same type
unsigned long l1;
ulong l2;
```



**声明函数指针 **

```c++
typedef void*(*Fun)(int,int);

// 这里将返回类型为void *，入参为int的函数类型命名为Fun，那么在其他地方，就可以像下面这样使用啦：

#include <stdio.h>

// 这里需要注意用括号将这个别名括起来，并在前面加*号
typedef void*(*Fun)(int,int);

void *test(int a,int b)
{
    printf("%d,%d\n",a,b);
    //do something
    return NULL;
}

int main(void)
{
  	// 使用时，是没有 *Fun 的，但是声明的时候，是有 Fun 的
    Fun myfun = test;//这里的Fun已经是一种类型名了
    myfun(1,1);
    return 0;
}

```





```c++
// more complicated typedef
// fp 为一种函数 typedef，返回值为 int
// 输入值为 int 和 long 
typedef int int_t, *intp_t, (&fp)(int, ulong), arr_t[10];
 
// the following two objects have the same type
int a1[10];
arr_t a2;
 
// common C idiom to avoid having to write "struct S"
typedef struct {int a; int b;} S, *pS;
 
// the following two objects have the same type
pS ps1;
S* ps2;
 
// error: storage-class-specifier cannot appear in a typedef declaration
// typedef static unsigned int uint;
 
// typedef can be used anywhere in the decl-specifier-seq
long unsigned typedef int long ullong;
// more conventionally spelled "typedef unsigned long long int ullong;"
 
// std::add_const, like many other metafunctions, use member typedefs
template< class T>
struct add_const {
    typedef const T type;
};
 
typedef struct Node {
    struct listNode* next; // declares a new (incomplete) struct type named listNode
} listNode; // error: conflicts with the previously declared struct name
```



















