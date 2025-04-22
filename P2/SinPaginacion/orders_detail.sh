#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "2\r"

expect "Enter a number that corresponds to your choice > "
send -- "3\r"

expect "Enter ordernumber > "
send -- "10100\r"

# 7933 -> stock for product with id = S10_1678
expect "S24_3969 49 35.29"
expect "S18_2248 50 55.09"
expect "S18_1749 30 136.00"
expect "S18_4409 22 75.46"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
