# -*- coding=utf-8 -*-

"""
Author:   Devotes
Date:     2022/5/15

FixLog:

"""

import os
import shutil

file_name = {

}

ANTLR4:          str = "./antlr-4.8-complete.jar"
output:          str = "./temp/"
srcfile:         str = "./grammarrule/Parser.g4"
dest_dir:        str = "./temp/grammarrule"

generate_dir:    str = "./compiler/antlr_generate"
generate_option: str = "-visitor"


antlr_runtime = {
    "build":                "./build",
    "run":                  "./run",
    "include":              "./run/usr/local/include/antlr4-runtime",
    "lib":                  "./run/usr/local/lib",
}


if __name__ == "__main__":
    print("Deploy Script...")

    print("=================================")
    print("Deploy g4 file to cpp file....")
    if not os.path.isdir("./temp"):
        os.mkdir("./temp")
    if not os.path.isdir(dest_dir):
        os.mkdir(dest_dir)

    # generate parser file
    v = os.system(f"java -jar {ANTLR4} {srcfile} {generate_option} -o {output}")

    file_list = os.listdir(dest_dir)
    file: str

    src_path = ""

    for file in file_list:
        src_path = os.path.join(dest_dir, file)

        if file.endswith('.h'):
            if os.path.exists(os.path.join(generate_dir, file)):
                os.remove(os.path.join(generate_dir, file))
            shutil.move(src_path, generate_dir)
        elif file.endswith('.cpp'):
            if os.path.exists(os.path.join(generate_dir, file)):
                os.remove(os.path.join(generate_dir, file))
            shutil.move(src_path, generate_dir)

    if os.path.isdir("./temp"):
        shutil.rmtree("./temp")

    print("Deploy success!")

    print("=================================")
    print("Deploy antlr runtime support....")
    # deploy antlr runtime
    pwd = os.getcwd()
    os.chdir("./third_party/antlr4")
    if not os.path.isdir("./build"):
        os.mkdir("build")
    if not os.path.isdir("./run"):
        os.mkdir("run")

    v = os.system("cd ./build; cmake ../; make -j10; DESTDIR=../run make install")

    if v != 0:
        raise "Deploy error"

    if os.path.isdir(os.path.join(pwd, "compiler/antlr4-runtime")):
        shutil.rmtree(os.path.join(pwd, "compiler/antlr4-runtime"))

    os.system(f"mv {antlr_runtime['include']} {os.path.join(pwd, 'compiler')}")

    if os.path.isdir(os.path.join(pwd, "compiler/lib")):
        shutil.rmtree(os.path.join(pwd, "compiler/lib"))
    else:
        os.mkdir(os.path.join(pwd, "compiler/lib"))

    os.system(f"mv {antlr_runtime['lib']} {pwd}/compiler")
    os.chdir(pwd)
    print("Deploy success!")


