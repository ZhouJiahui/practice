#!/bin/bash

#./tun_interface nt-test0
ifconfig nt-test0 0.0.0.0 up
ip route add 10.10.10.1 dev nt-test0
 
