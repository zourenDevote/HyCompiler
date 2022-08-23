//
// Created by shadows on 22-7-6.
//
#include "basic.h"

using namespace std;

namespace ASM {
    int Inst::inst_order = 0;

    void Inst::dumpInstruction(ostream &out) {
        switch (_op) {
            case Add:
                if (_result != _arg1 || _arg2 != "#0")
                    out << "\tadd " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Sub:
                if (_result != _arg1 || _arg2 != "#0")
                    out << "\tsub " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Subs:
                out << "\tsubs " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Rsb:
                out << "\trsb " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Mul:
                out << "\tmul " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Div:
                out << "\tsdiv " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Asr:
                out << "\tmov " << _result << ", " << _arg1 << ", asr #" << _arg2 << endl;
                break;
            case Lsl:
                out << "\tmov " << _result << ", " << _arg1 << ", lsl #" << _arg2 << endl;
                break;
            case Vadd:
                out << "\tvadd.f32 " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Vsub:
                out << "\tvsub.f32 " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Vcmp:
                out << "\tvcmpe.f32 " << _arg1 << ", " << _arg2 << endl;
                out << "\tvmrs	APSR_nzcv, FPSCR" << endl;
                break;
            case Vrsb:
                out << "\tvrsb.f32 " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Vmul:
                out << "\tvmul.f32 " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Vdiv:
                out << "\tvdiv.f32 " << _result << ", " << _arg1 << ", " << _arg2 << endl;
                break;
            case Itof:
                out << "\tvcvt.f32.s32 " << _result << ", " << _arg1 << endl;
                break;
            case Ftoi:
                out << "\tvcvt.s32.f32 " << _result << ", " << _arg1 << endl;
                break;
            case Ldr:
                if (_arg2 == "") {
                    out << "\tldr " << _result << ", [" << _arg1 << "]" << endl;
                } else {
                    out << "\tldr " << _result << ", [" << _arg1 << ", " << _arg2 << "]" << endl;
                }
                break;
            case Str:
                if (_arg2 == "") {
                    out << "\tstr " << _result << ", [" << _arg1 << "]" << endl;
                } else {
                    out << "\tstr " << _result << ", [" << _arg1 << ", " << _arg2 << "]" << endl;
                }
                break;
            case Strd:
                if (_arg2 == "") {
                    out << "\tstrd " << _result << ", [" << _arg1 << "]" << endl;
                } else {
                    out << "\tstrd " << _result << ", [" << _arg1 << ", " << _arg2 << "]" << endl;
                }
                break;
            case Vldr:
                if (_arg2 == "") {
                    out << "\tvldr.32 " << _result << ", [" << _arg1 << "]" << endl;
                } else {
                    out << "\tvldr.32 " << _result << ", [" << _arg1 << ", " << _arg2 << "]" << endl;
                }
                break;
            case Vstr:
                if (_arg2 == "") {
                    out << "\tvstr.32 " << _result << ", [" << _arg1 << "]" << endl;
                } else {
                    out << "\tvstr.32 " << _result << ", [" << _arg1 << ", " << _arg2 << "]" << endl;
                }
                break;
            case Mov:
                if (_result != _arg1)
                    out << "\tmov " << _result << ", " << _arg1 << endl;
                break;
            case Movlt:
                out << "\tmovlt " << _result << ", " << _arg1 << endl;
                break;
            case Movle:
                out << "\tmovle " << _result << ", " << _arg1 << endl;
                break;
            case Movgt:
                out << "\tmovgt " << _result << ", " << _arg1 << endl;
                break;
            case Movge:
                out << "\tmovge " << _result << ", " << _arg1 << endl;
                break;
            case Moveq:
                out << "\tmoveq " << _result << ", " << _arg1 << endl;
                break;
            case Movne:
                out << "\tmovne " << _result << ", " << _arg1 << endl;
                break;
            case Movwt:
                out << "\tmovw " << _result << ", #" << _arg1 << endl;
                if (_arg2 != "0")
                    out << "\tmovt " << _result << ", " << _arg2 << endl;
                break;
            case Vmov:
                if (_result != _arg1)
                    out << "\tvmov " << _result << ", " << _arg1 << endl;
                break;
            case Push:
                out << "\tpush " << "{" << _result << "}" << endl;
                break;
            case Pop:
                out << "\tpop " << "{" << _result << "}" << endl;
                break;
            case Cmp:
                out << "\tcmp " << _arg1 << ", " << _arg2 << endl;
                break;
            case Blt:
                out << "\tblt " << "." << _result << endl;
                break;
            case Ble:
                out << "\tble " << "." << _result << endl;
                break;
            case Bgt:
                out << "\tbgt " << "." << _result << endl;
                break;
            case Bge:
                out << "\tbge " << "." << _result << endl;
                break;
            case Beq:
                out << "\tbeq " << "." << _result << endl;
                break;
            case Bne:
                out << "\tbne " << "." << _result << endl;
                break;
            case B:
                out << "\tb " << "." << _result << endl;
                break;
            case Bl:
                out << "\tbl " << _result << endl;
                break;
            case Bx:
                out << "\tbx " << _result << endl;
                break;
            case Label:
                out << "." << _result << ":" << endl;
                break;
            case Mls:
                out << "\tmls " << _result << "," << _arg1 << "," << _arg2 << "," << _arg3 << endl;
                break;
            case Ite:
                out << "\tite " << _result << endl;
                break;
            default:
                break;
        }
    }

}
