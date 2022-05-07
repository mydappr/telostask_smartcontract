class common_controller{
  public:
    common_controller(name self){
      s = self;
    }

    void createTask(name sender,std::string data,asset quantity){
      //creattask:id,taskreward
      auto fbreak = data.find(",");
      auto id = std::stoull(data.substr(0,fbreak));
      auto reward = asset(std::stoull(data.substr(fbreak+1)),symbol_task);

      tasks_table tasks(s,s.value);
      auto tskItr = tasks.find(id);
      check(tskItr==tasks.end(),"This id already exist");

      amounts_table amounts(s,s.value);
      auto amItr = amounts.find(name("taskfee").value);
      check(amItr != amounts.end(),"taskfee amount not found");
      check(quantity >= amItr->amount, "quantity must be greater than taskfee");

      tasks.emplace(s,[&](auto &_){
        _.id = id;
        _.taskowner = sender;
        _.reward = reward;
        _.reward_pool = quantity - amItr->amount;
        _.start_date = now();
        _.status = 0;
      });

      //find amount should be burnt
      auto brnItr = amounts.find(name("taskfeeburn").value);
      transferTASK(s,banAccount,brnItr->amount,"burnt for task "+std::to_string(id));
    }

  private:
    name s;
};
