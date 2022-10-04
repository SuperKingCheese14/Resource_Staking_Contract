#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <string>
#include <cstdlib> 
using namespace std;
using namespace eosio;
CONTRACT cpurent : public contract {
  public:
    using contract::contract;        
    ACTION logstake(name from, time_point_sec unstakeTime, asset stake_value);
    
    ACTION getunstake();    
    
    [[eosio::on_notify("victoria1212::transfer")]] 
    void deposit(name from, name to, asset quantity, const std::string &memo);
    
    using transfer_action = action_wrapper<name("transfer"), &darkcpu::deposit>;
    
    struct delegatebw_args
    {
        name from;
        name receiver;
        asset stake_net_quantity;
        asset stake_cpu_quantity;
        bool transfer;
    };
    struct undelegatebw_args
    {
        name from;
        name receiver;
        asset unstake_net_quantity;
        asset unstake_cpu_quantity;
    };    
        
  private:  
    TABLE logstaking {
      uint64_t    key;
      name    user;
      time_point_sec  unstakeTime;
      asset waxstake;
      auto primary_key() const { return key; }
    };
    
    typedef multi_index<name("logstaking"), logstaking> staking_table;
};
