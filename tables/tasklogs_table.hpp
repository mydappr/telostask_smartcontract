//scope : task id
struct [[eosio::table, eosio::contract("telostask")]] tasklogs{
  name tasktaker;
  uint64_t date;
  uint64_t primary_key() const { return tasktaker.value; }
};
typedef multi_index<"tasklogs"_n, tasklogs> tasklogs_table;