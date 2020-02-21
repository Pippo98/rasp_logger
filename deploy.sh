 #!/bin/bash         

echo "Deploying files"
scp C/* pi@192.168.43.131:"/home/pi/Programmazione/Github/rasp_logger/C/"
#scp C/* pi@192.168.1.190:"/home/pi/Programmazione/Github/rasp_logger/C/"
