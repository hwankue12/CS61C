#!/bin/bash

export JAVA_HOME=${JAVA_HOME-/usr/lib/jvm/java-6-openjdk/jre}

#standard initialization
ec2-util --init

#configure cluster type
echo "[$USER-large]
image_id=ami-fd4aa494
instance_type=c1.xlarge
key_name=$USER-default
private_key=$HOME/$USER-default.pem
ssh_options=-i %(private_key)s -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null
user_data_file=https://s3.amazonaws.com/lab3/hadoop-cs61cec2-init-remote.sh
proxy_port=41484
client_cidr=0.0.0.0/0" > ~/.hadoop-cloud/clusters.cfg

#generate authentication certificate
new-ec2-certificate
