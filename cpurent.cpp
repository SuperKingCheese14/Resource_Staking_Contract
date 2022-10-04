#include "darkcpu.hpp"
#include "eosio.token.hpp"
#include "eosio.system.hpp"
#include <cctype>

ACTION cpurent::logstake(name from, time_point_sec unstakeTime, asset stake_value) {
    require_auth(get_self());
    staking_table data(get_self(), get_self().value);
    data.emplace(get_self(), [&](auto& msg) {
        msg.key = data.available_primary_key();
        msg.user = from;
        msg.unstakeTime = unstakeTime;
        msg.waxstake = stake_value;
    }); 
}

ACTION cpurent::getunstake(){
    require_auth(get_self());
    symbol token_symbol("WAX", 8);
    staking_table data(get_self(), get_self().value);
    vector<uint64_t> keysForDeletion;
    for(auto& item : data) {
        if (item.unstakeTime <= current_time_point()) {
         keysForDeletion.push_back(item.key);   
        }
    }
    check(keysForDeletion.size() > 0,"no one have to unstake");
    for (uint64_t key : keysForDeletion) {
        eosio::print("remove from _polls ", key);
        auto itr = data.find(key);
        if (itr != data.end()) {
            action(
            permission_level(get_self(),"active"_n), 
            "eosio"_n, "undelegatebw"_n,
            undelegatebw_args{
                get_self(),
                itr->user,
                asset(0, token_symbol),
                itr->waxstake}
            ).send();
            data.erase(itr);
        }
    }
}
void cpurent::deposit(name from, name to, asset quantity, const string &memo){
    auto self = get_self();
    
    symbol token_symbol("DTX", 5);
    symbol token_symbol2("WAX", 8);
    
    uint64_t day1 = 1;
    uint64_t day2 = 2;
    uint64_t day3 = 3;
    char* end;
        
    uint64_t stake_value1 = quantity.amount * 30000 / day1;
    uint64_t stake_value2 = quantity.amount * 30000 / day2;
    uint64_t stake_value3 = quantity.amount * 30000 / day3;
    uint64_t unstakeSeconds1 = day1 * 86400;
    uint64_t unstakeSeconds2 = day2 * 86400;
    uint64_t unstakeSeconds3 = day3 * 86400;    
    
    if (to == self){
      if(memo.size()){
        check(std::all_of(memo.begin(), memo.end(), ::isdigit),"memo must contains only number");
        auto getDay = stoi(memo);
        print(getDay);
        check(getDay >= 1,"Days must be higher than or equal to 1");
      }
      check(quantity.amount > 0, "Must transfer positive amount");
      check(quantity.symbol == token_symbol, "Incorrect asset symbol");
      auto getDay = stoi(memo);     
      if (memo == "1"){     
        const asset balance = eosio::token::get_balance("eosio.token"_n, self, token_symbol2.code());
        check(balance.amount >= stake_value1, "No liquid WAX available");
        auto nowWithSeconds = time_point_sec(current_time_point());
        time_point_sec unstakeTime = time_point_sec(nowWithSeconds + unstakeSeconds1);                
        action(
            permission_level(self,"active"_n),
            get_self(),
            "logstake"_n,
            make_tuple(from,unstakeTime,asset(stake_value1, token_symbol2))
        ).send();
        action(
            permission_level(self,"active"_n), 
            "eosio"_n, "delegatebw"_n,
            delegatebw_args{
                self,
                from,
                asset(0, token_symbol2),
                asset(stake_value1, token_symbol2),
                false}
          ).send();
      }
      else if (memo == "2"){      
        const asset balance = eosio::token::get_balance("eosio.token"_n, self, token_symbol2.code());
        check(balance.amount >= stake_value2, "No liquid WAX available");        
        auto nowWithSeconds = time_point_sec(current_time_point());
        time_point_sec unstakeTime = time_point_sec(nowWithSeconds + unstakeSeconds2);
        action(
            permission_level(self,"active"_n),
            get_self(),
            "logstake"_n,
            make_tuple(from,unstakeTime,asset(stake_value2, token_symbol2))
        ).send();
        action(
            permission_level(self,"active"_n), 
            "eosio"_n, "delegatebw"_n,
            delegatebw_args{
                self,
                from,
                asset(0, token_symbol2),
                asset(stake_value2, token_symbol2),
                false}
          ).send();
      }
      else if (memo == "3"){      
        const asset balance = eosio::token::get_balance("eosio.token"_n, self, token_symbol.code());
        check(balance.amount >= stake_value3, "No liquid WAX available");            
        auto nowWithSeconds = time_point_sec(current_time_point());
        time_point_sec unstakeTime = time_point_sec(nowWithSeconds + unstakeSeconds3);
        action(
            permission_level(self,"active"_n),
            get_self(),
            "logstake"_n,
            make_tuple(from,unstakeTime,asset(stake_value3, token_symbol2))
        ).send();
        action(
            permission_level(self,"active"_n), 
            "eosio"_n, "delegatebw"_n,
            delegatebw_args{
                self,
                from,
                asset(0, token_symbol2),
                asset(stake_value3, token_symbol2),
                false}
          ).send();
      }        
    }
}
