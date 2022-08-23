//
// Created by devotes on 22-7-11.
//

#include "pass.h"

namespace opt_pass{

    void PassManager::registerPassWork(const std::shared_ptr<Pass>& pass, int work_style) {
        switch (work_style) {
            case 1:{
                before_que.push_back(pass);
                break;
            }
            case 2:{
                work_que.push_back(pass);
                break;
            }
            default:
                after_que.push_back(pass);
                break;
        }
    }

    void PassManager::seqExecutePass(IR::MODULE& m) {
        for(const auto& pass : before_que)
            pass->execute(m);
        bool isChange = true;
        while(isChange){
            isChange = false;
            for(const auto& pass : work_que){
                pass->execute(m);
                isChange = (pass->isChange?pass->isChange:isChange);
            }
        }
        for(const auto& pass : after_que){
            pass->execute(m);
        }
    }

}

