#Staking Contract for WAX blockchain system resources.

This smart contract is designed for projects and communities who have their own token and wish to offer a staking service to their members.

The contract will accept deposits in the currency specified in cpurent.cpp line 48 and will then delegate CPU to the requester. 
Before the contract delegates WAX for CPU it will check the account balance to make sure there is a sufficient balance and will then ddelegate CPU for either 1, 2 or 3 days.
When users deposit tokens to the contract they must specify the number of days they would like to rent CPU for as the memo for the transaction. The contract will only accept deposits with a memo of "1", "2" or "3".
