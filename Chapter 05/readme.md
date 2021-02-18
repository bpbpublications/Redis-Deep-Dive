## installation commands as per book:

Downloads the redis server from the official Ubuntu package repo and installs it.

```
$ sudo apt update
$ sudo apt install redis-server
```
set password using the “requirepass” attribute.

```

requirepass "your-redis-db-password”

```

Restart Redis
```
sudosystemctl restart redis.service
```

Redis Server Status

```
sudosystemctl status redis 
```

## installing Redis in one click:
To install and configure Redis run the accompanying file:
```
chmod 777 install-redis.bash
./install-redis.bash
```
## installing Redis using [docker](https://en.wikipedia.org/wiki/Docker_(software)):

#### Install Docker:
```
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
```

#### Install Redis on Docker:
```
docker run --name some-redis -d redis
```

#### Connecting to Redis Docker instance:
```
 docker run -it --network some-network --rm redis redis-cli -h some-redis
```

## Redis Configuration

We start another redis-server process on port 6380, which is syncing data from our redis-server process running on 6379.

```
sudo ./redis-server --port 6380 --slaveof 127.0.0.1 6379
```
Save after 900 seconds if there is at least 1 change to the dataset and after 300 seconds if there are at least 10 changes to the dataset
```
CONFIG SET SAVE "900 1 300 10"
```
list of all the supported configuration parameters
```
CONFIG GET * 
```

## Redis Scripting

Registering a lua script
```
EVAL 'local val="Hello Compose" return val' 0
```

EVAL command syntax:
```
eval "LUA SCRIPT" [No. Of Arguments] key1 key2 first second
```
