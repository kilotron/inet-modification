import os
import re
import math

# 适用于omnet++5.5.1版本的ini文件解析脚本，能把所有的config解析为linux执行脚本

file = 'static.ini'
scriptPath = './'
ini = open(file)

head = 'screen -d -m -S '
run = ' opp_run -r '
cmd = ' -m -u Cmdenv -c '
tail = ' -n ../../../src:../..:../../../tutorials:../../../showcases --image-path=../../../images -l ../../../src/INET --cmdenv-redirect-output=false --record-eventlog=false --scalar-recording=false --vector-recording=false static.ini\n'


config = ''

# 两组成一个执行命令,目前需要一个config下仿真的组数是偶数组
num = 2

while True:
    line = ini.readline()

    if not line:

        if config:
            script.write(head + config + run + '0' + cmd + config + tail)
            script.close()
            break
        else:
            break

    # 匹配配置Config
    if re.match(r'\[.*\]',line):
        if not config:
            config = line.strip('[]\n\r')

            # 不对General配置生成命令
            if config == 'General':
                config = ''
                continue
            config = config.split(' ')
            config = config[1]
            script = open(scriptPath + config+'.sh', 'w')
        else:
            # 读到下一个config时，把上一个config的内容输出
            script = open(scriptPath + config + '.sh', 'w')
            script.write(head + config + run + '0' + cmd + config + tail)

            script.close()
            config = line.strip('[]\n\r')
            config = config.split(' ')
            config = config[1]
            script = open(scriptPath + config + '.sh', 'w')

    # 匹配多个的参数，根据参数生产命令
    elif re.search(r'\$\{.*\}',line) and line[0] != '#':
        match = re.search(r'\$\{.*\}',line)
        line = match.group()
        line = line.strip('${}')
        if 'step' in line:
            args = re.split(r'\.{2}|\s', line)
            begin = float(args[0])
            end = float(args[1])
            step = float(args[len(args)-1])
            ran = round((end - begin) / step)

            for i in range(int((ran + 1) / num)):
                index = str(i)+','+str(ran - i)
                script.write(head + config + str(i) + run + index + cmd + config + tail)
            config = ''
            script.close()

os.system("chmod +x ./*.sh")


