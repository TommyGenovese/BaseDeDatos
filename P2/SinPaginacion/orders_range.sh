#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "2\r"

expect "Enter a number that corresponds to your choice > "
send -- "2\r"

expect "Enter dates (YYYY-MM-DD - YYYY-MM-DD) > "
send -- "2003-01-10 - 2003-04-21\r"

# 7933 -> stock for product with id = S10_1678
expect "10102 2003-01-10 2003-01-14"
expect "10103 2003-01-29 2003-02-02"
expect "10104 2003-01-31 2003-02-01"
expect "10105 2003-02-11 2003-02-12"
expect "10106 2003-02-17 2003-02-21"
expect "10107 2003-02-24 2003-02-26"
expect "10108 2003-03-03 2003-03-08"
expect "10109 2003-03-10 2003-03-11"
expect "10110 2003-03-18 2003-03-20"
expect "10111 2003-03-25 2003-03-30"
expect "10112 2003-03-24 2003-03-29"
expect "10113 2003-03-26 2003-03-27"
expect "10114 2003-04-01 2003-04-02"
expect "10115 2003-04-04 2003-04-07"
expect "10116 2003-04-11 2003-04-13"
expect "10117 2003-04-16 2003-04-17"
expect "10118 2003-04-21 2003-04-26"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
