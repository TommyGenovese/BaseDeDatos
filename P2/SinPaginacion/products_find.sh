#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "1\r"

expect "Enter a number that corresponds to your choice > "
send -- "2\r"

expect "Enter productname > "
send -- "Harley\r"

# 7933 -> stock for product with id = S10_1678
expect "S10_1678"
expect "S10_4698"
expect "S18_2625"
puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "3\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
