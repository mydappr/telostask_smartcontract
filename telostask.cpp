
#include "telostask.hpp"


//Admin actions
void telostask::setamount(name admin,name asset_name,asset amount){
  _admin_controller.setAmount(admin,asset_name,amount);
}

void telostask::setvalue(name admin,name value_name,uint64_t value){
  _admin_controller.setValue(admin,value_name,value);
}

void telostask::addadmin(name admin_name,uint8_t role){
  _admin_controller.addAdmin(admin_name,role);
}

void telostask::setadminact(name admin_name,bool act){
  _admin_controller.setAdminAct(admin_name,act);
}

void telostask::approvetsk(name admin,uint64_t task_id){
  _admin_controller.approveTask(admin,task_id);
}

void telostask::disapprovtsk(name admin,uint64_t task_id,string memo){
  _admin_controller.disapproveTask(admin,task_id,memo);
}

void telostask::approvedtsk(name admin,uint64_t task_id,name tasktaker){
  _admin_controller.disapproveDoneTask(admin,task_id,tasktaker);
}

//Transfer action
void telostask::transfer(name sender, name receiver){
  auto transfer_data = unpack_action_data<st_transfer>();
  if (transfer_data.from == _self ){
      return;
  }
  check( transfer_data.quantity.is_valid(), "Invalid asset");
  const uint64_t amount = (uint64_t)transfer_data.quantity.amount;
  if(transfer_data.memo == "deposit")
    return;
  check(transfer_data.memo.length()!=0,"memo is needed");

  const size_t first_break = transfer_data.memo.find(":");
  auto action = transfer_data.memo.substr(0, first_break);
  auto data = transfer_data.memo.substr(first_break + 1);

  if(action=="createtask"){
    _common_controller.createTask(sender,data,transfer_data.quantity);
  }else{
    check(false,"Invalid action");
  }
}

extern "C" {
  void apply(name receiver, name code, uint64_t action) {
    auto self = receiver;
    if( code == self || code == "eosio.token"_n || code == tokenAccount) { 
      if( action == "transfer"_n.value){ 
        check( code == "eosio.token"_n || code == tokenAccount , "Must transfer TLOS or DRIC"); 
      }
      //  TYPE thiscontract( self ); 
      switch( action ) { 
        EOSIO_DISPATCH_HELPER(
          telostask, 
          (transfer)
          //admins
          (setamount)
          (setvalue)
          (setadminact)
          (addadmin)
          (approvetsk)
          (disapprovtsk)
        )
      }
    } 
  }
};