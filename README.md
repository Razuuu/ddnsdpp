# ddnsd++

## Description
DDNSD++ is a background service to dynamically update  
your IP adress in a DNS Zone file.  
It currently supports two secondary dns services  
which it is capable of updating master-ip for.  
Supported secondary dns services are:  
https://puck.nether.net/dns  
https://dns.he.net  

## Requirements
- Linux / Unix based operating system
- libcurl4
- systemd
- bind9 installed on localhosts

## Build Requirements
- Linux / Unix based operating system
- g++ / clang++
- make
- libcurl4-openssl-dev
