# 氢氢编译器(🐕)

## 编译器框架

![设计](./frame.png)

## 构建

目前Compiler仅可以在Linux环境下构建，不支持``MSVC`

```shell
# 从github克隆下来
cd HyCompiler
# 构建ANTLR4运行时库支持,前提得先安装Java，执行项目目录下的脚本
python3 deploy_env.py
# 使用cmake构建项目
mkdir build && cd build && cmake ..
make
```

## 使用

```shell
./compiler -h
This is a compiler of Sy2022 Language
Usage:
  /home/devotes/CLionProjects/hydrogencompiler/cmake-build-debug/compiler/compiler [OPTION...] [optional args]

  -S, --ASM         out asm code (default: true)
      --emit-ast    out the ast
      --emit-ir     out the IR
  -o, --output arg  the output file (default: testcase.s)
      --std arg     standard out stream (default: stdout)
  -i, --input arg   the input file (default: "")
  -h, --help        print help
  -O, --O2          open pass
```

**选项介绍**

| 选项          | 介绍                                            |
| ------------- | ----------------------------------------------- |
| -S, --ASM     | 生成汇编代码                                    |
| --emit-ir     | 输出IR                                          |
| --emit-ast    | 输出ast                                         |
| --o, --output | 目标.s文件位置                                  |
| --std         | IR和ast的输出位置，默认为stdout, 可以传入文件名 |
| -i,--input    | 输入文件                                        |
| -h --help     | 打印帮助信息                                    |
| -O, --O2,-O2  | 开启优化                                        |

## 优化操作

[Slice Of Optimize Introduction](./slice/intro.pptx)

## 贡献者

- 邹仁([Devotes](https://github.com/zourenDevote)):中南大学

- 吴杭宇([FirePixel](https://github.com/Thirty-creed)):中南大学

##  指导老师

- 章佳琪: 一位优秀美丽的辅导员

## 工作分配

- 邹仁：负责前端，语义分析，IR数据结构定义，IR生成，平台无关优化
- 吴杭宇：负责LLVM IR的生成和调试验证前端的设计，负责汇编代码生成、调试并验证IR的正确性以及平台相关优化
- 共同：共同讨论整个编译器框架的设计，沟通设计编译器流水线模块间具体的接口定义。

## 特别鸣谢

- 辅导员老师章佳琪
- [RonxBulld:一位经验丰富的编译器领域工程师](https://gitee.com/RonxBulld)
- [孙子平学长:清华大学静态分析硕士在读](https://www.szp15.com/author/%E5%AD%99%E5%AD%90%E5%B9%B3/)
- openEuler Compiler Sig群组的大佬们!
- 2021届毕昇编译器一等奖：湖南大学何纪宏大佬！
- 毕昇编译器交流群里面的同学们!
- 给我们提供比赛平台以及培训课程的华为赞助方，老师们！

