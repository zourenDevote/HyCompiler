**2022-05-21**

- 将Sy2022所用公开的测试用例以`submodule`的形式添加到了`hydrogencompiler\test\`下，每次开发时，除了拉取一下`develop`，还需要更新以下`submodule`，以防止测试用例改变。

```bash
# 命令 —— 拉取develop分支
git pull origin develop

# 命令 —— 拉取submodule
git submodule foreach git pull origin master
```

**函数内联的方法**

> **不展开的情况**
>
> 1. 函数在参数传递中调用，不展开
> 2. 函数调用链存在递归，不展开
>
> **展开方式**
>
> 1. 函数展开成一个作用域
> 2. 作为值传递的函数，重新定义这样一个变量，展开
> 3. 作为指针传递的参数，直接传递参数

```c++
// 示例1， 参数无赋值
int add(int a, int b){
    return a + b;
}

int fun(int a){
    return add(a, a);
}

int main(){
    return fun(1);
}
// 展开后示例
int main(){
    int ret_main;
    {
        int a = 1;
        int ret_add;
        {
            int a = a;
            int b = a;
            ret_fun = a + b;
        }
        ret_main =  ret_fun;
    }
    return ret_main;
}
// 生成的IR是
define @int main():
        entry:
                1. int* %ret_main = alloca int align 4 define in entry
                2. int* %a = alloca int align 4 define in entry
                3. int* %ret_fun = alloca int align 4 define in entry
                4. int* %a1 = alloca int align 4 define in entry
                5. int* %b = alloca int align 4 define in entry
                6. store 1 to int* %a
                7. load int* %a to int %0
                8. store int %0 to int* %a1
                9. load int* %a1 to int %1
                10. store int %1 to int* %b
                11. load int* %a1 to int %2
                12. load int* %b to int %3
                13. int %4 = int %2 + int %3
                14. store int %4 to int* %ret_fun
                15. load int* %ret_fun to int %5
                16. store int %5 to int* %ret_main
                17. load int* %ret_main to int %6
                18. ret int %6
// 优化后的entry为
define @int main():
		entry:
				1. ret 2;
```

```c++
// 示例2 参数被赋值了
int add(int a, int b){
    b = a + 1;
    return a + b;
}

int fun(int a){
    return add(a, a);
}

int main(){
	return fun(1);
}
// 展开后
int main(){
    int ret_main;
    {
        int a = 1;
        int ret_fun;
        {
            int a = 1;
            int b = 1;
            b = a + 1;
           	ret_fun = a + b;
        }
        ret_main = ret_fun;
    }
    return ret_main;
}
// 生成IR为
define @int main():
        entry:
                1. int* %ret_main = alloca int align 4 define in entry
                2. int* %ret_fun = alloca int align 4 define in entry
                3. int* %a1 = alloca int align 4 define in entry
                4. int* %b = alloca int align 4 define in entry
                5. store 1 to int* %a1
                6. store 1 to int* %b
                7. load int* %a1 to int %0
                8. int %1 = int %0 + 1
                9. store int %1 to int* %b
                10. load int* %a1 to int %2
                11. load int* %b to int %3
                12. int %4 = int %2 + int %3
                13. store int %4 to int* %ret_fun
                14. load int* %ret_fun to int %5
                15. store int %5 to int* %ret_main
                16. load int* %ret_main to int %6
                17. ret int %6
// 优化的entry为
define @int main():
		entry:
				1. ret 3;
```

```c++
// 示例三，带数组
void printarray(int a[], int len){
    int i = 0;
    while(i < len){
		putint(a[i]);
        i = i + 1;
    }
}

int main(){
    int array[4][2] = {{1,2}, {3,4}, {5,6}, {7.8}};
    printarray(array[0], 2);
}

// 展开后
int main(){
    int array[4][2] = {{1,2}, {3,4}, {5,6}, {7.8}};
    {
        int i = 0;
        int len = 2;
        while(i < len){
            putint(array[0][i]);
            i = i + 1;
        }
    }
}

// 生成IR为

```

