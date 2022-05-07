struct [[eosio::table, eosio::contract("telostask")]] values{
    name name;
    uint64_t value;
    uint64_t primary_key() const { return name.value; }
};
typedef multi_index<"values"_n, values> values_table;
