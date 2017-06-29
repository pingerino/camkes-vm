#!/usr/bin/env python3

import sys
import sh
import pexpect
import re
import pdb
# parameters in milliseconds
START_BUDGET = 1
END_BUDGET = 10
PERIOD = 10
ITERATIONS = 10

# commands
CONSOLE_CMD = 'console -f %s'
IPBENCH = """ipbench --test=latency --test-args="Mbps=10,size=100,socktype=udp" -p 8036 --client=192.168.1.101 --client=192.168.1.103 --test-target=192.168.1.98 --test-port=7"""


# results format
# Request throughput, achieved throughput, sent throughput, Size, min, Avg, max, std.dev, median
RESULTS_RE = re.compile('(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+)\.(\d+),(\d+)\s')

def spawn_console(machine):
    console = pexpect.spawnu(CONSOLE_CMD % machine)
    console.logfile = sys.stdout
    console.expect('[Enter `^Ec? for help]')
    # check the shell is running
    console.sendline('')
    console.expect('#')
    return console

def ipbenchd(console):
    # kill any active sessions
    console.sendline('pkill python')
    console.expect('#')
    console.sendline('ipbenchd &')
    console.expect('#')

def ipbench(console, output, budget, period):
    for i in range (0, ITERATIONS):
        console.sendline(IPBENCH)
        # wait until ipbench finishes
        console.expect(RESULTS_RE, timeout=5*60)
        output.write('{0},{1},'.format(budget * 1000, PERIOD * 1000))
        output.write(console.match.group(0))
        output.write('\n')

def changesc(console, budget, period):
    console.sendline('changesc {0} {1}'.format(budget * 1000, period * 1000))
    console.expect('Result 0')
    console.expect('#')

def main():

    # open outputfile
    output = open('output', 'w')

    # we assume haswell1, haswell2 and haswell3 are awake, logged in and running the correct linux
    # spawn a console to each one
    haswell1 = spawn_console('haswell1')
    haswell2 = spawn_console('haswell2')
    haswell3 = spawn_console('haswell3')

    # start ipbenchd on load generators
    ipbenchd(haswell1)
    ipbenchd(haswell2)

    # output header
    output.write('Budget,period,')
    output.write('Request throughput,achieved throughput,sent throughput,')
    output.write('Size,min,Avg,max,std.dev,median')
    output.write('\n')

    output.write('0, 0,')
    ipbench(haswell1, output, 0, 0)

    for i in range(START_BUDGET, END_BUDGET):
        # configure linux on haswell3
        changesc(haswell3, i, PERIOD)
        # make linux spin
        haswell3.sendline('yes > /dev/null')
        # run benchmark
        ipbench(haswell1, output, i, PERIOD)
        # stop linux spinning
        haswell3.sendcontrol('C')

    # done!
    output.close()


if __name__ == '__main__':
    sys.exit(main())
