cd /home/zcruz/Documents/IIITH_Assignments/IIITH/Sem2/IAS/Hackathon/hackathon2/kafka_2.13-2.7.0/

xterm -e "bin/zookeeper-server-start.sh config/zookeeper.properties" & 
sleep 5
xterm -e "JMX_PORT=8004 bin/kafka-server-start.sh config/server.properties" &
sleep 5


cd /home/zcruz/Documents/IIITH_Assignments/IIITH/Sem2/IAS/Hackathon/hackathon2/CMAK-master/target/universal/cmak-3.0.0.5/

xterm -e "bin/cmak -Dconfig.file=conf/application.conf -Dhttp.port=8080"



