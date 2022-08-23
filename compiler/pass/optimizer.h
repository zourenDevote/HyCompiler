//
// Created by devotes on 22-7-11.
//

#ifndef COMPILER_OPTIMIZER_H
#define COMPILER_OPTIMIZER_H

#include "mem2reg_pass.h"
#include "initial_pass.h"
#include "phi_translate_pass.h"
#include "dead_code_elimination_pass.h"
#include "interval_pass.h"
#include "reduce_ana_pass.h"
#include "const_fold_pass.h"
#include "branch_remove_pass.h"
#include "bbmerge_pass.h"
#include "part_constant_sperate_pass.h"
#include "gvn_pass.h"
#include "load_adder_pass.h"

#endif //COMPILER_OPTIMIZER_H
