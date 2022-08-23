# -*- coding=utf-8 -*-

"""
检查脚本
"""
import argparse
import os

# 用于辅助检测的全局变量
import shutil
import difflib
import sys

result_die:dict = {}


def readFile(file: str):
    """
    读取文件内容
    """
    try:
        fp = open(file, "rt+", encoding="utf-8")
        text = fp.read().splitlines()
        fp.close()
        return text
    except Exception as e:
        print("File open error! ", file)
        raise e


def writeFile(file: str, res):
    """
    写文件
    """
    with open(file, "wt+", encoding="utf-8") as fp:
        fp.write(res)


# 检测输出的方法
def check(result_dir: str):
    """
    检查结果
    """
    global  result_die
    file_die = os.listdir(result_dir)
    # 将shell输出的标准输出和实际输出组织称成Python能识别的数据结构
    for die in file_die:
        title = die[0:-4]
        if title not in result_die:
            result_die[title] = ['', '']
        if die.endswith('.std'):
            result_die[title][0] = die
        else:
            result_die[title][1] = die
    not_pass = []
    for die in result_die:
        std_bit: bytes
        out_bit: bytes
        # 将这些从文件中读取出来，转换为bytes
        with open(os.path.join(result_dir, result_die[die][0]), 'rb+') as fp:
            std_bit = fp.read()
        with open(os.path.join(result_dir, result_die[die][1]), 'rb+') as fp:
            out_bit = fp.read()
        # 比较两个bytes对象
        if not out_bit.__eq__(std_bit):
            not_pass.append(die)

    print("===============================================")
    print("Total: ", len(result_die))
    print("===============================================")
    if len(not_pass) == 0:
        print("Result: all pass")
    else:

        print("NotPassCount: ", len(not_pass))
        i = 1
        for die in not_pass:
            # 输出对比Html文件
            t1 = readFile(os.path.join(result_dir, result_die[die][0]))
            t2 = readFile(os.path.join(result_dir, result_die[die][1]))
            if os.path.isdir("./diff_result"):
                shutil.rmtree("./diff_result")
            os.mkdir("./diff_result")
            diff = difflib.HtmlDiff()
            res = diff.make_file(t1, t2, "标准文件", "输出文件", context=True)
            writeFile(f"./diff_result/{die}.html", res)

            print(i, ': ', die)
            i+=1
    print("===============================================")


if __name__ == "__main__":
    check(sys.argv[1])