#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "3\r"

expect "Enter a number that corresponds to your choice > "
send -- "1\r"

expect "Enter customer name > "
send -- "Mary\r"

# 7933 -> stock for product with id = S10_1678
expect "146 Saveley & Henriot, Co. Mary  Saveley"
expect "219 Boards & Toys Co. Mary Young"


puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
