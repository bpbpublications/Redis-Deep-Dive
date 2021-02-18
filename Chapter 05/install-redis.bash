#!/bin/bash
if [[ $(id -u) != 0 ]] ; then
    echo "Must be run as root" >&2
    exit 1
fi
apt update
apt install redis-server
# Just in case, ...
systemctl stop redis-server
# Change "supervised no" so "supervised systemd"? Question is unclear
# If "#bind 127.0.0.1 ::1", change to "bind 127.0.0.1 ::1"
sed -e '/^supervised no/supervised systemd/' \
    -e 's/^# *bind 127\.0\.0\.1 ::1/bind 127.0.0.1 ::1' \
    /etc/redis/redis.conf >/etc/redis/redis.conf.new
# $(date +%y%b%d-%H%M%S) == "18Aug13-125913"
mv /etc/redis/redis.conf /etc/redis/redis.conf.$(date +%y%b%d-%H%M%S)
mv /etc/redis/redis.conf.new /etc/redis/redis.conf
systemctl start redis-server
# give redis-server a second to wake up
sleep 1
if [[ "$( echo 'ping' | /usr/bin/redis-cli )" == "PONG" ]] ; then
    echo "ping worked"
else
    echo "ping FAILED"
fi
systemctl status redis
systemctl status redis-server
exit 0