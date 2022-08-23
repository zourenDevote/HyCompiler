/**
* Author:   Devotes
* Date:     2022/5/31
*
* FixLog:
*   支配者树
*/

#include "ast_visitor.h"
#include "antlr_generate/ParserLexer.h"
#include "antlr_generate/ParserParser.h"
#include "ast/ast_creator.h"
#include "semantic/semantic_check.h"
#include "ir/ir_generator.h"
#include "pass/optimizer.h"
#include "asm/codegen.h"
#include "cxxopts.hpp"
#include <iostream>
#include "utility/hy_utility.h"
#include "semantic/dag_ast_builder.h"
#include "semantic/pre_ir_analysis.h"
#include "preopti/func_inline.h"
#include "preopti/inst_combine.h"
#include "preopti/func_execute_analysis.h"

using namespace std;

/**
 * Compiler 主函数入口
 * 选项
 *   -S         汇编
 *   --emit-ast 输出ast
 *   --emit-ir  输出IR
 *   -O2        开启优化选项
 *   -o         输出位置
 */
int main(int argc, char *argv[]) {

    // 开始设置选项
    cxxopts::Options options(argv[0], "This is a compiler of Sy2022 Language");
    options.positional_help("[optional args]")
            .show_positional_help();

    // 设置命令行解析器属性
    options.set_width(70)
            .set_tab_expansion()
            .allow_unrecognised_options()
            .add_options()
                    ("S,ASM", "out asm code", cxxopts::value<bool>()->default_value("true"))
                    ("emit-ast", "out the ast", cxxopts::value<bool>()->default_value("false"))
                    ("emit-ir", "out the IR", cxxopts::value<bool>()->default_value("false"))
                    ("o, output", "the output file", cxxopts::value<string>()->default_value("testcase.s"))
                    ("std", "standard out stream", cxxopts::value<string>()->default_value("stdout"))
                    ("i,input", "the input file", cxxopts::value<string>()->default_value(""))
                    ("h,help", "print help")
                    ("O,O2", "open pass", cxxopts::value<bool>()->default_value("false"));

    options.parse_positional({"input", "O2"});

    // 获得解析结果
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        cout << options.help({""}) << endl;
        exit(0);
    }

    string output = result["o"].as<string>();
    string in = result["input"].as<string>();

    antlr4::ANTLRFileStream input(in);

    // 从文件中加载字符流
    input.loadFromFile(in);

    // 词法分析
    ParserLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    // 语法分析
    ParserParser parser(&tokens);

    auto comp_unit = parser.comp_unit();

    if (parser.getNumberOfSyntaxErrors()) {
        cerr << "Error! parser error! program exit with code -1" << endl;
        exit(-1);
    }

    // AST构造
    auto unit = NewAstCreator().visitComp_unit(comp_unit).as<std::shared_ptr<CompUnitNode>>();

    if (result["O2"].as<bool>()) {
        {
            FuncInline funcinline;
            funcinline.visit(unit);
        }
        // 内联之后再检查一次
//        InstCombine combine;
//        combine.visit(unit);
        {
            FuncExecuteAnalysis exeAna;
            exeAna.visit(unit);
        }

        {

        }
    }


    // 语义分析
    SemanticChecker checker;
    checker.visit(unit);

//    DagAstBuilder dagBuilder;
//    dagBuilder.visit(unit);

    // IR -> AST 优化分析
    PreIrGenAnalysis preAna;
    preAna.visit(unit);

    if (result.count("emit-ast")) {
        stringstream ss;
        unit->toAstInfoString(ss, 0);
        if (result["std"].as<string>() == "stdout") {
            cout << ss.str() << endl;
        } else {
            ofstream f;
            f.open(result["std"].as<string>());
            f << ss.str() << endl;
            f.close();
        }
        exit(0);
    }

    if (!checker.hasCheckError()) {
        IR::MODULE m = make_shared<IR::Module>("Test");
        IR::IRGenerate gen(m);
        gen.visit(unit);

        opt_pass::PassManager manager;

        bool open_backend_opt= false;
        if (result["O2"].as<bool>()) {
            open_backend_opt= true;
//            auto gvnPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::GvnPass>());
//            manager.registerPassWork(gvnPass);
            auto initPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::InitPass>());
            manager.registerPassWork(initPass, 2);
//
//            auto mem2regPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::Mem2RegPass>());
//            manager.registerPassWork(mem2regPass);

            auto constFoldPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::PartConstFoldPass>());
            manager.registerPassWork(constFoldPass, 2);

            auto constSpreatePass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::PartConstantSperatePass>());
            manager.registerPassWork(constSpreatePass, 2);

            manager.registerPassWork(constFoldPass, 2);

            auto branchRemovePass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::BranchRemovePass>());
            manager.registerPassWork(branchRemovePass, 2);

            auto deadEmitPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::DeadCodeElimination>());
            manager.registerPassWork(deadEmitPass, 2);

            auto loadCountPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::LoadAdderPass>());
            manager.registerPassWork(loadCountPass, 3);

//            auto bbMerge = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::BBMergePass>());
//            manager.registerPassWork(bbMerge, 3);

//            auto constFoldPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::PartConstFoldPass>());

        }

//        auto intervalPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::IntervalPass>());
//        manager.registerPassWork(intervalPass);

        auto reduceNextPass = dynamic_pointer_cast<opt_pass::Pass>(make_shared<opt_pass::ReduceAnaPass>());
        manager.registerPassWork(reduceNextPass);

        manager.seqExecutePass(m);

        if (result.count("emit-ir")) {
            if (result["std"].as<string>() == "stdout") {
                m->dump(cout);
            } else {
                ofstream f;
                f.open(result["std"].as<string>());
                m->dump(f);
                f.close();
            }
            exit(0);
        }

        string outS = result["output"].as<string>();
        if (!endsWith(outS, ".s")) {
            outS.append(".s");
        }

        CodeGen codegen(outS);
        if(open_backend_opt)
            codegen.openOpt();
        codegen.emitModule(m);

    } else {
        cerr << "Error! semantic check error! program exit with code -1" << endl;
        exit(-1);
    }
}

