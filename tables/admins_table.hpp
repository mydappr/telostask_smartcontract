struct [[eosio::table, eosio::contract("telostask")]] admins{
    name name;
    //1 admin
    //2 viewer
    uint8_t role;
    bool is_active;
    uint64_t primary_key() const { return name.value; }
};
typedef multi_index<"admins"_n, admins> admins_table;
