/*
 * Author:   Devotes
 * Date:     2022/5/15
 *
 * FixLog:
 *
 *
*/

grammar Parser;

import Token;

options{language = Cpp;}

@parser::header{
    #include <vector>
    #include <utility>
}

/// context free grammar

// compile unit grammar
comp_unit[] returns [std::vector<std::pair<int, DeclContext*>> decl_vec, std::vector<std::pair<int, Func_defContext*>> func_def_vec]
        locals[int i = 0]
        @init{
            $decl_vec = std::vector<std::pair<int, DeclContext*>>();
            $func_def_vec = std::vector<std::pair<int, Func_defContext*>>();
        }
        :
        (
         decl           {$decl_vec.push_back({$i, $decl.ctx}); $i++;}
        |func_def       {$func_def_vec.push_back({$i, $func_def.ctx}); $i++;}
        )* EOF
        ;

// decl unit grammar
decl:           const_decl | var_decl;

// const variable declare grammar
const_decl:     Const btype const_def  (',' const_def)* ';';

// type declare grammar
btype:          Int | Float;

// const define grammar
const_def:      Id (Lsb const_exp Rsb)* Assign const_init_val;

// const initialized variable grammar, there have a sequence can not check.
const_init_val:   const_exp
                | '{' (const_init_val (',' const_init_val)* )? '}';

// variable declare garmmar
var_decl:        btype var_def (',' var_def)* ';';

// variable define grammar
var_def:          Id (Lsb const_exp Rsb)*
                | Id ( Lsb const_exp Rsb )* Assign init_val;

// initialized variable grammar
init_val:         exp
                | '{' (init_val (',' init_val)* )? '}';

// function define grammar
func_def:       func_type Id SLp (func_f_params)? SRp block;

// function type define grammar
func_type:      Void | Int | Float;

// function list or parameter grammar
func_f_params:  func_f_param (',' func_f_param)*;

// function param grammar
func_f_param:   btype Id (Lsb Rsb (Lsb const_exp Rsb)* )?;

// block grammar
block returns [std::vector<std::pair<int, DeclContext*>> decl_vec, std::vector<std::pair<int, StmtContext*>> stmt_vec]
        locals[int i = 0]
        :
        '{'
        (
          decl      {$decl_vec.push_back({$i, $decl.ctx}); $i++;}
         |stmt      {$stmt_vec.push_back({$i, $stmt.ctx}); $i++;}
         )*
         '}'
         ;

// statement grammar
stmt:           l_val Assign exp ';'
                | (add_exp)? ';'
                | block
                | While SLp l_or_exp SRp stmt
                | If SLp l_or_exp SRp stmt (Else stmt)?
                | Break ';'
                | Continue ';'
                | Return (exp)? ';'
                ;

// expression grammar
exp:            add_exp;

l_val:          Id (Lsb exp Rsb)*;

primary_exp:    SLp exp SRp
                | l_val
                | (IntConst | FloatConst);

unary_exp:        unary_op* primary_exp
                | unary_op* Id SLp (func_r_params)? SRp
                ;

unary_op:       Add | Sub | Not;

func_r_params:  exp (',' exp)*;

mul_exp:         unary_exp (sem_op unary_exp)*
                ;

sem_op:          Mul | Div | Mod
                ;

add_exp:         mul_exp (op_as mul_exp)*
                ;

op_as:           Add | Sub
                ;

rel_exp:         add_exp (com_op add_exp)*
                ;

com_op:          Gt | Lt | Le | Ge
                ;

eq_exp:          rel_exp (com_op_eqm rel_exp)*
                ;

com_op_eqm:      Equal | Nequal
                ;

l_and_exp:       eq_exp (And eq_exp)*
                ;

l_or_exp:        l_and_exp (Or l_and_exp)*
                ;

const_exp:      add_exp;
