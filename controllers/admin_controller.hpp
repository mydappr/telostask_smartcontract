class admin_controller{
  public:
    admin_controller(name self){
      s = self;
    }

    void setAmount(name admin,name amount_name,asset amount){
      auto role = _getAdminRole(admin);
      if(role>=2)
        check(false,"Invalid role");
      amounts_table amounts(s,s.value);
      auto itr = amounts.find(amount_name.value);
      if(itr == amounts.end()){
        amounts.emplace(s,[&](auto &_){
          _.name = amount_name;
          _.amount = amount;
        });
      }else{
        amounts.modify(itr,s,[&](auto &_){
          _.amount = amount;
        });
      }
    }

    void setValue(name admin,name value_name,uint64_t value){
      auto role = _getAdminRole(admin);
      if(role>=2)
        check(false,"Invalid role");
      values_table values(s,s.value);
      auto itr = values.find(value_name.value);
      if(itr == values.end()){
        values.emplace(s,[&](auto &_){
          _.name = value_name;
          _.value = value;
        });
      }else{
        values.modify(itr,s,[&](auto &_){
          _.value = value;
        });
      }      
    }

    void addAdmin(name admin_name,uint8_t role){
      require_auth(s);
      
      admins_table admins(s,s.value);
      auto itr = admins.find(admin_name.value);
      check(itr == admins.end(),"admin exist");
      admins.emplace(s,[&](auto &_){
        _.name = admin_name;
        _.role = role;
        _.is_active = true;
      });
    }

    void setAdminAct(name admin_name,bool act){
      auto role = _getAdminRole(admin_name);
      if(role>=2)
        check(false,"Invalid role");

      admins_table admins(s,s.value);
      auto itr = admins.find(admin_name.value);
      check(itr != admins.end(),"admin not found");
      admins.modify(itr,s,[&](auto &_){
        _.is_active = act;
      });
    }

    void approveTask(name admin,uint64_t task_id){
      //all admin can approve
      auto role = _getAdminRole(admin);
      
      tasks_table tasks(s,s.value);
      auto tskitr = tasks.find(task_id);
      check(tskitr != tasks.end(),"Task not found");
      tasks.modify(tskitr,s,[&](auto &_){
        _.status = 1;
        _.start_date = now();
      });
    }

    void disapproveTask(name admin,uint64_t task_id,string memo){
      //all admin can disapprove
      auto role = _getAdminRole(admin);

      tasks_table tasks(s,s.value);
      auto tskitr = tasks.find(task_id);
      check(tskitr != tasks.end(),"Task not found");
      check(tskitr->status == 0,"This task is approved");

      transferTASK(s,tskitr->taskowner,tskitr->reward_pool,memo);

      tasks.modify(tskitr,s,[&](auto &_){
        _.status = 2;
        _.start_date = now();
        _.reward_pool = asset(0,symbol_task);
      });
    }

    void disapproveDoneTask(name admin,uint64_t task_id,name tasktaker){
      //all admin can approve
      auto role = _getAdminRole(admin);

      tasks_table tasks(s,s.value);
      auto tskitr = tasks.find(task_id);
      check(tskitr != tasks.end(),"Task not found");
      check(tskitr->status != 0,"Invalid status");

      tasklogs_table tasklogs(s,task_id);
      auto logitr = tasklogs.find(tasktaker.value);
      check(logitr == tasklogs.end(),"task taker already exist");
      tasklogs.emplace(s,[&](auto &_){
        _.tasktaker = tasktaker;
        _.date = now();
      });

      transferTASK(s,tasktaker,tskitr->reward_pool < tskitr->reward ? tskitr->reward : tskitr->reward_pool,"Task reward, task id: "+std::to_string(task_id));

      tasks.modify(tskitr,s,[&](auto &_){
        if(tskitr->reward_pool < tskitr->reward){
          _.status = 4;
          _.reward_pool = asset(0,symbol_task);
        }else{
          _.reward_pool = tskitr->reward_pool - tskitr->reward;
        }
      });
    }

  private:
    name s;
    uint8_t _getAdminRole(name account){
      if(!has_auth(s)){
        admins_table admins(s,s.value);
        auto admItr = admins.find(account.value);
        check(admItr != admins.end(),"Admin not found");
        check(admItr->is_active, "Admin is in active");
        return admItr->role;
      }
      return 0;
    }
};