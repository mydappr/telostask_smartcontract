#include <string>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/fixed_bytes.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;
using namespace std;

using eosio::indexed_by;
using eosio::const_mem_fun;
using eosio::asset;
using eosio::permission_level;
using eosio::action;
using eosio::name;
using eosio::contract;

using std::string;
using std::vector;
using std::map;

const auto symbol_tlos = symbol(symbol_code("TLOS"),4);
const auto symbol_task = symbol(symbol_code("TASK"),4);
uint32_t now(){return current_time_point().sec_since_epoch();}


const name tokenAccount = "telostaskapp"_n;
const name banAccount = "telostaskap1"_n;

void transferTASK(name from,name to, asset quantity, string memo){
  eosio::action(
      permission_level{ from , "active"_n },
      tokenAccount, "transfer"_n,
      std::make_tuple(from, to, quantity, memo)
  ).send();
}

#include "tables/amounts_table.hpp"
#include "tables/values_table.hpp"
#include "tables/admins_table.hpp"
#include "tables/tasks_table.hpp"
#include "tables/tasklogs_table.hpp"
#include "controllers/admin_controller.hpp"
#include "controllers/common_controller.hpp"

class [[eosio::contract("telostask")]] telostask : public contract {
  public:
    using contract::contract;
    telostask(name receiver, name code, datastream<const char*> ds):
    contract(receiver, code, ds),
    _common_controller(get_self()),
    _admin_controller(get_self()){
    }

  //admin actions
  ACTION setvalue(name admin,name value_name,uint64_t value);
  ACTION setamount(name admin,name asset_name,asset amount);
  ACTION addadmin(name admin_name,uint8_t role);
  ACTION setadminact(name admin_name,bool act);
  ACTION approvetsk(name admin,uint64_t task_id);
  ACTION approvedtsk(name admin,uint64_t task_id,name tasktaker);
  ACTION disapprovtsk(name admin,uint64_t task_id,string memo);

  void transfer(name sender, name receiver);

  //common
  private:
		struct st_transfer {
      name          from;
      name          to;
      asset         quantity;
      std::string   memo;
    };

    //controllers
    admin_controller _admin_controller;
    common_controller _common_controller;
};