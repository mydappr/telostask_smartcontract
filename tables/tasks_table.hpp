struct [[eosio::table, eosio::contract("telostask")]] tasks{
  uint64_t id;
  name taskowner;
  asset reward_pool;
  asset reward;
  uint64_t start_date;
  //0 pending for approve
  //1 approved
  //2 disapproved
  //3 inactived
  //4 finished
  uint8_t status;
  uint64_t primary_key() const { return id; }
};
typedef multi_index<"tasks"_n, tasks> tasks_table;