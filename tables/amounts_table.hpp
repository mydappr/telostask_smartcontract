struct [[eosio::table, eosio::contract("telostask")]] amounts{
    name name;
    asset amount;
    uint64_t primary_key() const { return name.value; }
};
typedef multi_index<"amounts"_n, amounts> amounts_table;
