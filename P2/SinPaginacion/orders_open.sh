#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "2\r"

expect "Enter a number that corresponds to your choice > "
send -- "1\r"


# 7933 -> stock for product with id = S10_1678
expect "10167"
expect "10248"
expect "10260"
expect "10262"
expect "10334"
expect "10401"
expect "10407"
expect "10414"
expect "10420"
expect "10421"
expect "10422"
expect "10423"
expect "10424"
expect "10425"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
