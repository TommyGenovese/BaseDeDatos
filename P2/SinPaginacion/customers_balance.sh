#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "3\r"

expect "Enter a number that corresponds to your choice > "
send -- "3\r"

expect "Enter customer number > "
send -- "141\r"

# 7933 -> stock for product with id = S10_1678
expect "Balance = -104950.56"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
